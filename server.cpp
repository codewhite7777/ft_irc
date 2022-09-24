/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:28 by alee              #+#    #+#             */
/*   Updated: 2022/08/28 18:31:22 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "irc_protocol.hpp"
#include <arpa/inet.h>
#include <cctype>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstring>
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
	host_name_ = "irc.server";

	//network init
	networkInit();
}

Server::~Server(void)
{
	networkClose();
}

bool	Server::configPort(std::string port)
{
	int	retPort;
	if (isValidPort(port) == false)
		return (false);
	if (setPortNumber(port.c_str(), &retPort) == false)
		return (false);
	s_port_ = static_cast<unsigned short>(retPort);
	return (true);
}

bool	Server::configPwd(std::string pwd)
{
	if (isValidPwd(pwd) == false)
		return (false);
	raw_pwd_ = pwd;
	return (true);
}

void	Server::networkInit(void)
{
	int retval;

	// socket
	listen_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock_ == -1)
	{
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
	if (retval == -1)
	{
		status_ = false;
		return ;
	}

	// unset Nagle algorithm
	//int optval = 1;
	retval = setsockopt(listen_sock_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
	if (retval == -1)
	{
		status_ = false;
		return ;
	}

	// bind
	memset(&s_addr_in_, 0x00, sizeof(s_addr_in_));
	s_addr_in_.sin_family = AF_INET;
	s_addr_in_.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in_.sin_port = htons(s_port_); // SERVERPORT
	retval = bind(listen_sock_, reinterpret_cast<struct sockaddr *>(&s_addr_in_), sizeof(s_addr_in_));
	if (retval == -1)
	{
		status_ = false;
		return ;
	}

	// listen
	retval = listen(listen_sock_, SOMAXCONN);
	if (retval == -1)
	{
		status_ = false;
		return ;
	}

	//client count
	sock_count_ += 1;

	std::cout << "IRC Server started" << std::endl;
}

void	Server::networkClose(void)
{
	close(listen_sock_);
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
	Client*	new_client = new Client(client_sock, inet_ntoa(c_addr_in.sin_addr), this);

	//push client socket
	client_map_.insert(std::make_pair(client_sock, new_client));

	//client count
	sock_count_ += 1;

	// test: display client network info
	std::cout << "-------------------" << std::endl;
	std::cout << "client connected" << std::endl;
	std::cout << "client socket : " << client_sock << std::endl;
	std::cout << "client port   : " << ntohs(c_addr_in.sin_port) << std::endl;
	std::cout << "client ip     : " << inet_ntoa(c_addr_in.sin_addr) << std::endl;
	std::cout << "-------------------" << std::endl;
}

void	Server::recvPacket(std::map<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];
	int				recv_ret(0);
	
	recv_ret = recv(iter->first, reinterpret_cast<void *>(buf), sizeof(buf), 0);
	if (recv_ret == 0)
		iter->second->setDisconnectFlag(true);
	else if (recv_ret > 0)
	{
		buf[recv_ret] = '\0';
		iter->second->appendToRecvBuf(buf); //iter->second->getRecvBuf().append(reinterpret_cast<char *>(buf));

		// test: print RecvBuf
		std::cout << "<" << iter->second->getSocket() << "|" << iter->second->getNickName() << ">"\
			 << " recvPacket: " << "[" << buf << "]\n";
	}
}

void	Server::sendPacket(std::map<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];
	int				send_ret(0);

	memcpy(buf, iter->second->getSendBufCharStr(), iter->second->getSendBufLength() + 1);
	send_ret = send(iter->first, reinterpret_cast<void *>(buf), strlen(reinterpret_cast<char *>(buf)), 0);
	if (send_ret == -1)
		iter->second->setDisconnectFlag(true);
	else if (send_ret > 0)
	{
		// test: print SendBuf
		std::cout << "<" << iter->second->getSocket() << "|" << iter->second->getNickName() << ">"\
			 << " sendPacket: " << "[" << buf << "]\n";

		iter->second->eraseSendBufSize(send_ret); //iter->second->getSendBuf().erase(0, send_ret);
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

/*
	take first protocol from packet

	1) [one protocol] If no more behind \r\n, just return pure packet.
	2) [more than one protocol] If something behind \r\n, split packet by first \r\n and return only first protocol.
	3) [no protocol] If no \r\n, return empty.
*/
std::string	Server::takeFirstProtocol(std::string& packet)
{
	std::string	first_proto("");
	size_t		found(0);

	found = packet.find("\r\n");
	if (found != std::string::npos)
	{
		first_proto = packet.substr(0, found);
		packet.erase(0, found + 2);
	}
	return (first_proto);
}

void	Server::packetAnalysis(std::map<SOCKET, Client *>::iterator& iter)
{
	std::string	packet_buf;
	std::string	command;
	std::string	param;

	// parsing
	packet_buf = takeFirstProtocol(iter->second->getRecvBuf());
	//std::cout << "recvBuf(after): [" <<	iter->second->getRecvBuf() << "]\n";


	if (packet_buf.find(' ') != std::string::npos)
	{
		command = packet_buf.substr(0, packet_buf.find(' '));
		param = packet_buf.substr(packet_buf.find(' ') + 1);
	}
	else
	{
		command = "";
		param = packet_buf;
	}

	std::cout << "--requestCommand-- [command : " << command << ']' << ", " << "[param : " << param << ']' << "\n\n";

	// executing
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
	if (command != "PASS")
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":ex) <PASS> <password>\r\n"));
		return ;
	}
	if (this->raw_pwd_ == param)
	{
		iter->second->setPassFlag(true);
		insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", ":info) Successful Authentication.\r\n"));
		std::cout << "-----------------------------------" << std::endl;
		std::cout << iter->first << " Client Successful Authentication." << std::endl;
		std::cout << "-----------------------------------" << std::endl;
	}
	else
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":info> Wrong password\r\n"));
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

void	Server::requestSetUserName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	if (command != "USER")
	{
		insertSendBuffer(iter->second, buildErrPacket(ERR_PASSWDMISMATCH, "UNKNOWN", ":ex) <USER> <user> <mode> <unused> <realname>\r\n"));
		return ;
	}
	else
	{
		std::vector<std::string> splitVector = split(param, ' ');
		if (splitVector.size() < 4)
		{
			insertSendBuffer(iter->second, buildErrPacket(ERR_NEEDMOREPARAMS, "UNKNOWN", ":info) Not enough parameter\r\n"));
			return ;
		}
		else
		{
			iter->second->setUserName(true, splitVector[0]);
			iter->second->setMode(splitVector[1]);
			iter->second->setUnused(splitVector[2]);

			std::string	tmp_user_name;
			for (std::size_t i = 3; i < splitVector.size(); ++i)
			{
				tmp_user_name += splitVector[i];
			}
			std::size_t	found = tmp_user_name.find_first_of(':');
			if (found != std::string::npos)
			{
				tmp_user_name.erase(found);
			}
			iter->second->setUserRealName(true, tmp_user_name);
			// 내 맘임!
			// insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", "info) Successful username.\r\n"));
			// insertSendBuffer(iter->second, buildReplyPacket(RPL_NONE, "UNKNOWN", "info) User Name : " + iter->second->getUserName() + "\r\n"));
			// insertSendBuffer(iter->second, buildReplyPacket(RPL_WELCOME, iter->second->getUserName(), "Welcome irc Server \r\n"));

			//001
			std::string tmp = ":" + host_name_ + " 001 " + iter->second->getNickName() + " :Welcome to the ft_irc Network ";
			std::string	user_info = iter->second->getNickName() \
							+ "!" + iter->second->getUserName() \
							+ "@" + iter->second->getHostName();
			tmp += user_info + "\r\n";
			insertSendBuffer(iter->second, tmp);
			
			//002
			std::string RPL_YOURHOST("");
			RPL_YOURHOST += ":" + host_name_ + " 002 " + iter->second->getNickName() + \
				" :Your host is " + host_name_ + ", running version " + "ft_irc-1" + "\r\n";
			insertSendBuffer(iter->second, RPL_YOURHOST);

			//003
			std::string RPL_CREATED("");
			RPL_CREATED += ":" + host_name_ + " 003 " + iter->second->getNickName() + \
				" :This server was created ";
			std::string tmp_curr_time;
			tmp_curr_time = "07:08:31 Sep 23 2022";
			RPL_CREATED += tmp_curr_time + "\r\n";
			insertSendBuffer(iter->second, RPL_CREATED);
			
			//004
			std::string RPL_MYINFO = ":";
			RPL_MYINFO += host_name_ + 
			" 004 " + 
			iter->second->getNickName() + " " + 
			host_name_ + " "
			"v1.0" + " " + "io " + "snio :\r\n";
			insertSendBuffer(iter->second, RPL_MYINFO);

			// irc.local 004 henalove2 irc.local InspIRCd-3 iosw biklmnopstv :bklov

			/*
				001    RPL_WELCOME
						"Welcome to the Internet Relay Network
						<nick>!<user>@<host>"
				002    RPL_YOURHOST
						"Your host is <servername>, running version <ver>"
				003    RPL_CREATED
						"This server was created <date>"
				004    RPL_MYINFO
						"<servername> <version> <available user modes>
						<available channel modes>"

				- The server sends Replies 001 to 004 to a user upon
				successful registration.

				005    RPL_BOUNCE
						"Try server <server name>, port <port number>"

				- Sent by the server to a user to suggest an alternative
				server.  This is often used when the connection is
				refused because the server is already full.
			*/
			

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
	// if (command == "PONG")
	// 	return ;
	if (command == "PASS" || command == "USER")
		insertSendBuffer(iter->second, buildErrPacket(ERR_ALREADYREGISTRED, iter->second->getUserName(), "already registered \r\n"));
	else if (command == "PING")
	{
		STRING msg = ":" + host_name_ + " PONG "+ host_name_ +  " :\r\n";
		insertSendBuffer(iter->second, msg);
		// PONG csd.bu.edu tolsun.oulu.fi
	}
	else if (command == "PONG")
		return ;
	else if (command == "JOIN")
	{
		//TODO : 채널 구조 구상 및 구현
		std::cout << "command : join " << std::endl;
		requestJoin(iter, command, param);
	}
	else if (command == "PART")
	{
		std::cout << "command : PART " << std::endl;
		requestPart(iter, command, param);
	}
	// else if (command == "QUIT")
	// {
	// 	std::cout << "command : quit " << std::endl;

	// 	iter->second->setDisconnectFlag(true);
	// 	quitTest(iter, command, param);
	// 	return ;
	// }
	// else if (command == "PRIVMSG" || command == "NOTICE")
	// {
	// 	std::cout << "command : privmsg" << std::endl;
	// 	requestPrivateMsg(iter, command, param);
	// }
	// else if (command == "MODE")
	// {
	// 	std::cout << "command : " << command << ", param : " << param << std::endl;
	// 	//requestMode(iter, command, param);
	// }
	// else if (command == "KICK")
	// {
	// 	std::cout << "command : KICK " << std::endl;
	// 	requestKickMsg(iter, command, param);
	// }
	// else if (command == "INVITE")
	// {
	// 	std::cout << "command : INVITE " << std::endl;
	// 	inviteTest(iter, command, param);
	// }
	else
		insertSendBuffer(iter->second, buildErrPacket(ERR_UNKNOWNCOMMAND, iter->second->getUserName(), "Unknown command \r\n"));
	return ;
}

void	Server::processClientMessages()
{
	for (std::map<SOCKET, Client*>::iterator iter = client_map_.begin()\
		; iter != client_map_.end()\
		; iter++)
	{
		if (iter->second->getRecvBufLength() > 0)
		{
			//packetAnalysis(iter);
			iter->second->processMessageInBuf();
		}
	}
}

void	Server::clientDisconnect(void)
{
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin(); iter != client_map_.end();)
	{
		if (iter->second->getDisconnectFlag() == true)
		{
			std::cout << iter->first << " Socket Disconnected" << std::endl;
			close(iter->first);
			delete iter->second; // critical point
			iter = client_map_.erase(iter);
			sock_count_ -= 1;
		}
		else
			iter++;
	}
}

bool	Server::getStatus(void) const
{
	return (status_);
}

void	Server::Run(void)
{
	networkProcess();
	processClientMessages(); //packetMarshalling();
	clientDisconnect();
}

std::string	Server::getHostName(void) const
{
	return host_name_;
}

std::string	Server::getHostNamePrefix() const
{
	return (":" + host_name_ + " ");
}

std::string	Server::getPwd(void)
{
	return raw_pwd_;
}
