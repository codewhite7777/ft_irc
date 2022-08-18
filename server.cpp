/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:28 by alee              #+#    #+#             */
/*   Updated: 2022/08/19 05:08:32 by alee             ###   ########.fr       */
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
#include <cstring>

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
	if (configPort(argv[1]) == false)
	{
		std::cerr << "Error : Invalid Port" << std::endl;
		status_ = false;
		return ;
	}
	//verify pwd, config pwd
	if (configPwd(argv[2]) == false)
	{
		std::cerr << "Error : Invalid password" << std::endl;
		status_ = false;
		return ;
	}

	//set operator pwd
	s_operator_pwd_ = "admin";

	//network init
	networkInit();
	return ;
}

Server::~Server(void)
{
	networkClose();
	return ;
}

bool	Server::configPort(std::string port)
{
	int	retPort;
	if (isValidPort(port) == false)
		return (false);
	if (getPortNumber(port.c_str(), &retPort) == false)
		return (false);
	s_port_ = static_cast<unsigned short>(retPort);
	return (true);
}

bool	Server::isValidPort(std::string port)
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

bool	Server::configPwd(std::string pwd)
{
	if (isValidPwd(pwd) == false)
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

bool	Server::isValidPwd(std::string pwd)
{
	if (pwd.length() == 0)
		return (false);
	if (isSpecialCharactor(pwd))
		return (false);
	return (true);
}

void	Server::networkInit(void)
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

void	Server::networkClose(void)
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

void	Server::networkProcess(void)
{
	//FD ZERO
	FD_ZERO(&read_set_);
	FD_ZERO(&write_set_);

	//listen socket add(read_set)
	FD_SET(listen_sock_, &read_set_);

	//client socket add(read_set, write_set)
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end(); iter++)
	{
		FD_SET(iter->first, &read_set_);
		FD_SET(iter->first, &write_set_);
	}

	//set timeout
	struct timeval	time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

	//select
	int	select_result = select(getMaxFD(listen_sock_) + 1, &read_set_, &write_set_, NULL, &time_out);
	if (select_result > 0)
	{
		//new client
		if (FD_ISSET(listen_sock_, &read_set_))
		{
			acceptClient(listen_sock_);
			if (select_result == 1)
				return ;
		}
		//old client
		for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end(); iter++)
		{
			if (FD_ISSET(iter->first, &read_set_))
				recvPacket(iter);
			if ((iter->second->getDisconnectFlag() == false) \
			&& FD_ISSET(iter->first, &write_set_) \
			&& iter->second->getSendBuf().length() > 0)
				sendPacket(iter);
		}
	}
	return ;
}

void	Server::acceptClient(SOCKET listen_sock)
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

void	Server::recvPacket(std::map<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];
	int	recv_ret = recv(iter->first, reinterpret_cast<void *>(buf), sizeof(buf), 0);
	//disconnect
	if (recv_ret == 0)
		iter->second->setDisconnectFlag(true);
	else if (recv_ret > 0)
	{
		buf[recv_ret] = '\0';
		iter->second->getRecvBuf().append(reinterpret_cast<char *>(buf));
		std::cout << "client : " << iter->first << "\n" << recv_ret << "Byte Msg Recv" << std::endl;
		std::cout << "Msg : " << '[' << buf << ']' << std::endl;
		std::cout << "current size : " << iter->second->getRecvBuf().length() << std::endl;
		for(int i=0; i<recv_ret; i++)
			std::cout << '[' << (int)buf[i] << ']' << ',' << '[' << buf[i] << ']' << std::endl;
	}
	return ;
}

void	Server::sendPacket(std::map<SOCKET, Client *>::iterator &iter)
{
	// std::cout << "sendPacket Called " << std::endl;
	// std::cout << "sendPacket : " << '[' << iter->second->getSendBuf() << ']' << std::endl;
	unsigned char	buf[BUFFER_MAX];
	memcpy(buf, iter->second->getSendBuf().c_str(), iter->second->getSendBuf().length() + 1);
	// buf[iter->second->getSendBuf().length()] = '\0';
	// std::cout << "sendPacket buf : " << '[' << buf << ']' << std::endl;
	int	send_ret = send(iter->first, reinterpret_cast<void *>(buf), strlen(reinterpret_cast<char *>(buf)), 0);
	if (send_ret == -1)
	{
		iter->second->setDisconnectFlag(true);
		return ;
	}
	iter->second->getSendBuf().erase(0, send_ret);
	return ;
}


void	Server::packetMarshalling(void)
{
	for (std::map<SOCKET, Client*>::iterator iter = client_map_.begin(); iter != client_map_.end(); iter++)
	{
		if (iter->second->getRecvBuf().length() != 0)
		{
			packetAnalysis(iter);
			iter->second->getRecvBuf().clear();
		}
	}
	return ;
}

void Server::insertSendBuffer(Client* target_client, const std::string& msg)
{
	target_client->getSendBuf().append(msg);
	return ;
}

void	Server::packetAnalysis(std::map<SOCKET, Client *>::iterator& iter)
{
	std::string	packet_buf = iter->second->getRecvBuf();
	std::string	command;
	std::string	param;

	if (packet_buf.find('\n') != std::string::npos && packet_buf.at(packet_buf.length() - 1) == '\n')
	{
		packet_buf.erase(packet_buf.begin() + packet_buf.find('\n'));
	}
	if (packet_buf.find(' ') != std::string::npos)
	{
		command = packet_buf.substr(0, packet_buf.find(' '));
		param = packet_buf.substr(packet_buf.find(' ') + 1);
	}
	// std::cout << "command : " << '<' << command << '>' << std::endl;
	// std::cout << "param : " << '<' << param << '>' << std::endl;
	//PASS에 대한 처리
	if (iter->second->getPassFlag() == false)
		requestAuth(iter, command, param);
	// if (iter->second->getNickFlag() == false)
		// requestSetNickName(iter, command, param);
	//NICK에 대한 처리

	//USER_NAMe에 대한 처리
	return ;
}

void	Server::requestAuth(std::map<SOCKET, Client*>::iterator &iter, std::string& command, std::string& param)
{
	if (command != "PASS")
	{
		insertSendBuffer(iter->second, "ex) <PASS> <password>\n");
		return ;
	}
	if (this->raw_pwd_ == param)
		iter->second->setPassFlag(true);
	else
		insertSendBuffer(iter->second, "Wrong password.\n");
	return ;
}

void	Server::clientDisconnect(void)
{
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end();)
	{
		if (iter->second->getDisconnectFlag() == true)
		{
			std::cout << iter->first << " Socket Disconnected" << std::endl;
			close(iter->first);
			delete iter->second;
			iter = client_map_.erase(iter);
			sock_count -= 1;
		}
		else
			iter++;
	}
	return ;
}

bool	Server::getStatus(void)
{
	return (this->status_);
}

void	Server::Run(void)
{
	networkProcess();
	packetMarshalling();
	clientDisconnect();
	return ;
}
