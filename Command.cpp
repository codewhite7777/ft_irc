/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:33:06 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:57:10 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Protocol.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Chatbot.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

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
	std::string			param(clnt->getParam());
	std::string     	tmp_nick;
	std::size_t			pos_found_ws(0);
	std::list<Client*>*	clnts_in_same_channs(NULL);
	Client*				each_clnt_ptr(NULL);

	pos_found_ws = param.find_first_of(" \n\r\t\f\v");
	tmp_nick = param.substr(0, pos_found_ws);
	if (sv_->isOverlapNickName(tmp_nick))
	{
		if (tmp_nick != clnt->getNickname())
			clnt->appendToSendBuf(proto_->errNicknameInUse(clnt, tmp_nick));
	}
	else if (tmp_nick.empty())
	{
		clnt->appendToSendBuf(proto_->errNoNicknameGiven(clnt));
	}
	else if (tmp_nick.length() > 30 || isnumber(tmp_nick.front()) \
			|| isOnlyNums(tmp_nick) || isNotAlnumAndUnderscore(tmp_nick))
	{
		clnt->appendToSendBuf(proto_->errErroneusNickname(clnt, tmp_nick));
	}
	else
	{
		if (clnt->isWelcomed())
		{
			clnt->appendToSendBuf(proto_->clntNick(clnt, tmp_nick));
			clnts_in_same_channs = sv_->makeOtherClntListInSameChanns(clnt);
			for (std::list<Client*>::iterator each_clnt_it(clnts_in_same_channs->begin())
				; each_clnt_it != clnts_in_same_channs->end()
				; ++each_clnt_it)
			{
				each_clnt_ptr = *each_clnt_it;
				each_clnt_ptr->appendToSendBuf(proto_->clntNick(clnt, tmp_nick));
				each_clnt_ptr = NULL;
			}
			delete clnts_in_same_channs;
			sv_->requestAllChannsToReplaceKeyNickOfUser(clnt, tmp_nick);
		}
		else
		{
			clnt->setNickFlagOn();
		}
		clnt->setNickname(tmp_nick);
	}
}

void    Command::user(Client* clnt)
{
	std::string     			args(clnt->getParam());
	std::size_t     			colon_pos(0);
	std::size_t 				cnt_arg(0);
	std::size_t 				space_pos(0);
	std::vector<std::string>    spltd_args;

	colon_pos = args.find_first_of(':');
	if (colon_pos != std::string::npos)
	{
		space_pos = args.rfind(' ', colon_pos);
		while (space_pos != std::string::npos)
		{
			++cnt_arg;
			space_pos = args.rfind(' ', --space_pos);
		}
		if (cnt_arg == 3)
		{
			clnt->setRealname(args.substr(colon_pos + 1));
			args.erase(colon_pos, std::string::npos);
			spltd_args = split(args, ' ');
			clnt->setUsername(spltd_args[0]);
			clnt->setHostname(spltd_args[2]);
			clnt->setUserFlagOn();
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
		if (chann_names[i].length() >= 65)
		{
			clnt->appendToSendBuf(proto_->errBadChanMask(clnt, chann_names[i]));
			continue;
		}
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
	std::string		recved_arg(clnt->getParam());
	std::string		chann_name;
	std::string		msg_part;
	std::size_t		found_space(0);
	Channel*		chann_ptr(NULL);
	std::string		protocol_msg_to_send;

	if (recved_arg.find_first_of(":") == std::string::npos)
	{
		chann_name = recved_arg;
		msg_part = ':' + chann_name;
	}
	else
	{
		found_space = recved_arg.find(' ');
		if (found_space == std::string::npos)
		{
			clnt->appendToSendBuf(proto_->errNeedMoreParams());
			return ;
		}
		chann_name = recved_arg.substr(0, found_space);
		msg_part = recved_arg;
	}
	chann_ptr = sv_->findChannel(chann_name);
	if (chann_ptr)
	{
		if (chann_ptr->isUserIn(clnt) == true)
		{
			chann_ptr->eraseClntIfIs(clnt);
			protocol_msg_to_send = proto_->clntPartChann(clnt, msg_part);
			clnt->appendToSendBuf(protocol_msg_to_send);
			chann_ptr->sendToAll(protocol_msg_to_send);
		}
		else
		{
			clnt->appendToSendBuf(proto_->errNotOnChannel(clnt, chann_ptr));
		}
	}
	else
	{
		clnt->appendToSendBuf(proto_->errNoSuchChannel(clnt, chann_name));
	}
}

void	Command::ping(Client* clnt)
{
	std::string	token(clnt->getParam());

	if (token.empty() == false)
		clnt->appendToSendBuf(proto_->svPong(token));
	else
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
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
		return ;
	}
	names = split(arg.substr(0, found_space), ',');
	msg = arg.substr(found_space + 1);
	for (std::size_t i = 0; i < names.size(); ++i)
	{
		if ('#' == names[i].front())
		{
			chann_ptr = sv_->findChannel(names[i]);
			
			if (chann_ptr)
			{
				chann_ptr->sendToOthers(clnt, \
					proto_->clntPrivmsgToChann(clnt, msg, chann_ptr));

			
				std::string bot_msg = "";
				// todo: setting function in if statement with removing ret
				bool ret = chann_ptr->GetChatBot().ChatBotCommand(msg, bot_msg);
				if (ret)
				{	
					// todo: setting bot privmsg protocol
					if (bot_msg != "")
						
						chann_ptr->sendToAll(proto_->chatPrivOthers(
								chann_ptr->GetChatBot(), chann_ptr, bot_msg));
				}
			}
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

void	Command::notice(Client* clnt)
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
		return ;
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
					proto_->clntNoticeToChann(clnt, msg, chann_ptr));
			else
				clnt->appendToSendBuf(proto_->errNoSuchChannel(clnt, names[i]));
		}
		else
		{
			clnt_recv_ptr = sv_->findClient(names[i]);
			if (clnt_recv_ptr)
				clnt_recv_ptr->appendToSendBuf(proto_->clntNoticeToClnt(\
												clnt, msg, clnt_recv_ptr));
			else
				clnt->appendToSendBuf(proto_->errNoSuchNick(clnt, names[i]));
		}
	}
}

void	Command::quit(Client* clnt)
{
	std::string			reason(clnt->getParam());
	std::list<Client*>*	clnts_in_same_channs(NULL);
	Client*				each_clnt_ptr(NULL);

	if (':' == reason.front())
		reason.erase(0, 1);
	reason.insert(0, "Quit: ");
	clnt->appendToSendBuf(proto_->errorClosingLink(clnt, reason));
	clnts_in_same_channs = sv_->makeOtherClntListInSameChanns(clnt);
	for (std::list<Client*>::iterator each_clnt_it(clnts_in_same_channs->begin())
		; each_clnt_it != clnts_in_same_channs->end()
		; ++each_clnt_it)
	{
		each_clnt_ptr = *each_clnt_it;
		each_clnt_ptr->appendToSendBuf(proto_->clntQuit(clnt, reason));
		each_clnt_ptr = NULL;
	}
	delete clnts_in_same_channs;
	sv_->requestAllChannsToEraseOneUser(clnt);
	clnt->setDisconnectFlag(true);
}

void	Command::kick(Client* clnt)
{
	std::string		params(clnt->getParam());
	std::string		chann_name;
	std::string		nick;
	std::string		recved_msg;
	std::size_t		found_space(0);
	std::size_t		pos_to_parse(0);
	Channel*		ptr_chann(NULL);
	Client*			ptr_clnt_to_be_kicked(NULL);

	// parsing for chann_name and nick
	found_space = params.find(' ');
	if (found_space == std::string::npos)
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
		return ;
	}
	chann_name = params.substr(pos_to_parse, found_space);
	pos_to_parse = found_space + 1;
	found_space = params.find(' ', pos_to_parse);
	if (found_space == std::string::npos)
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
		return ;
	}
	nick = params.substr(pos_to_parse, found_space - pos_to_parse);
	recved_msg = params.substr(found_space + 1);

	// finding the channel and checking whether the command is operator.
	ptr_chann = sv_->findChannel(chann_name);
	if (ptr_chann == NULL)
	{
		clnt->appendToSendBuf(proto_->errNoSuchChannel(clnt, chann_name));
		return ;
	}
	if (ptr_chann->isOperator(clnt) == false)
	{
		clnt->appendToSendBuf(proto_->errChanOPrivsNeeded(clnt, ptr_chann));
		return ;
	}

	// ejecting the user
	ptr_clnt_to_be_kicked = sv_->findClient(nick);
	if (ptr_clnt_to_be_kicked)
	{
		if (ptr_chann->isUserIn(ptr_clnt_to_be_kicked) == true)
		{
			ptr_chann->sendToAll(proto_->clntKickUserInChann(clnt, \
								ptr_chann, ptr_clnt_to_be_kicked, recved_msg));
			ptr_chann->eraseClntIfIs(ptr_clnt_to_be_kicked);
		}
		else
		{
			clnt->appendToSendBuf(proto_->errUserNotInChannel(clnt, \
											ptr_clnt_to_be_kicked, ptr_chann));
		}
	}
	else
	{
		clnt->appendToSendBuf(proto_->errNoSuchNick(clnt, nick));
	}
}

void    Command::invite(Client* clnt)
{
	std::string		params(clnt->getParam());
	std::string		nick;
	std::string		chann_name;
	std::size_t		pos_space_found(0);
	Channel*		ptr_chann(NULL);
	Client*			ptr_clnt_to_be_invtd(NULL);

	// parsing for nick and chann_name
	pos_space_found = params.find(' ');
	if (pos_space_found == std::string::npos)
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
		return ;
	}
	nick = params.substr(0, pos_space_found);
	chann_name = params.substr(pos_space_found + 1);

	// checking condition valid
	ptr_chann = sv_->findChannel(chann_name);
	if (ptr_chann == NULL)
	{
		clnt->appendToSendBuf(proto_->errNoSuchChannel(clnt, chann_name));
		return ;
	}
	ptr_clnt_to_be_invtd = sv_->findClient(nick);
	if (ptr_clnt_to_be_invtd == NULL)
	{
		clnt->appendToSendBuf(proto_->errNoSuchNick(clnt, nick));
		return ;
	}
	if (ptr_chann->isUserIn(ptr_clnt_to_be_invtd))
	{
		clnt->appendToSendBuf(proto_->errUserOnChannel(clnt, \
										ptr_clnt_to_be_invtd, ptr_chann));
		return ;
	}
	if (ptr_chann->isUserIn(clnt) == false)
	{
		clnt->appendToSendBuf(proto_->errNotOnChannel(clnt, ptr_chann));
		return ;
	}

	// checking operator privilege
	if (ptr_chann->isOperator(clnt))
	{
		ptr_clnt_to_be_invtd->appendToSendBuf(proto_->clntInviteClnt(clnt, \
										ptr_clnt_to_be_invtd, ptr_chann));
		clnt->appendToSendBuf(proto_->rplInviting(clnt, \
										ptr_clnt_to_be_invtd, ptr_chann));
	}
	else
	{
		clnt->appendToSendBuf(proto_->errChanOPrivsNeeded(clnt, ptr_chann));
	}
}

void	Command::oper(Client* clnt)
{
	std::string		params(clnt->getParam());
	std::string		name;
	std::string		password;
	std::size_t		pos_space_found(0);

	// parsing for name and password
	pos_space_found = params.find(' ');
	if (pos_space_found == std::string::npos)
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
		return ;
	}
	name = params.substr(0, pos_space_found);
	password = params.substr(pos_space_found + 1);

	// checking information valid to give oper privilege
	if (sv_->isOperName(name)\
	 && sv_->isOperPassword(password)\
	 && sv_->isOperHost(clnt->getHostname()))
	{
		clnt->appendToSendBuf(proto_->svModeClntAddOper(clnt));
		clnt->setSvOperFlagOn();
		clnt->appendToSendBuf(proto_->rplYoureOper(clnt));
	}
	else
	{
		clnt->appendToSendBuf(proto_->errNoOperHost(clnt));
	}
}


void		Command::kill(Client* clnt)
{
	std::string			params(clnt->getParam());
	std::string			target_nick;
	std::string			comment;
	std::size_t			pos_space_found(0);
	std::string			msg_for_proto;
	Client*				target_clnt(NULL);
	std::list<Client*>*	clnts_in_same_channs(NULL);
	Client*				each_clnt_ptr(NULL);

	// parsing for target_nick and comment
	pos_space_found = params.find(' ');
	if (pos_space_found == std::string::npos)
	{
		clnt->appendToSendBuf(proto_->errNeedMoreParams());
		return ;
	}
	target_nick = params.substr(0, pos_space_found);
	comment = params.substr(pos_space_found + 1);
	if (':' == comment.front())
		comment.erase(0, 1);

	// checking condition valid
	if (clnt->isSvOper() == false)
	{
		clnt->appendToSendBuf(proto_->errNoPrivileges(clnt));
		return ;
	}
	target_clnt = sv_->findClient(target_nick);
	if (target_clnt == NULL)
	{
		clnt->appendToSendBuf(proto_->errNoSuchNick(clnt, target_nick));
		return ;
	}

	// sending protocols
	msg_for_proto = "Killed (" + clnt->getNickname();
	msg_for_proto += " (" + comment + "))";
	target_clnt->appendToSendBuf(\
		proto_->clntKillClnt(clnt, target_clnt, msg_for_proto));
	target_clnt->appendToSendBuf(\
		proto_->errorClosingLink(target_clnt, msg_for_proto));
	clnts_in_same_channs = sv_->makeOtherClntListInSameChanns(target_clnt);
	for (std::list<Client*>::iterator each_clnt_it(clnts_in_same_channs->begin())
		; each_clnt_it != clnts_in_same_channs->end()
		; ++each_clnt_it)
	{
		each_clnt_ptr = *each_clnt_it;
		each_clnt_ptr->appendToSendBuf(proto_->clntQuit(target_clnt, msg_for_proto));
		each_clnt_ptr = NULL;
	}
	delete clnts_in_same_channs;
	sv_->requestAllChannsToEraseOneUser(target_clnt);

	target_clnt->setDisconnectFlag(true);
}

void		Command::die(Client* clnt)
{
	std::string		cmd(clnt->getCommand());
	std::string		param(clnt->getParam());
	std::string		msg_to_send;

	// chekcing permission
	if (clnt->isSvOper())
	{
		msg_to_send = "Died (";
		msg_to_send += clnt->getNickname();
		msg_to_send += " (" + param + "))";
		sv_->requestAllClientsToDisconnect();
		sv_->sendErrorClosingLinkProtoToAllClientsWithMsg(msg_to_send);
		sv_->setStatusOff();
	}
	else
	{
		clnt->appendToSendBuf(proto_->errNoPrivileges(clnt));
	}
}
