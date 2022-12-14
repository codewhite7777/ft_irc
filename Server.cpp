/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:20:25 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:20:26 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Protocol.hpp"
#include "irc_protocol.hpp"
#include "utils.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

Server::Server(int argc, char *argv[])
	: status_(true)
	, cmd_(NULL)
	, proto_(NULL)
	, name_("irc.server")
	, version_("ft_irc-v1.0")
	, created_time_(time(NULL))
	, sock_count_(0)
	, oper_name_("root")
	, oper_pwd_("12345")
	, oper_host_("127.0.0.1")
	, oper_type_("ServerOperator")
{
	if (argc != 3)
	{
		std::cerr << "Error: Invalid Argument" << std::endl;
		std::cerr << "./ircserv <port> <server_pwd>" << std::endl;
		status_ = false;
		return ;
	}
	if (configPort(argv[1]) == false)
	{
		std::cerr << "Error: Invalid Port" << std::endl;
		status_ = false;
		return ;
	}
	if (configPwd(argv[2]) == false)
	{
		std::cerr << "Error: Invalid password" << std::endl;
		status_ = false;
		return ;
	}
	networkInit();
}

Server::~Server(void)
{
	networkClose();
	for (std::map<std::string, Channel*>::iterator chann_it(chann_map_.begin())
		; chann_it != chann_map_.end()
		; ++chann_it)
	{
		delete chann_it->second;
		chann_it->second = NULL;
	}
	chann_map_.clear();
}

void	Server::equipCommandAndProtocol(Command* cmd, Protocol* proto)
{
	cmd_ = cmd;
	proto_ = proto;
}

bool	Server::getStatus(void) const
{
	return status_;
}

void	Server::Run(void)
{
	networkProcess();
	processClientMessages();
	clientDisconnect();
}

Command*	Server::getCommand() const
{
	return cmd_;
}

Protocol*	Server::getProtocol() const
{
	return proto_;
}

std::string		Server::getCreatedDateAsString() const
{
	std::string		ret;

	ret += ctime(&created_time_);
	return (ret);
}

const std::string&	Server::getName(void) const
{
	return name_;
}

std::string		Server::getNamePrefix() const
{
	return (":" + name_ + " ");
}

const std::string&	Server::getVersion() const
{
	return version_;
}

const std::string&	Server::getPwd(void) const
{
	return raw_pwd_;
}

bool	Server::configPort(std::string port)
{
	int	retPort(0);

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
	int	retval(0);
	int optval(1);

	// getting listen socket
	listen_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock_ == -1)
	{
		std::cerr << "Error: listen_sock_ socket() failed" << std::endl;
		status_ = false;
		return ;
	}

	// setting listen_sock_ O_NONBLOCK
	fcntl(listen_sock_, F_SETFL, O_NONBLOCK);

	// setting listen_sock_ SO_REUSEADDR
	retval = setsockopt(listen_sock_, SOL_SOCKET, SO_REUSEADDR, \
		&optval, sizeof(optval));
	if (retval == -1)
	{
		std::cerr << "Error: setting sockopt SO_REUSEADDR failed" << std::endl;
		status_ = false;
		return ;
	}

	// unsetting Nagle algorithm
	retval = setsockopt(listen_sock_, IPPROTO_TCP, TCP_NODELAY, \
		&optval, sizeof(optval));
	if (retval == -1)
	{
		std::cerr << "Error: setting sockopt TCP_NODELAY failed" << std::endl;
		status_ = false;
		return ;
	}

	// bind
	memset(&s_addr_in_, 0x00, sizeof(s_addr_in_));
	s_addr_in_.sin_family = AF_INET;
	s_addr_in_.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in_.sin_port = htons(s_port_);
	retval = bind(listen_sock_, \
		reinterpret_cast<struct sockaddr *>(&s_addr_in_), sizeof(s_addr_in_));
	if (retval == -1)
	{
		std::cerr << "Error: listen_sock_ bind() failed" << std::endl;
		status_ = false;
		return ;
	}

	// listen
	retval = listen(listen_sock_, SOMAXCONN);
	if (retval == -1)
	{
		std::cerr << "Error: listen_sock_ listen() failed" << std::endl;
		status_ = false;
		return ;
	}

	sock_count_ += 1;
	std::cout << "IRC Server started" << std::endl;
}

void	Server::networkClose(void)
{
	close(listen_sock_);
}

int	Server::getMaxSD(void)
{
	int	max_fd(listen_sock_);

	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin()
		; iter != client_map_.end()
		; iter++)
	{
		if (max_fd < iter->first)
			max_fd = iter->first;
	}
	return (max_fd);
}

void	Server::networkProcess(void)
{
	struct timeval	time_out;
	int				select_result(0);

	FD_ZERO(&read_set_);
	FD_ZERO(&write_set_);
	FD_SET(listen_sock_, &read_set_);
	for (std::map<SOCKET, Client*>::iterator iter = client_map_.begin()
		; iter != client_map_.end()
		; iter++)
	{
		FD_SET(iter->first, &read_set_);
		FD_SET(iter->first, &write_set_);
	}
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;
	select_result = select(getMaxSD() + 1, &read_set_, &write_set_, NULL, \
		&time_out);
	if (select_result == -1)
	{
		std::cerr << "select() failed\n";
		status_ = false;
		return ;
	}
	if (select_result > 0)
	{
		if (FD_ISSET(listen_sock_, &read_set_))
		{
			acceptClient(listen_sock_);
			if (select_result == 1)
				return ;
		}
		for (std::map<SOCKET, Client*>::iterator iter = client_map_.begin()
			; iter != client_map_.end()
			; iter++)
		{
			if (FD_ISSET(iter->first, &read_set_))
				recvMsgEachClnt(iter->first, iter->second);
			if ((iter->second->getSendBufLength() > 0) \
				&& FD_ISSET(iter->first, &write_set_))
				sendMsgEachClnt(iter->first, iter->second);
		}
	}
}

void	Server::recvMsgEachClnt(SOCKET sdes, Client* clnt)
{
	unsigned char	buf[BUFFER_MAX];
	int				recv_ret(0);
	
	recv_ret = recv(sdes, reinterpret_cast<void *>(buf), sizeof(buf), 0);
	if (recv_ret == 0)
		clnt->setDisconnectFlag(true);
	else if (recv_ret > 0)
	{
		buf[recv_ret] = '\0';
		clnt->appendToRecvBuf(buf);
		clnt->promptRecvedMsg();
	}
}

void	Server::sendMsgEachClnt(SOCKET sdes, Client* clnt)
{
	unsigned char	buf[BUFFER_MAX];
	int				send_ret(0);

	memcpy(buf, clnt->getSendBufCStr(), clnt->getSendBufLength() + 1);
	send_ret = send(sdes, reinterpret_cast<void *>(buf), \
		strlen(reinterpret_cast<char *>(buf)), 0);
	if (send_ret == -1)
		clnt->setDisconnectFlag(true);
	else if (send_ret > 0)
	{
		clnt->promptSendedMsg();
		clnt->eraseSendBufSize(send_ret);
	}
}

void	Server::acceptClient(SOCKET listen_sock)
{
	SOCKET				client_sock(-1);
	struct sockaddr_in	c_addr_in;
	socklen_t			c_addr_len(sizeof(c_addr_in));

	memset(&c_addr_in, 0x00, sizeof(c_addr_in));
	client_sock = accept(listen_sock, \
		reinterpret_cast<sockaddr *>(&c_addr_in), &c_addr_len);
	if (client_sock == -1)
		return ;
	if (sock_count_ + 3 >= FD_SETSIZE)
	{
		close(client_sock);
		return ;
	}
	fcntl(client_sock, F_SETFL, O_NONBLOCK);
	Client*	new_client = new Client(client_sock, \
		inet_ntoa(c_addr_in.sin_addr), this);
	client_map_.insert(std::make_pair(client_sock, new_client));
	sock_count_ += 1;

	std::cout << "\t------------------------" << std::endl;
	std::cout << "\tConnecting with a client" << std::endl;
	std::cout << "\tClient socket : " << client_sock << std::endl;
	std::cout << "\tClient port   : " << ntohs(c_addr_in.sin_port) << std::endl;
	std::cout << "\tClient ip     : " << inet_ntoa(c_addr_in.sin_addr) << std::endl;
	std::cout << "\t------------------------" << std::endl;
}

bool	Server::isOverlapNickName(std::string& search_nick)
{
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin()
		; iter != client_map_.end()
		; iter++)
	{
		if (iter->second->getNickname() == search_nick)
			return (true);
	}
	return (false);
}

void	Server::processClientMessages()
{
	for (std::map<SOCKET, Client*>::iterator iter = client_map_.begin()\
		; iter != client_map_.end()\
		; iter++)
	{
		if (iter->second->getRecvBufLength() > 0)
		{
			iter->second->processMessageInRecvBuf();
		}
	}
}

void	Server::clientDisconnect(void)
{
	for (std::map<SOCKET, Client *>::iterator iter = client_map_.begin()
		; iter != client_map_.end()
		;)
	{
		if (iter->second->getDisconnectFlag() == true)
		{
			if (iter->second->getSendBufLength() > 0)
				sendMsgEachClnt(iter->first, iter->second);
			requestAllChannsToQuitClntUnexpectdly(iter->second);
			close(iter->first);
			delete iter->second;
			std::cout << iter->first << " Socket Disconnected" << std::endl;
			iter = client_map_.erase(iter);
			sock_count_ -= 1;
		}
		else
			iter++;
	}
}

void		Server::requestAllChannsToQuitClntUnexpectdly(Client* clnt)
{
	for (std::map<std::string, Channel*>::iterator	chann_it = chann_map_.begin()
		; chann_it != chann_map_.end()
		; ++chann_it)
	{
		if (chann_it->second->isUserIn(clnt))
		{
			chann_it->second->eraseClntIfIs(clnt);
			chann_it->second->sendToAll(proto_->clntQuit(clnt, "unexpectedly quit"));
		}
	}
}

Channel*	Server::findChannel(std::string chann_name)
{
	std::map<std::string, Channel*>::iterator	chann_it;

	chann_it = chann_map_.find(chann_name);
	if (chann_it != chann_map_.end())
		return (chann_it->second);
	else
		return NULL;
}

void		Server::assignNewChannel(Channel* new_chann)
{
	chann_map_.insert(std::make_pair(new_chann->getName(), new_chann));
}

Client*		Server::findClient(std::string clnt_nickname)
{
	for (std::map<SOCKET, Client*>::iterator clnt_it(client_map_.begin())
		; clnt_it != client_map_.end()
		; ++clnt_it)
	{
		if (clnt_nickname == clnt_it->second->getNickname())
			return (clnt_it->second);
	}
	return NULL;
}

std::list<Client*>*		Server::makeOtherClntListInSameChanns(Client* clnt)
{
	std::list<Client*>*	ret(NULL);
	Channel*			chann_ptr(NULL);
	std::list<Client*>*	each_clnt_list(NULL);

	ret = new std::list<Client*>;
	for (std::map<std::string, Channel*>::iterator chann_it(chann_map_.begin())
		; chann_it != chann_map_.end()
		; ++chann_it)
	{
		chann_ptr = chann_it->second;
		each_clnt_list = chann_ptr->makeClntListInChannExceptOne(clnt);
		ret->assign(each_clnt_list->begin(), each_clnt_list->end());
		delete each_clnt_list;
		each_clnt_list = NULL;
		chann_ptr = NULL;
	}
	ret->unique();
	return ret;
}

void				Server::requestAllChannsToEraseOneUser(Client* clnt)
{
	Channel*	each_chann_ptr(NULL);

	for (std::map<std::string, Channel*>::iterator chann_it(chann_map_.begin())
		; chann_it != chann_map_.end()
		; ++chann_it)
	{
		each_chann_ptr = chann_it->second;
		each_chann_ptr->eraseClntIfIs(clnt);
		each_chann_ptr = NULL;
	}
}

void			Server::requestAllChannsToReplaceKeyNickOfUser(Client* clnt, \
														std::string nick_to_key)
{
	Channel*	each_chann_ptr(NULL);

	for (std::map<std::string, Channel*>::iterator chann_it(chann_map_.begin())
		; chann_it != chann_map_.end()
		; ++chann_it)
	{
		each_chann_ptr = chann_it->second;
		each_chann_ptr->replaceClntKeyNick(clnt, nick_to_key);
		each_chann_ptr = NULL;
	}
}

bool		Server::isOperName(std::string name)
{
	if (this->oper_name_ == name)
		return true;
	else
		return false;
}

bool		Server::isOperPassword(std::string password)
{
	if (this->oper_pwd_ == password)
		return true;
	else
		return false;
}

bool		Server::isOperHost(std::string hostname)
{
	if (this->oper_host_ == hostname)
		return true;
	else
		return false;
}

const std::string&	Server::getOperType() const
{
	return (this->oper_type_);
}

void		Server::requestAllClientsToDisconnect()
{
	for (std::map<SOCKET, Client *>::iterator clnt_it = client_map_.begin()
		; clnt_it != client_map_.end()
		; clnt_it++)
	{
		clnt_it->second->setDisconnectFlag(true);
	}
}

void		Server::sendErrorClosingLinkProtoToAllClientsWithMsg(\
													std::string msg)
{
	Client*		each_clnt;
	for (std::map<SOCKET, Client *>::iterator clnt_it = client_map_.begin()
		; clnt_it != client_map_.end()
		; clnt_it++)
	{
		each_clnt = clnt_it->second;
		each_clnt->appendToSendBuf(proto_->errorClosingLink(each_clnt, msg));
	}
}

void		Server::setStatusOff()
{
	this->status_ = false;
}
