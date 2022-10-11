/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:01:59 by alee              #+#    #+#             */
/*   Updated: 2022/10/03 13:52:29 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "Protocol.hpp"
#include <iostream>

Client::Client(SOCKET sdes, std::string hostname, Server* sv)
	: sock_des_(sdes)
	, pass_flag_(false)
	, nick_flag_(false)
	, user_flag_(false)
	, nickname_("*")
	, hostname_(hostname)
	, disconnect_flag_(false)
	, sv_oper_flag_(false)
	, passed_(false)
	, welcomed_(false)
	, sv_(sv)
	, cmd_(sv_->getCommand())
	, proto_(sv_->getProtocol()) {}

Client::~Client(void) {}

SOCKET&	Client::getSocket(void)
{
	return sock_des_;
}

void	Client::appendToRecvBuf(unsigned char* buf)
{
	getRecvBuf().append(reinterpret_cast<char *>(buf));
}

size_t		Client::getRecvBufLength()
{
	return (getRecvBuf().length());
}

void		Client::appendToSendBuf(const std::string& str)
{
	getSendBuf().append(str);
}

const char*	Client::getSendBufCStr()
{
	return (getSendBuf().c_str());
}

size_t		Client::getSendBufLength()
{
	return (getSendBuf().length());
}

void		Client::eraseSendBufSize(int size)
{
	getSendBuf().erase(0, size);
}

void	Client::processMessageInRecvBuf()
{
	marshalMessage(command_, param_);
	processProtocol();
	clearCommandAndParam();
}

const std::string&	Client::getCommand() const
{
	return command_;
}

const std::string&	Client::getParam(void) const
{
	return param_;
}

bool	Client::getDisconnectFlag(void) const
{
	return disconnect_flag_;
}

void	Client::setDisconnectFlag(bool flag)
{
	disconnect_flag_ = flag;
}

void	Client::setPassFlag(bool flag)
{
	pass_flag_ = flag;
}

bool	Client::getPassFlag(void) const
{
	return pass_flag_;
}

void			Client::setNickname(std::string nickname)
{
	nickname_ = nickname;
}

const std::string&	Client::getNickname() const
{
	return nickname_;
}

void			Client::setNickFlagOn(void)
{
	nick_flag_ = true;
}

bool	Client::getNickFlag(void) const
{
	return nick_flag_;
}

void			Client::setUsername(std::string username)
{
	username_ = username;
}

void			Client::setHostname(std::string hostname)
{
	hostname_ = hostname;
}

void			Client::setRealname(std::string realname)
{
	realname_ = realname;
}

const std::string&	Client::getUsername() const
{
	return username_;
}

const std::string&	Client::getHostname() const
{
	return hostname_;
}

const std::string&	Client::getRealname() const
{
	return realname_;
}

const std::string	Client::getUserRealHostNamesInfo() const
{
	return (nickname_ + "!" + username_ + "@" + hostname_);
}

void				Client::setUserFlagOn()
{
	user_flag_ = true;
}

bool				Client::getUserFlag() const
{
	return user_flag_;
}

std::string			Client::getNamesPrefix() const
{
	return (":" + nickname_ + "!" + username_ + "@" + hostname_ + " ");
}

void				Client::setSvOperFlagOn()
{
	this->sv_oper_flag_ = true;
}

bool				Client::isSvOper() const
{
	return (this->sv_oper_flag_);
}

void	Client::marshalMessage(std::string& command__, std::string& param__)
{
	std::string	tmp_msg;

	tmp_msg = extractFirstMsg(getRecvBuf());
	if (tmp_msg.find(' ') != std::string::npos)
	{
		command__ = tmp_msg.substr(0, tmp_msg.find(' '));
		param__ = tmp_msg.substr(tmp_msg.find(' ') + 1);
	}
	else
	{
		command__ = "";
		param__ = tmp_msg;
	}

	// test: print command__ and param__
	{
	std::cout << "command__: [" << command__ << "], ";
	std::cout << "param__: [" << param__ << "] ";
	std::cout << "in marshalMessage()\n";
	}
}

/*
	extract first protocol message from packet

	1) [one protocol] If no more behind \r\n, just return pure packet.
	2) [more than one protocol] If something behind \r\n, split packet by first \r\n and return only first protocol.
	3) [no protocol] If no \r\n, return empty.
*/
std::string	Client::extractFirstMsg(std::string& recv_buf)
{
	std::string	first_msg("");
	size_t		found(0);

	found = recv_buf.find("\r\n");
	if (found != std::string::npos)
	{
		first_msg = recv_buf.substr(0, found);
		recv_buf.erase(0, found + 2);
	}
	return (first_msg);
}

void	Client::processProtocol(void)
{
	if (command_.empty() && param_.empty())
		return ;
	if (isWelcomed() == false)
		processAuthToWelcome();
	else
		processCommand();
}

void	Client::processAuthToWelcome()
{
	if (getPassFlag() == false)
	{
		if (command_ == "CAP")
			return ;
		else if (command_ == "PASS")
			cmd_->pass(this);
		else
			appendToSendBuf(proto_->errNotPassCmd());
	}
	else if (getNickFlag() == false || getUserFlag() == false)
	{
		if (command_ == "NICK")
			cmd_->nick(this);
		else if (command_ == "USER")
			cmd_->user(this);
	}
	if (getPassFlag() && getNickFlag() && getUserFlag())
	{
		welcomed_ = true;
		appendToSendBuf(proto_->rplWelcome(this));
		appendToSendBuf(proto_->rplYourHost(this));
		appendToSendBuf(proto_->rplCreated(this));
		appendToSendBuf(proto_->rplMyInfo(this));
	}
}

bool	Client::isPassed() const
{
	return passed_;
}

bool	Client::isWelcomed() const
{
	return welcomed_;
}

void	Client::processCommand()
{
	if (command_ == "PING")
		cmd_->ping(this);
	else if (command_ == "JOIN")
		cmd_->join(this);
	else if (command_ == "PART")
		cmd_->part(this);
	else if (command_ == "PRIVMSG")
		cmd_->privmsg(this);
	else if (command_ == "NOTICE")
		cmd_->notice(this);
	else if (command_ == "QUIT")
		cmd_->quit(this);
	else if (command_ == "KICK")
		cmd_->kick(this);
	else if (command_ == "INVITE")
		cmd_->invite(this);
	else if (command_ == "NICK")
		cmd_->nick(this);
	else if (command_ == "OPER")
		cmd_->oper(this);
	else if (command_ == "kill")
		cmd_->kill(this);
	else if (command_ == "die"\
		 || (command_.empty() && param_ == "die"))
		cmd_->die(this);
}

void			Client::clearCommandAndParam()
{
	command_.clear();
	param_.clear();
}

std::string&	Client::getSendBuf(void)
{
	return send_buf_;
}

std::string&	Client::getRecvBuf(void)
{
	return recv_buf_;
}
