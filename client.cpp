/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:01:59 by alee              #+#    #+#             */
/*   Updated: 2022/08/17 02:09:02 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(SOCKET s)
{
	client_sock_ = s;
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
