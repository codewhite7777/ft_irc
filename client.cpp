/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:01:59 by alee              #+#    #+#             */
/*   Updated: 2022/09/12 17:23:08 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp" // consider to remove

#include "Command.hpp"

Client::Client(SOCKET s, std::string host_name, Server* sv)
	: client_sock_(s)
	, host_name_(host_name)
	, disconnect_flag_(false)
	, pass_flag_(false)
	, nick_flag_(false)
	, user_flag_(false)
	, user_real_name_flag_(false)
	//, operator_flag_(false)
	, passed_(false)
	, welcomed_(false)
	, sv_(sv) {}

Client::~Client(void) {}

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

void		Client::appendToSendBuf(unsigned char* buf)
{
	getSendBuf().append(reinterpret_cast<char *>(buf));
}

const char*	Client::getSendBufCharStr()
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


SOCKET&	Client::getSocket(void)
{
	return (this->client_sock_);
}

std::string&	Client::getSendBuf(void)
{
	return (s_buf_);
}

std::string&	Client::getRecvBuf(void)
{
	return (r_buf_);
}

bool	Client::getDisconnectFlag(void) const
{
	return (this->disconnect_flag_);
}

void	Client::setDisconnectFlag(bool flag)
{
	this->disconnect_flag_ = flag;
	return ;
}

void	Client::setPassFlag(bool flag)
{
	this->pass_flag_ = flag;
	return ;
}

bool	Client::getPassFlag(void) const
{
	return (this->pass_flag_);
}

bool	Client::getNickFlag(void) const
{
	return (this->nick_flag_);
}

void	Client::setNickName(bool flag, std::string& nickname)
{
	this->nick_flag_ = flag;
	this->nick_name_ = nickname;
	return ;
}

std::string&	Client::getNickName(void)
{
	return (this->nick_name_);
}

bool	Client::getUserNameFlag(void) const
{
	return (this->user_flag_);
}


void	Client::setUserName(bool flag, std::string &username)
{
	this->user_flag_ = flag;
	this->user_name_ = username;
	return ;
}

std::string&	Client::getUserName(void)
{
	return (this->user_name_);
}

void	Client::setMode(std::string &mode)
{
	this->mode_ = mode;
	return ;
}

void	Client::setUnused(std::string &unused)
{
	this->unused_ = unused;
	return ;
}

bool	Client::getUserRealNameFlag(void) const
{
	return (this->user_real_name_flag_);
}

void	Client::setUserRealName(bool flag, std::string &realname)
{
	this->user_real_name_flag_ = flag;
	this->user_real_name_ = realname;
	return ;
}

std::string&	Client::getUserRealName(void)
{
	return (this->user_real_name_);
}

std::string&	Client::getHostName(void)
{
	return (this->host_name_);
}


/*
	take first protocol from packet

	1) [one protocol] If no more behind \r\n, just return pure packet.
	2) [more than one protocol] If something behind \r\n, split packet by first \r\n and return only first protocol.
	3) [no protocol] If no \r\n, return empty.
*/
std::string	extractFirstMsg(std::string& recv_buf)
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


void	Client::processMessageInBuf()
{
	marshalMessage(command_, param_);
	processProtocol();
	//clearCommandAndParam();
}

void	Client::marshalMessage(std::string& command, std::string& param)
{
	std::string	tmp;

	tmp = extractFirstMsg(getRecvBuf());
	if (tmp.find(' ') != std::string::npos)
	{
		command = tmp.substr(0, tmp.find(' '));
		param = tmp.substr(tmp.find(' ') + 1);
	}
	else
	{
		command = "";
		param = tmp;
	}

	// test: print command and param
	{
		std::cout << "command: [" << command << "], ";
		std::cout << "param: [" << param << "] ";
		std::cout << "in marshalMessage()\n";
	}
}

void	Client::processProtocol(void)
{
	if (isWelcomed() == false)
		processToWelcome();
	else
		processCommand();
}

bool	Client::isPassed() const
{
	return passed_;
}

bool	Client::isWelcomed() const
{
	return welcomed_;
}

void	Client::processToWelcome()
{
	Command		cmd(sv_);
	Protocol	proto(sv_);
	// 얘들을 main에서 만들고 주소를 Client의 상태로 설정하면 더 효율적일 것 같다.

	if (getPassFlag() == false)
	{
		if (command_ == "CAP")
			return ;
		else if (command_ == "PASS")
			cmd.pass(this);
		else
			appendToSendBuf(proto.errNotPassCmd());
	}
	else if (getNickFlag() == false || getUserFlag() == false)
	{
		if (command_ == "NICK")
		{
			cmd.nick(this);
		}
		else if (command_ == "USER")
		{
			cmd.user(this);
		}
	}
	if (getPassFlag() && getNickFlag() && getUserFlag())
	{
		welcomed_ = true;

		// send: Protocols when Welcome
		// 001
		appendToSendBuf(proto.rplWelcome(this));
		// 002
		appendToSendBuf(proto.rplYourHost(this));
		// 003
		//appendToSendBuf(proto.rplCreated(this));
		// 004
		//appendToSendBuf(proto.rplMyInfo(this));
	}
}

void	Client::processCommand()
{
	std::cout << "in processCommand() ^o^\n";
}

std::string&	Client::getCommand(void)
{
	return command_;
}

std::string&	Client::getParam(void)
{
	return param_;
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

void			Client::setUsername(std::string username)
{
	username_ = username;
}

const std::string&	Client::getUsername() const
{
	return username_;
}

void			Client::setHostname(std::string hostname)
{
	hostname_ = hostname;
}

const std::string&	Client::getHostname() const
{
	return hostname_;
}

void			Client::setRealname(std::string realname)
{
	realname_ = realname;
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
