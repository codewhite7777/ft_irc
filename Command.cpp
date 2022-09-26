/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:33:06 by mgo               #+#    #+#             */
/*   Updated: 2022/09/26 18:20:00 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

Command::Command(Server* sv)
	: sv_(sv), proto_(sv_->getProtocol()) {}

Command::~Command(void) {}

void    Command::pass(Client* clnt)
{
	Protocol    proto(sv_);

	if (clnt->getParam() == sv_->getPwd())
	{
		clnt->setPassFlag(true);
		clnt->appendToSendBuf(proto.rplPass());
	}
	else
	{
		clnt->appendToSendBuf(proto.errWrongPasswd());
	}
}

void    Command::nick(Client* clnt)
{
	Protocol        proto(sv_);
	std::string     tmp_nick(clnt->getParam());

	if (sv_->isOverlapNickName(tmp_nick))
	{
		clnt->appendToSendBuf(proto.errNicknameInUse(tmp_nick));
	}
	else if (tmp_nick == "")
	{
		clnt->appendToSendBuf(proto.errNoNicknameGiven());
	}
	else
	{
		clnt->setNickname(tmp_nick);
		clnt->setNickFlagOn();

		// test: print
		std::cout << "\n\t-----------------------------------" << std::endl;
		std::cout << "\tSuccessfully set Client Nickname!\n";
		std::cout << "\tClient Nickname: [" << clnt->getNickname() << "]\n";
		std::cout << "\t-----------------------------------\n" << std::endl;
	}
}

void    Command::user(Client* clnt)
{
	Protocol        proto(sv_);
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
			clnt->appendToSendBuf(proto.errNeedMoreParams());
		}
	}
	else
	{
		clnt->appendToSendBuf(proto.errNeedMoreParams());
	}
}

void    Command::join(Client* clnt)
{
	Protocol        			proto(sv_);
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
		//send Protocols
		chann_ptr->sendToAll(proto.clntJoinChann(clnt, chann_ptr));
		clnt->appendToSendBuf(proto.rplNamReply(clnt, chann_ptr));
		clnt->appendToSendBuf(proto.rplEndOfNames(clnt, chann_ptr));
	}
}

void	Command::part(Client* clnt)
{
	(void)clnt;
}