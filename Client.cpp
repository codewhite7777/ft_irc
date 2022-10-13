/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:19:48 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:19:50 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "Protocol.hpp"
#include <string>
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

size_t		Client::getRecvBufLength(void)
{
	return (getRecvBuf().length());
}

void		Client::appendToSendBuf(const std::string& str)
{
	getSendBuf().append(str);
}

const char*	Client::getSendBufCStr(void)
{
	return (getSendBuf().c_str());
}

size_t		Client::getSendBufLength(void)
{
	return (getSendBuf().length());
}

void		Client::eraseSendBufSize(int size)
{
	getSendBuf().erase(0, size);
}

void	Client::processMessageInRecvBuf(void)
{
	marshalMessageToCmdAndParam();
	processProtocol();
	clearCommandAndParam();
}

const std::string&	Client::getCommand(void) const
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

const std::string&	Client::getNickname(void) const
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

const std::string&	Client::getUsername(void) const
{
	return username_;
}

const std::string&	Client::getHostname(void) const
{
	return hostname_;
}

const std::string&	Client::getRealname(void) const
{
	return realname_;
}

const std::string	Client::getUserRealHostNamesInfo(void) const
{
	return (nickname_ + "!" + username_ + "@" + hostname_);
}

void				Client::setUserFlagOn(void)
{
	user_flag_ = true;
}

bool				Client::getUserFlag(void) const
{
	return user_flag_;
}

std::string			Client::getNamesPrefix(void) const
{
	return (":" + nickname_ + "!" + username_ + "@" + hostname_ + " ");
}

void				Client::setSvOperFlagOn(void)
{
	this->sv_oper_flag_ = true;
}

bool				Client::isSvOper(void) const
{
	return (this->sv_oper_flag_);
}

void			Client::promptRecvedMsg(void)
{
	std::cout << "Received message from <SD: " << this->getSocket();
	std::cout << " | nickname: " << this->getNickname() << ">\n" \
				<< "[" << this->getRecvBuf() << "]\n";
}

void			Client::promptSendedMsg(void)
{
	std::cout << "Sended message to <SD: " << this->getSocket();
	std::cout << " | nickname: " << this->getNickname() << ">\n"\
			<< "[" << this->getSendBuf() << "]\n\n";
}

void	Client::marshalMessageToCmdAndParam(void)
{
	std::string	tmp_msg;

	tmp_msg = extractFirstMsg(getRecvBuf());
	if (tmp_msg.find(' ') != std::string::npos)
	{
		command_ = tmp_msg.substr(0, tmp_msg.find(' '));
		param_ = tmp_msg.substr(tmp_msg.find(' ') + 1);
	}
	else
	{
		command_ = tmp_msg;
		param_ = "";
	}
	promptCommandAndParam();
}

void		Client::promptCommandAndParam(void)
{
	std::cout << "\tCommand: [" << command_ << "]\n";
	std::cout << "\tParameter: [" << param_ << "]\n\n";
}

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
	if (command_ == "PING")
		cmd_->ping(this);
	else if (isWelcomed() == false)
		processAuthToWelcome();
	else
		processCommand();
}

void	Client::processAuthToWelcome(void)
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
		appendToSendBuf(proto_->svPrivmsgClntWhenInit(this));
	}
}

bool	Client::isPassed(void) const
{
	return passed_;
}

bool	Client::isWelcomed(void) const
{
	return welcomed_;
}

void	Client::processCommand(void)
{
	if (command_ == "JOIN")
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
	else if (command_ == "die")
		cmd_->die(this);
}

void			Client::clearCommandAndParam(void)
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
