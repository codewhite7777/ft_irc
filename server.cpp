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

#define WELL_KNOWN	0
#define REGISTERED	1
#define DYNAMIC		2
#define ERROR		3
int getPortType(int value)
{
	if (value >= 0 && value < 1024)
		return (WELL_KNOWN);
	else if (value < 49152)
		return (REGISTERED);
	else if (value < 65536) // unsigned short면 왜 에러를??
		return (DYNAMIC);
	return (ERROR);
}

bool getPortNumber(const char *str, int *value)
{
	long long total = 0;

	while (*str)
	{
		total = total * 10 + *str - '0';
		#ifdef DEBUG
		std::cout << "total:" << total << '\n';
		#endif
		// if (total > 0xcffffff || total < 0xffffffff)
		if (total > 2147483647 || total < -2147483648)
			return (false);
		str++;
	}

	// getType: WELL_KNOWN | REGISTERED | DYNAMIC | ERROR
	if (getPortType(total) != REGISTERED)
		return (false);
	*value = total;
	return (true);
}

// true: valid port | false: not valid port
bool	Server::is_valid_port(std::string port)
{
	bool	flag;
	int		retPort;

	flag = ( port.find_first_not_of("0123456789") == std::string::npos );

	#ifdef DEBUG
	std::cout << "find_first_not_of start\n";
	std::cout << "Flag Value: " << flag << '\n';
	std::cout << "----------END----------\n\n";
	#endif

	if (!flag)
		return (flag);
	
	#ifdef DEBUG
	std::cout << "getPortNumber start\n";
	#endif

	flag = getPortNumber(port.c_str(), &retPort);
	if (!flag)
		return (flag);
	
	#ifdef DEBUG
	std::cout << "----------END----------\n\n";
	#endif

	s_port_ = static_cast<unsigned short>(retPort);

	#ifdef DEBUG
	std::cout << "real portnum: " << s_port_ << '\n';
	#endif

	return (flag);
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