/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:01:59 by alee              #+#    #+#             */
/*   Updated: 2022/08/18 09:44:59 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(SOCKET s)
{
	client_sock_ = s;
	pass_flag = 0;
	nick_flag = 0;
	user_flag = 0;
	operator_flag = 0;
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

bool	Client::isSetPass(void) const
{
	return (this->pass_flag);
}

bool	Client::isSetNick(void) const
{
	return (this->nick_flag);
}

bool	Client::isSetUserName(void) const
{
	return (this->user_flag);
}

bool	Client::isSetOperator(void) const
{
	return (this->operator_flag);
}
