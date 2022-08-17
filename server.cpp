/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:28 by alee              #+#    #+#             */
/*   Updated: 2022/08/17 16:19:09 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <arpa/inet.h>
#include <cctype>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

Server::Server(int argc, char *argv[])
	: status_(true), sock_count(0)
{
	//argument check (port, pwd)
	if (argc != 3)
	{
		std::cerr << "Error : Invalid Argument" << std::endl;
		std::cerr << "./ircserv <port> <server_pwd>" << std::endl;
		status_ = false;
		return ;
	}
	//verify port, config port
	if (config_port(argv[1]) == false)
	{
		std::cerr << "Error : Invalid Port" << std::endl;
		status_ = false;
		return ;
	}
	//verify pwd, config pwd
	if (config_pwd(argv[2]) == false)
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
	network_close();
	return ;
}

bool	Server::config_port(std::string port)
{
	int	retPort;
	if (is_valid_port(port) == false)
		return (false);
	if (getPortNumber(port.c_str(), &retPort) == false)
		return (false);
	s_port_ = static_cast<unsigned short>(retPort);
	return (true);
}

bool	Server::is_valid_port(std::string port)
{
	return (port.find_first_not_of("0123456789") == std::string::npos);
}

t_port	Server::getPortType(int value)
{
	if (value >= 0 && value < 1024)
		return (WELL_KNOWN_PORT);
	else if (value < 49152)
		return (REGISTERED_PORT);
	else if (value < 65536)
		return (DYNAMIC_PORT);
	return (INVALID_PORT);
}

bool	Server::getPortNumber(const char *str, int *o_value)
{
	long long total = 0;

	while (*str)
	{
		total = total * 10 + *str - '0';
		if (total > 2147483647 || total < -2147483648)
			return (false);
		str++;
	}
	t_port	retPort = getPortType(total);
	if (retPort == WELL_KNOWN_PORT || retPort == INVALID_PORT)
		return (false);
	*o_value = total;
	return (true);
}

bool	Server::config_pwd(std::string pwd)
{
	if (is_valid_pwd(pwd) == false)
		return (false);
	raw_pwd_ = pwd;
	return (true);
}

bool	isSpecialCharactor(std::string str)
{
	size_t i = 0;
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
	listen_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock_ == -1) {
		#ifdef DEBUG
		std::cout << "socket() failed\n";
		#endif
		status_ = false;
		return ;
	}

	// set listen_sock_ O_NONBLOCK
	fcntl(listen_sock_, F_SETFL, O_NONBLOCK);

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
	if (retval == -1)
	{
		#ifdef DEBUG
		std::cout << "setsockopt() for TCP_NODELAY failed\n";
		#endif
		status_ = false;
		return ;
	}

	// bind
	memset(&s_addr_in_, 0x00, sizeof(s_addr_in_));
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

	//client count
	sock_count += 1;

	std::cout << "IRC Server started" << std::endl;
	return ;
}

void	Server::network_close(void)
{
	close(listen_sock_);
	return ;
}

int	Server::getMaxFD(SOCKET sock)
{
	int	max_fd = sock;
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end(); iter++)
	{
		if (max_fd < iter->first)
			max_fd = iter->first;
	}
	return (max_fd);
}

void	Server::accept_client(SOCKET listen_sock)
{
	SOCKET				client_sock;
	struct sockaddr_in	c_addr_in;
	socklen_t			c_addr_len = sizeof(c_addr_in);
	memset(&c_addr_in, 0x00, sizeof(c_addr_in));
	//accept(...)
	client_sock = accept(listen_sock, reinterpret_cast<sockaddr *>(&c_addr_in), &c_addr_len);
	if (client_sock == -1)
		return ;

	//select에서 처리할 수 있는 최대 set의 개수를 넘어서는 경우 접속을 끊는다.
	if (sock_count >= FD_SETSIZE)
	{
		close(client_sock);
		return ;
	}

	// set client_sock O_NONBLOCK
	fcntl(client_sock, F_SETFL, O_NONBLOCK);

	//TODO : 클라이언트 세션 생성 및 데이터 초기화
	Client*	new_client = new Client(client_sock);

	//push client socket
	// client_map_.push_back(new_client);
	client_map_.insert(std::make_pair(client_sock, new_client));

	//client count
	sock_count += 1;

	//display client network info
	std::cout << "-------------------" << std::endl;
	std::cout << "client connected" << std::endl;
	std::cout << "client socket : " << client_sock << std::endl;
	std::cout << "client port   : " << ntohs(c_addr_in.sin_port) << std::endl;
	std::cout << "client ip     : " << inet_ntoa(c_addr_in.sin_addr) << std::endl;
	std::cout << "-------------------" << std::endl;
	return ;
}

void	Server::recv_client(std::map<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];
	int	recv_ret = recv(iter->first, (void *)buf, sizeof(buf), 0);
	// int	recv_ret = recv(iter->first, const_cast<char *>(iter->second->getRecvBuf().c_str()), iter->second->getRecvBuf().npos, 0);
	//disconnect
	if (recv_ret == 0)
	{
		std::cout << iter->first << " : Disconnected" << std::endl;
		close(iter->first);
		iter = client_map_.erase(iter);
		delete iter->second;
		sock_count -= 1;
	}
	else if (recv_ret > 0)
	{
		iter->second->getRecvBuf().append(reinterpret_cast<char *>(buf));
		std::cout << recv_ret << "byte recv" << std::endl;
		std::cout << "msg : " << '[' << buf << ']' << std::endl;
	}
	return ;
}

void	Server::send_client(Client& client_session)
{
	(void)client_session;
	return ;
}


bool	Server::getStatus(void)
{
	return (this->status_);
}

void	Server::Run(void)
{
	//FD ZERO
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);

	//listen socket add(read_set)
	FD_SET(listen_sock_, &read_set);

	//client socket add(read_set, write_set)
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end(); iter++)
	{
		FD_SET(iter->first, &read_set);
		// FD_SET(iter->first, &write_set);
	}

	//set timeout
	struct timeval	time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

	//select
	int	select_result = select(getMaxFD(listen_sock_) + 1, &read_set, &write_set, NULL, &time_out);
	if (select_result > 0)
	{
		//new client
		if (FD_ISSET(listen_sock_, &read_set))
			accept_client(listen_sock_);
		//old client
		for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end();)
		{
			if (FD_ISSET(iter->first, &read_set))
				recv_client(iter);
			else
				iter++;
		}
	}
	//클라이언트에 대한 마샬링 -> 패킷 커맨드에 대해서 분석 후 해당하는 로직을 처리한다.
	//packet_marsharing(...)
	return ;
}
