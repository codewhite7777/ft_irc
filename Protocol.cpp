/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Protocol.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:35:59 by mgo               #+#    #+#             */
/*   Updated: 2022/10/01 16:58:34 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Protocol.hpp"
#include "irc_protocol.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Chatbot.hpp"

Protocol::Protocol(Server* sv)
	: sv_(sv) {}

Protocol::~Protocol(void) {}

std::string    Protocol::rplPass()
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_NONE;
	ret += " :info) Successful Authentication.";
	ret += "\r\n";
	return ret;
}

std::string     Protocol::rplWelcome(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_WELCOME;
	ret += " " + clnt->getNickname();
	ret += " :Welcome to the IRC Network ";
	ret += clnt->getUserRealHostNamesInfo();
	ret += "\r\n";
	return ret;
}

std::string     Protocol::rplYourHost(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_YOURHOST;
	ret += " " + clnt->getNickname();
	ret += " :Your host is ";
	ret += sv_->getName();
	ret += ", running version ";
	ret += sv_->getVersion();
	ret += "\r\n";
	return ret;
}

std::string     Protocol::rplCreated(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_CREATED;
	ret += " " + clnt->getNickname();
	ret += " :This server was created ";
	ret += sv_->getCreatedDateAsString();
	ret += "\r\n";
	return ret;
}

std::string     Protocol::rplMyInfo(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_MYINFO;
	ret += " " + clnt->getNickname();
	ret += " " + sv_->getName();
	ret += " " + sv_->getVersion();
	ret += " o";
	ret += " o";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::rplNamReply(Client* clnt, Channel* chann)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += RPL_NAMREPLY;
	ret += " " + clnt->getNickname();
	ret += " = " + chann->getName();
	ret += " :";
	ret += chann->getUserListStr();
	ret += "\r\n";
	return ret;
}

std::string		Protocol::rplEndOfNames(Client* clnt, Channel* chann)
{
	std::string	ret;
	
	ret = sv_->getNamePrefix();
	ret += RPL_ENDOFNAMES;
	ret += " " + clnt->getNickname();
	ret += " " + chann->getName();
	ret += " :End of /NAMES list.";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::rplInviting(Client* clnt, \
									Client* clnt_to_be_invtd, Channel* chann)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += RPL_INVITING;
	ret += " " + clnt->getNickname();
	ret += " " + clnt_to_be_invtd->getNickname();
	ret += " :" + chann->getName();
	ret += "\r\n";
	return ret;
}

std::string		Protocol::rplYoureOper(Client* clnt)
{
	std::string		ret;

	ret = sv_->getNamePrefix();
	ret += RPL_YOUREOPER;
	ret += " " + clnt->getNickname();
	ret += " :You are now a " + sv_->getOperType();
	ret += "\r\n";
	return ret;
}

std::string		Protocol::svPrivmsgClntWhenInit(Client* clnt)
{
	std::string		ret;

	ret = sv_->getNamePrefix();
	ret += "PRIVMSG";
	ret += " " + clnt->getNickname();
	ret += " :*** I/O logging is enabled on this server.";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntJoinChann(Client* clnt, Channel* chann)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "JOIN :";
	ret += chann->getName();
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntPartChann(Client* clnt, std::string msg)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "PART ";
	ret += msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::svPong(std::string token)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += "PONG ";
	ret += sv_->getName();
	ret += " :" + token;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntPrivmsgToChann(Client* clnt, std::string msg, \
											Channel* chann)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "PRIVMSG ";
	ret += chann->getName();
	ret += " " + msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntPrivmsgToClnt(Client* clnt_send, std::string msg, \
											Client* clnt_recv)
{
	std::string	ret;

	ret = clnt_send->getNamesPrefix();
	ret += "PRIVMSG ";
	ret += clnt_recv->getNickname();
	ret += " " + msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntNoticeToChann(Client* clnt, std::string msg, \
										Channel* chann)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "NOTICE ";
	ret += chann->getName();
	ret += " " + msg;
	int	i(0);
	ret += std::to_string(i);
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntNoticeToClnt(Client* clnt_send, std::string msg, \
									Client* clnt_recv)
{
	std::string	ret;

	ret = clnt_send->getNamesPrefix();
	ret += "NOTICE ";
	ret += clnt_recv->getNickname();
	ret += " " + msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errorClosingLink(Client* clnt, std::string msg)
{
	std::string	ret;

	ret = "ERROR :Closing link: ";
	ret += '(' + clnt->getUsername() + '@' + clnt->getHostname() + ')';
	ret += " [" + msg + "]";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntQuit(Client* clnt, std::string msg)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "QUIT :";
	ret += msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntNick(Client* clnt, std::string nick)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "NICK :";
	ret += nick;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntKickUserInChann(Client* clnt, \
								Channel* chann, Client* user, std::string msg)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "KICK";
	ret += " " + chann->getName();
	ret += " " + user->getNickname();
	ret += " " + msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntInviteClnt(Client* clnt, \
									Client* clnt_to_be_invtd, Channel* chann)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "INVITE";
	ret += " " + clnt_to_be_invtd->getNickname();
	ret += " :" + chann->getName();
	ret += "\r\n";
	return ret;
}

std::string		Protocol::svModeClntAddOper(Client* clnt)
{
	std::string		ret;

	ret = sv_->getNamePrefix();
	ret += "MODE";
	ret += " " + clnt->getNickname();
	ret += " +o";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntKillClnt(Client* clnt, \
									Client* target_clnt, std::string msg)
{
	std::string		ret;

	ret = clnt->getNamesPrefix();
	ret += "KILL";
	ret += " " + target_clnt->getNickname();
	ret += " :" + msg;
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errChanOPrivsNeeded(Client* clnt, Channel* chann)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_CHANOPRIVSNEEDED;
	ret += " " + clnt->getNickname();
	ret += " " + chann->getName();
	ret += " :You must be a channel operator";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errUserNotInChannel(Client* clnt, \
										Client* user, Channel *chann)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_USERNOTINCHANNEL;
	ret += " " + clnt->getNickname();
	ret += " " + user->getNickname();
	ret += " " + chann->getName();
	ret += " :The client is not on that channel";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::chatPrivOthers(ChatBot& chat, Channel *chann, std::string msg)
{
	std::string ret;

	ret = chat.getPrefix();
	ret += "PRIVMSG";
	ret += " " + chann->getName();
	ret += " :";
	ret += msg;
	ret += "\r\n";

	return ret;
}

std::string     Protocol::errWrongPasswd()
{
	std::string ret;

	ret = ERR_PASSWDMISMATCH;
	ret += " " + sv_->getName();
	ret += " :info) Wrong password";
	ret += "\r\n";
	return ret;
}

std::string     Protocol::errNotPassCmd()
{
	std::string ret;

	ret = ERR_PASSWDMISMATCH;
	ret += " " + sv_->getName();
	ret += " :ex) <PASS> <password>";
	ret += "\r\n";
	return ret;
}

std::string     Protocol::errNicknameInUse(Client* clnt, std::string nick)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NICKNAMEINUSE;
	ret += " " + clnt->getNickname();
	ret += " " + nick;
	ret += " :Nickname is already in use.";
	ret += "\r\n";
	return ret;
}

std::string     Protocol::errNoNicknameGiven(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NONICKNAMEGIVEN;
	ret += " " + clnt->getNickname();
	ret += " :No nickname is given.";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errErroneusNickname(Client* clnt, std::string nick)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_ERRONEUSNICKNAME;
	ret += " " + clnt->getNickname();
	ret += " " + nick;
	ret += " :Erroneus Nickname";
	ret += "\r\n";
	return ret;
}

std::string     Protocol::errNeedMoreParams()
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret = ERR_NEEDMOREPARAMS;
	ret += " :Not enough parameters.";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errNoSuchNick(Client* clnt, std::string nick)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NOSUCHNICK;
	ret += " " + clnt->getNickname();
	ret += " " + nick + " :No such nick";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errNoSuchChannel(Client* clnt, std::string chann_name)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NOSUCHCHANNEL;
	ret += " " + clnt->getNickname();
	ret += " " + chann_name + " :No such channel";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errNotOnChannel(Client* clnt, Channel* chann)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NOTONCHANNEL;
	ret += " " + clnt->getNickname();
	ret += " " + chann->getName();
	ret += " :You're not on that channel";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errUserOnChannel(Client* clnt, \
										Client* user, Channel* chann)
{
	std::string	ret;

	ret = sv_->getNamePrefix();
	ret += ERR_USERONCHANNEL;
	ret += " " + clnt->getNickname();
	ret += " " + user->getNickname();
	ret += " " + chann->getName();
	ret += " :is already on channel";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errNoOperHost(Client* clnt)
{
	std::string		ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NOOPERHOST;
	ret += " " + clnt->getNickname();
	ret += " :Invalid oper credentials";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errNoPrivileges(Client* clnt)
{
	std::string		ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NOPRIVILEGES;
	ret += " " + clnt->getNickname();
	ret += " :Permission Denied";
	ret += " - You do not have the required operator privileges";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::errBadChanMask(Client* clnt, std::string chann_name)
{
	std::string		ret;

	ret = sv_->getNamePrefix();
	ret += ERR_BADCHANMASK;
	ret += " " + clnt->getNickname();
	ret += " " + chann_name;
	ret += "\r\n";
	return ret;
}

Server* Protocol::getServer_(void)
{
	return sv_;
}
