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
#include <cctype>
#include <fcntl.h>
#include <netinet/tcp.h> // TCP_NODELAY

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

bool	isSpecialCharactor(std::string str)
{
	int i = 0;
#ifdef DEBUG
	std::cout << str << '\n';
	std::cout << "size: " << str.size() << '\n';
#endif
	while (i < str.size())
	{
		if (!std::isalnum(str[i]))
			return (true);
#ifdef DEBUG
		else
			std::cout << str[i] << "is number or alpha\n";
#endif
		++i;
	}
	return (false);
}

bool	Server::is_valid_pwd(std::string pwd)
{
	//TODO : 특수문자가 패스워드로 등록이 불가능 하도록
	if (pwd.length() == 0)
		return (false);
	if (isSpecialCharactor(pwd))
		return (false);
	return (true);
}

void	Server::network_init(void)
{
	int retval;

	// socket
	listen_sock_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock_ == -1) {
		#ifdef DEBUG
		std::cout << "socket() failed\n";
		#endif
		status_ = false;
		return ;
	}

	// set listen_sock_ O_NONBLOCK
	int flags = fcntl(listen_sock_, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(listen_sock_, F_SETFL, flags);

	// set listen_sock_ SO_REUSEADDR
	//struct timeval optval = {0, 1000};
	int optval = 1;
	retval = setsockopt(listen_sock_, SOL_SOCKET, SO_REUSEADDR, 
		&optval, sizeof(optval));
	if (retval == -1) {
		#ifdef DEBUG
		std::cout << "setsockopt() for SO_REUSADDR failed\n";
		#endif
		status_ = false;
		return ;
	}

	// unset Nagle algorithm
	//int optval = 1;
	retval = setsockopt(listen_sock_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
	if (retval == -1) {
		#ifdef DEBUG
		std::cout << "setsockopt() for TCP_NODELAY failed\n";
		#endif
		status_ = false;
		return ;
	}

	// bind
	memset(&s_addr_in_, 0, sizeof(s_addr_in_));
	s_addr_in_.sin_family = AF_INET;
	s_addr_in_.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in_.sin_port = htons(s_port_); // SERVERPORT
	retval = bind(listen_sock_, reinterpret_cast<struct sockaddr *>(&s_addr_in_), sizeof(s_addr_in_));
	if (retval == -1) {
		#ifdef DEBUG
		std::cout << "bind() failed\n";
		#endif
		status_ = false;
		return ;
	}

	// listen
	retval = listen(listen_sock_, SOMAXCONN);
	if (retval == -1) {
		#ifdef DEBUG
		std::cout << "listen() failed\n";
		#endif
		status_ = false;
		return ;
	}

	std::cout << "Server started\n";
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