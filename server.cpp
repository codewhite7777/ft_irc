/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:28 by alee              #+#    #+#             */
/*   Updated: 2022/08/15 15:12:20 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <arpa/inet.h>

Server::Server(int argc, char *argv[])
	: status_(true)
{
	//argument check (port, pwd)
	if (argc != 3)
	{
		std::cerr << "Error : Invalid Argument" << std::endl;
		status_ = false;
		return ;
	}
	//verify port, config port
	if (is_valid_port(argv[1]) == false)
	{
		std::cerr << "Error : Invalid Port" << std::endl;
		status_ = false;
		return ;
	}
	//verify pwd, config pwd
	if (is_valid_pwd(argv[2]) == false)
	{
		std::cerr << "Error : Invalid password" << std::endl;
		status_ = false;
		return ;
	}
	network_init();
	return ;
}

Server::~Server(void)
{
	return ;
}


bool	Server::is_valid_port(std::string port)
{
	return (port.find_first_not_of("0123456789") == std::string::npos);
}

bool	Server::is_valid_pwd(std::string pwd)
{
	//TODO : 특수문자가 패스워드로 등록이 불가능 하도록
	if (pwd.length() == 0)
		return (false);
	return (true);
}

void	Server::network_init(void)
{
	return ;
}

bool	Server::getStatus(void)
{
	return (this->status_);
}

void	Server::Run(void)
{
	return ;
}
