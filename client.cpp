/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:01:59 by alee              #+#    #+#             */
/*   Updated: 2022/08/28 11:50:38 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(SOCKET s, std::string host_name)
{
	client_sock_ = s;
	disconnect_flag_ = 0;
	pass_flag_ = 0;
	nick_flag_ = 0;
	user_flag_ = 0;
	operator_flag_ = 0;
	user_real_name_flag_ = 0;
	host_name_ = host_name;
	return ;
}

Client::~Client(void)
{
	return ;
}

SOCKET&	Client::getSocket(void)
{
	return (this->client_sock_);
}

std::string&	Client::getSendBuf(void)
{
	return (this->s_buf_);
}

std::string&	Client::getRecvBuf(void)
{
	return (this->r_buf_);
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

bool	Client::getOperatorFlag(void) const
{
	return (this->operator_flag_);
}

std::vector<std::string>& Client::getChannelList()
{
	return channelList;
}