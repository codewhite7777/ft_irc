/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:28 by alee              #+#    #+#             */
/*   Updated: 2022/08/29 17:05:24 by alee             ###   ########.fr       */
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
#include "irc_protocol.hpp"
#include <sstream>
#include <vector>

Server::Server(int argc, char *argv[])
	: status_(true), sock_count_(0)
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
	sock_count_ += 1;

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

	//select(...)
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
		//current client
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

	//if current client's counts are over select function's set socket max counts, accepted socket is closed.
	if (sock_count_ >= FD_SETSIZE)
	{
		close(client_sock);
		return ;
	}

	//set client_sock O_NONBLOCK
	fcntl(client_sock, F_SETFL, O_NONBLOCK);

	//create client session
	Client*	new_client = new Client(client_sock, inet_ntoa(c_addr_in.sin_addr));

	//push client socket
	client_map_.insert(std::make_pair(client_sock, new_client));

	//client count
	sock_count_ += 1;

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
		// std::cout << "client : " << iter->first << "\n" << recv_ret << "Byte Msg Recv" << std::endl;
		// std::cout << "Msg : " << '[' << buf << ']' << std::endl;
		// std::cout << "current size : " << iter->second->getRecvBuf().length() << std::endl;
		// for(int i=0; i<recv_ret; i++)
			// std::cout << '[' << (int)buf[i] << ']' << ',' << '[' << buf[i] << ']' << std::endl;
	}
	return ;
}

void	Server::sendPacket(std::map<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];
	memcpy(buf, iter->second->getSendBuf().c_str(), iter->second->getSendBuf().length() + 1);
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
			// iter->second->getRecvBuf().clear();
			// iter->second->getRecvBuf().erase(0, iter->second->getWritePos());
		}
	}
	return ;
}

void Server::insertSendBuffer(Client* target_client, const std::string& msg)
{
	target_client->getSendBuf().append(msg);
	return ;
}

std::string	Server::buildErrPacket(std::string err_code, std::string user_name, std::string err_msg)
{
	std::string	packet;
	packet = err_code + " " + user_name + " " + err_msg;
	return (packet);
}

std::string	Server::buildReplyPacket(std::string reply_code, std::string user_name, std::string reply_msg)
{
	std::string	packet;
	packet = reply_code + " " + user_name + " " + reply_msg;
	return (packet);
}

std::string	Server::getUserInfo(std::string nickname, std::string username, std::string hostname)
{
	std::string	msg = ":" + nickname + "!" + username + "@" + hostname;
	return (msg);
}

std::string	Server::packetTrim(std::string& packet)
{
	std::string	packet_buf = packet;

	//erase newline
	if (packet_buf.find('\n') != std::string::npos && packet_buf.at(packet_buf.length() - 1) == '\n')
	{
		packet_buf.erase(packet_buf.begin() + packet_buf.find('\n'));
		//erase carrige return
		if (packet_buf.find('\r') != std::string::npos && packet_buf.length() > 1)
			packet_buf.erase(packet_buf.begin() + packet_buf.find('\r'));
	}
	return (packet_buf);
}

bool	Server::packetTokenize(std::string& packet_buf, std::string& o_command, std::string& o_param, \
						size_t&	o_carridge_pos, const char *str)
{
	bool	ret;
	size_t	carridge_tok;

	ret = true;
	carridge_tok = packet_buf.find(str);
	if (carridge_tok != std::string::npos)
	{
		size_t	space_tok = packet_buf.find(' ', carridge_tok);
		if (space_tok == std::string::npos)
		{
			o_carridge_pos = carridge_tok + strlen(str);
			ret = false;
		}
		else
		{
			o_command = packet_buf.substr(0, space_tok);
			o_param = packet_buf.substr(space_tok + 1);
			std::cout << "packetToke(...) : " << '[' << o_command << ']' << std::endl;
			std::cout << "packetToke(...) : " << '[' << o_param << ']' << std::endl;
			o_carridge_pos = carridge_tok + strlen(str);
			ret = false;
		}
	}
	else
	{
		o_carridge_pos = packet_buf.length();
		ret = false;
	}
	return (ret);
}

void	Server::packetAnalysis(std::map<SOCKET, Client *>::iterator& iter)
{
	std::string	packet_buf;
	std::string	command;
	std::string	param;
	size_t		write_pos;

	// packet_buf = packetTrim(iter->second->getRecvBuf());
	packet_buf = iter->second->getRecvBuf();
	if (packetTokenize(packet_buf, command, param, write_pos, "\r\n") == false)
		iter->second->getRecvBuf().erase(write_pos);

	// if (packet_buf.find(' ') != std::string::npos)
	// {
	// 	command = packet_buf.substr(0, packet_buf.find(' '));
	// 	param = packet_buf.substr(packet_buf.find(' ') + 1);
	// }
	// else
	// {
	// 	command = "";
	// 	param = packet_buf;
	// }

	std::cout << "packet : " << "[" << packet_buf << "]" << std::endl;
	std::cout << "command : " << "[" << command << "]" << std::endl;
	std::cout << "param : " << "[" << param << "]" << std::endl;
	if (iter->second->getPassFlag() == false)
		requestAuth(iter, command, param);
	else if (iter->second->getNickFlag() == false)
		requestSetNickName(iter, command, param);
	else if (iter->second->getUserNameFlag() == false)
		requestSetUserName(iter, command, param);
	else
		requestCommand(iter, command, param);
	return ;
}

void	Server::requestAuth(std::map<SOCKET, Client*>::iterator &iter, \
							std::string& command, std::string& param)
{
	if (command == "CAP")
	{
		size_t	pos = param.find("PASS");
		if (pos == std::string::npos)
		{

			return ;
		}
		param.erase(0, pos);//command + param = packet_buf만큼 write_pos를 이동시킨다.
		// packetTokenize(param, command, param, ' ');
	}
	if (command != "PASS")
	{
		//command + param = packet_buf만큼 write_pos를 이동시킨다.
		// iter->second->setWritePos(total_length);
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":ex) <PASS> <password>\r\n"));
		return ;
	}
	if (this->raw_pwd_ == param)
	{
		iter->second->setPassFlag(true);
		insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", ":info) Successful Authentication.\r\n"));
		// iter->second->setWritePos(total_length);
		std::cout << "-----------------------------------" << std::endl;
		std::cout << iter->first << " Client Successful Authentication." << std::endl;
		std::cout << "-----------------------------------" << std::endl;
	}
	else
	{
		//PASS & NICK 같이 오는 케이스
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":info> Wrong password\r\n"));
	}
	return ;
}

void	Server::requestSetNickName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	//커맨드 확인
	if (command != "NICK")
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":ex) <NICK> <nickname>\r\n"));
		return ;
	}
	//닉네임 중복 확인
	if (isOverlapNickName(param) == true)
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_NICKNAMEINUSE, "UNKNOWN", "info) This nickname is already taken.\r\n"));
		return ;
	}
	//유효하지 않은 닉네임
	if (param == "\"\"")
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_NONICKNAMEGIVEN, "UNKNOWN", ":info) No nickname given\r\n"));
		return ;
	}

	iter->second->setNickName(true, param);
	insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", "info) Successful nickname.\r\n"));
	insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", "info) Nick Name : " + iter->second->getNickName() + "\r\n"));
	std::cout << "-----------------------------------" << std::endl;
	std::cout << iter->first << " Client Successful Set NickName" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	return ;
}

bool	Server::isOverlapNickName(std::string& search_nick)
{
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end(); iter++)
	{
		if (iter->second->getNickName() == search_nick)
			return (true);
	}
	return (false);
}

std::vector<std::string> split(std::string str, char delimiter)
{
	std::vector<std::string>	internal;
	std::stringstream			ss(str);
	std::string					temp;

	while (getline(ss, temp, delimiter))
		internal.push_back(temp);
	return (internal);
}

void	Server::requestSetUserName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::cout << "requestSetUserName called " << std::endl;
	if (command != "USER")
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":ex) <USER> <user> <mode> <unused> <realname>\r\n"));
		return ;
	}
	else
	{
		std::vector<std::string> splitVector = split(param, ' ');
		if (splitVector.size() != 4)
		{
			insertSendBuffer(iter->second, buildErrPacket(ERR_NEEDMOREPARAMS, "UNKNOWN", ":info) Not enough parameter\r\n"));
			return ;
		}
		else
		{
			iter->second->setUserName(true, splitVector[0]);
			iter->second->setMode(splitVector[1]);
			iter->second->setUnused(splitVector[2]);
			iter->second->setUserRealName(true, splitVector[3]);
			insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", "info) Successful username.\r\n"));
			insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", "info) User Name : " + iter->second->getUserName() + "\r\n"));
			insertSendBuffer(iter->second, buildReplyPacket(RPL_WELCOME, iter->second->getUserName(), "Welcome irc Server \r\n"));
			#ifdef DEBUG
				std::cout << user << ' ' << mode << ' ' << unused << ' ' << realname << '\n';
			#endif
		}
	}
	return ;
}

void	Server::requestCommand(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::cout << "--requestCommand-- [command : " << command << ']' << ", " << "[param : " << param << ']' << std::endl;
	if (command == "PASS" || command == "USER")
		insertSendBuffer(iter->second, buildErrPacket(ERR_ALREADYREGISTRED, iter->second->getUserName(), "already registered \r\n"));
	else if (command == "JOIN")
	{
		//TODO : 채널 구조 구상 및 구현
		std::cout << "command : join " << std::endl;
	}
	else if (command == "PART")
	{
		std::cout << "command : PART " << std::endl;
	}
	else if (command == "QUIT")
	{
		std::cout << "command : quit " << std::endl;
		iter->second->setDisconnectFlag(true);
	}
	else if (command == "PRIVMSG" || command == "NOTICE")
	{
		std::cout << "command : privmsg" << std::endl;
		requestPrivateMsg(iter, command, param);
	}
	else if (command == "MODE")
	{
		std::cout << "command : " << command << ", param : " << param << std::endl;
		requestMode(iter, command, param);
	}
	else if (command == "KICK")
	{
		std::cout << "command : KICK " << std::endl;
	}
	else if (command == "INVITE")
	{
		std::cout << "command : INVITE " << std::endl;
	}
	else
		insertSendBuffer(iter->second, buildErrPacket(ERR_UNKNOWNCOMMAND, iter->second->getUserName(), "Unknown command \r\n"));
	return ;
}

void	Server::requestPrivateMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::string	tar_nick = param.substr(0, param.find(' '));
	std::string	msg = param.substr(param.find(' ') + 1) + "\r\n";

	//채널 메시지
	//루프 -> 유효한 방 -> 리스트 명단 -> 보내기
	302 :hena!hena@hostname
	//개인 메시지
	for (std::map<SOCKET, Client*>::iterator c_iter = client_map_.begin(); c_iter != client_map_.end(); c_iter++)
	{
		if (c_iter->second->getNickName() == tar_nick)
		{
			msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), iter->second->getHostName()) \
				+ " PRIVMSG " + iter->second->getNickName() + " :" + msg;
			insertSendBuffer(c_iter->second, msg);
			return ;
		}
	}
	//존재하지 않는 닉네임 메시지 처리
	//TODO : notice의 경우 에러 메시지를 처리하지 않는다.
	insertSendBuffer(iter->second, buildErrPacket(ERR_NOSUCHNICK, iter->second->getUserName(), tar_nick + ": No such nick \r\n"));
	(void)command;
	return ;
}

void		Server::requestMode(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::string	cp_param;
	std::string	tar_nick;
	std::string	option;
	size_t		pos;

	cp_param = param;
	pos = cp_param.find(' ');
	if (pos == std::string::npos)
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_NEEDMOREPARAMS, iter->second->getUserName(), ":info) Not enough parameter\r\n"));
		return ;
	}
	tar_nick = param.substr(0, pos);
	option = cp_param.erase(0, pos + 1);

	std::cout << "target : " << tar_nick << std::endl;
	std::cout << "option : " << option << std::endl;

	//채널에서 해당 타겟 클라이언트가 존재하는지 확인 -> 없다? -> Error(401): +o No such nick


	if (option == "-o")
	{

	}
	else
	{

	}

	//OPER <Name> <Pass>
	(void)command;
	(void)iter;
	(void)param;
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
			sock_count_ -= 1;
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
extern int g_count;

void	Server::Run(void)
{
	networkProcess();
	packetMarshalling();
	clientDisconnect();
	return ;
}
