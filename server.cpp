/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:28 by alee              #+#    #+#             */
/*   Updated: 2022/08/15 13:19:49 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(int argc, char *argv[])
	: status(true)
{
	if (argc != 3)
	{
		std::cerr << "Error : Invalid Argument" << std::endl;
		status = false;
		return ;
	}
	if (is_vaild_port(argv[1]))
	{
		std::cerr << "Error : Invalid Port" << std::endl;

	}


ERR:

	return ;
}

Server::~Server(void)
{
	return ;
}


bool	Server::is_vaild_port(std::string port)
{

	
}

bool	Server::getStatus(void)
{
	return (this->status);
}
