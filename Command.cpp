/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:33:06 by mgo               #+#    #+#             */
/*   Updated: 2022/10/01 16:57:04 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

Command::Command(Server* sv, Protocol* proto)
	: sv_(sv), proto_(proto) {}

Command::~Command(void) {}

void    Command::pass(Client* clnt)
{
	if (clnt->getParam() == sv_->getPwd())
	{
		clnt->setPassFlag(true);
		clnt->appendToSendBuf(proto_->rplPass());
	}
	else
	{
		clnt->appendToSendBuf(proto_->errWrongPasswd());
	}
}

void    Command::nick(Client* clnt)
{
	std::string     tmp_nick(clnt->getParam());

	if (sv_->isOverlapNickName(tmp_nick))
	{
		clnt->appendToSendBuf(proto_->errNicknameInUse(tmp_nick));
	}
	else if (tmp_nick == "")
	{
		clnt->appendToSendBuf(proto_->errNoNicknameGiven());
	}
	else
	{
		clnt->setNickname(tmp_nick);
		clnt->setNickFlagOn();

		// test: print
		{
		std::cout << "\n\t-----------------------------------" << std::endl;
		std::cout << "\tSuccessfully set Client Nickname!\n";
		std::cout << "\tClient Nickname: [" << clnt->getNickname() << "]\n";
		std::cout << "\t-----------------------------------\n" << std::endl;
		}
	}
}

void    Command::user(Client* clnt)
{
	std::string     args(clnt->getParam());
	std::size_t     colon_pos;

	colon_pos = args.find_first_of(':');
	if (colon_pos != std::string::npos)
	{
		std::size_t cnt_arg(0);
		std::size_t space_pos(args.rfind(' ', colon_pos));
		while (space_pos != std::string::npos)
		{
			++cnt_arg;
			space_pos = args.rfind(' ', --space_pos);
		}
		if (cnt_arg == 3)
		{
			clnt->setRealname(args.substr(colon_pos + 1));
			args.erase(colon_pos, std::string::npos);

			std::vector<std::string>    spltd_args(split(args, ' '));
			clnt->setUsername(spltd_args[0]);
			clnt->setHostname(spltd_args[2]);
			clnt->setUserFlagOn();

			// test: print
			{
			std::cout << "\n\t-----------------------------------" << std::endl;
			std::cout << "\tSuccessfully processed USER command\n";
			std::cout << "\tClient Username: [" << clnt->getUsername() << "]\n";
			std::cout << "\tClient Hostname: [" << clnt->getHostname() << "]\n";
			std::cout << "\tClient Realname: [" << clnt->getRealname() << "]\n";
			std::cout << "\t-----------------------------------\n" << std::endl;
			}
		}
		else
		{
			clnt->appendToSendBuf(proto_->errNeedMoreParams());
		}
	}
	else
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
	}
}

void    Command::join(Client* clnt)
{
	std::vector<std::string>	chann_names(split(clnt->getParam(), ','));
	Channel*					chann_ptr(NULL);

	for (std::size_t i = 0 ; i < chann_names.size() ; ++i)
	{
		chann_ptr = sv_->findChannel(chann_names[i]);
		if (chann_ptr != NULL)
		{
			if (chann_ptr->getCurrUserCount() == 0)
				chann_ptr->assignAsOperator(clnt);
			chann_ptr->assignAsUser(clnt);
		}
		else
		{
			chann_ptr = new Channel(chann_names[i]);
			sv_->assignNewChannel(chann_ptr);
			chann_ptr->assignAsOperator(clnt);
			chann_ptr->assignAsUser(clnt);
		}
		chann_ptr->sendToAll(proto_->clntJoinChann(clnt, chann_ptr));
		clnt->appendToSendBuf(proto_->rplNamReply(clnt, chann_ptr));
		clnt->appendToSendBuf(proto_->rplEndOfNames(clnt, chann_ptr));
	}
}

void	Command::part(Client* clnt)
{
	Channel*		chann_ptr(sv_->findChannel(clnt->getParam()));
	std::string		msg_part;

	// todo: process part when message argument is.
	if (chann_ptr)
	{
		chann_ptr->eraseAsOperator(clnt);
		chann_ptr->eraseAsUser(clnt);
		msg_part = proto_->clntPartChann(clnt, chann_ptr);
		clnt->appendToSendBuf(msg_part);
		chann_ptr->sendToAll(msg_part);
	}
}

void	Command::ping(Client* clnt)
{
	std::string	token(clnt->getParam());

	// todo: if no param, send ERR_NEEDMOREPARAMS
	if (token.empty() == false)
		clnt->appendToSendBuf(proto_->msgPong(token));
}

void    Command::privmsg(Client* clnt)
{
	std::string					arg(clnt->getParam());
	std::size_t					found_space(0);
	std::vector<std::string>	names;
	std::string					msg;
	Channel*					chann_ptr(NULL);
	Client*						clnt_recv_ptr(NULL);

	found_space = arg.find(' ');
	if (found_space == std::string::npos)
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
	}
	names = split(arg.substr(0, found_space), ',');
	msg = arg.substr(found_space + 1);
	for (std::size_t i = 0; i < names.size(); ++i)
	{
		if ('#' == names[i].front())
		{
			chann_ptr = sv_->findChannel(names[i]);
			if (chann_ptr)
				chann_ptr->sendToOthers(clnt, \
					proto_->clntPrivmsgToChann(clnt, msg, chann_ptr));
			else
				clnt->appendToSendBuf(proto_->errNoSuchChannel(clnt, names[i]));
		}
		else
		{
			clnt_recv_ptr = sv_->findClient(names[i]);
			if (clnt_recv_ptr)
				clnt_recv_ptr->appendToSendBuf(proto_->clntPrivmsgToClnt(\
												clnt, msg, clnt_recv_ptr));
			else
				clnt->appendToSendBuf(proto_->errNoSuchNick(clnt, names[i]));
		}
	}
}

#include <list>

void	Command::quit(Client* clnt)
{
	std::string			msg(clnt->getParam());
	std::list<Client*>*	clnts_in_same_channs(NULL);
	Client*				each_clnt_ptr(NULL);

	if (':' == msg.front())
		msg.erase(0, 1);
	clnt->appendToSendBuf(proto_->rplErrorClosing(clnt, msg));
	clnts_in_same_channs = sv_->makeOtherClntListInSameChanns(clnt); // make user list in same channels
	// loop -> each_client->appendSendBuf(proto_->clntQuit(clnt));
	for (std::list<Client*>::iterator each_clnt_it(clnts_in_same_channs->begin())
		; each_clnt_it != clnts_in_same_channs->end()
		; ++each_clnt_it)
	{
		each_clnt_ptr = *each_clnt_it;
		each_clnt_ptr->appendToSendBuf(proto_->clntQuit(clnt, msg));
		each_clnt_ptr = NULL;
	}
	delete clnts_in_same_channs;
	sv_->requestChannsToEraseOne(clnt);
	clnt->setDisconnectFlag(true);
}
