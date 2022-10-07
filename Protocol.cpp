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

Protocol::Protocol(Server* sv)
	: sv_(sv) {}

Protocol::~Protocol(void) {}

std::string    Protocol::rplPass()
{
	std::string ret;

	ret = RPL_NONE;
	ret += " " +  sv_->getName() + " :info) Successful Authentication.\r\n";
	return ret;
}

// 001
std::string     Protocol::rplWelcome(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_WELCOME;
	ret += " " + clnt->getNickname();
	ret += " :Welcome to the IRC Network ";
	ret += clnt->getUserRealHostNamesInfo();
	ret += "\r\n"; // todo: getRCLF()
	return ret;
}

// 002
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

// 003
std::string     Protocol::rplCreated(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_CREATED;
	ret += " " + clnt->getNickname();
	ret += " :This server was created ";
	ret += "05:22:40 Sep 25 2022"; // todo: sv_->getCreatedDate()
	ret += "\r\n";
	return ret;
}

// 004
std::string     Protocol::rplMyInfo(Client* clnt)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += RPL_MYINFO;
	ret += " " + clnt->getNickname();
	ret += " " + sv_->getName();
	ret += " " + sv_->getVersion();
	ret += " iosw"; // available user modes
	ret += " biklmnopstv"; // available channel modes
	ret += " :bklov";
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

std::string		Protocol::msgPong(std::string token)
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

std::string		Protocol::rplErrorClosing(Client* clnt, std::string msg)
{
	std::string	ret;

	ret = "ERROR :Closing link: ";
	ret += '(' + clnt->getUsername() + '@' + clnt->getHostname() + ')';
	ret += " [Quit: " + msg + "]";
	ret += "\r\n";
	return ret;
}

std::string		Protocol::clntQuit(Client* clnt, std::string msg)
{
	std::string	ret;

	ret = clnt->getNamesPrefix();
	ret += "QUIT :";
	ret += "Quit: " + msg;
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


std::string     Protocol::errWrongPasswd()
{
	std::string ret;

	ret = ERR_PASSWDMISMATCH;
	ret += " " + sv_->getName() + " :info) Wrong password\r\n";
	return ret;
}

std::string     Protocol::errNotPassCmd()
{
	std::string ret;

	ret = ERR_PASSWDMISMATCH;
	ret += " " + sv_->getName() + " :ex) <PASS> <password>\r\n";
	return ret;
}

std::string     Protocol::errNicknameInUse(const std::string& nick)
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret += ERR_NICKNAMEINUSE;
	ret += " *";
	ret += " " + nick;
	ret += " :Nickname is already in use.\r\n";
	return ret;
}

std::string     Protocol::errNoNicknameGiven()
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret = ERR_NONICKNAMEGIVEN;
	ret += " :No nickname is given.\r\n";
	return ret;
}

std::string     Protocol::errNeedMoreParams()
{
	std::string ret;

	ret = sv_->getNamePrefix();
	ret = ERR_NEEDMOREPARAMS;
	ret += " :Not enough parameters.\r\n";
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


Server* Protocol::getServer_(void)
{
	return sv_;
}
