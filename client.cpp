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

#include "client.hpp"
#include "channel.hpp" // consider to remove

#include "Command.hpp"

Client::Client(SOCKET s, std::string host_name, Server* sv)
	: client_sock_(s)
	, host_name_(host_name)
	, disconnect_flag_(false)
	, pass_flag_(false)
	, nick_flag_(false)
	, user_flag_(false)
	, user_real_name_flag_(false)
	, operator_flag_(false)
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
	std::string	command;
	std::string	param;

	marshalMessage(command, param);
	processProtocol(command, param);
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

void	Client::processProtocol(std::string& command, std::string& param)
{
	//isWelcomed
	if (isWelcomed() == false)
		processToWelcome(command, param);
	else
		processCommand(command, param);
}


bool	Client::isWelcomed() const
{
	return (welcomed_);
}

void	Client::processToWelcome(std::string& command, std::string& param)
{
	Command	cmd(sv_);

	if (getPassFlag() == false)
	{
		cmd.pass(this);
	}
	else if (getNickFlag() == false || getUserNameFlag() == false)
	{
		// nick
		// user
	}
	if (getPassFlag() && getNickFlag() && getUserNameFlag())
	{
		// send: welcomeProtocol
	}
}

void	Client::processCommand(std::string& command, std::string& param)
{
	std::cout << "in processCommand() ^o^\n";
	(void)command;
	(void)param;
}
