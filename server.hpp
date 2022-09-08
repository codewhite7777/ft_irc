/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/08/28 18:40:10 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "client.hpp"
# include "channel.hpp"
# include "utils.hpp"
# include <iostream>
# include <map>
# include <netinet/in.h>

class Server
{
private:
	//config value
	std::string					raw_port_;
	std::string					raw_pwd_;

	//server info
	bool						status_;
	std::string					s_operator_pwd_;

	//network
	SOCKET						listen_sock_;
	struct sockaddr_in			s_addr_in_;
	unsigned short				s_port_;
	std::string					s_ip_;

	//client
	unsigned int				sock_count_;
	std::map<SOCKET, Client *>	client_map_;
	fd_set						read_set_;
	fd_set						write_set_;

	//channel
	std::map<std::string, Channel *>	chann_map_;

public:
	Server(int argc, char *argv[]);
	~Server(void);

	//configure port
	bool		configPort(std::string port);
	//bool		isValidPort(std::string port);
	//bool		getPortNumber(const char *str, int *o_value);
	//t_port		getPortType(int value);

	//configure pwd
	bool		configPwd(std::string pwd);
	bool		isValidPwd(std::string pwd);

	//network init, close
	void		networkInit(void);
	void		networkClose(void);

	//select
	int			getMaxFD(SOCKET sock);

	//network process
	void		networkProcess(void);
	void		acceptClient(SOCKET listen_sock);
	void		recvPacket(std::map<SOCKET, Client *>::iterator &iter);
	void		sendPacket(std::map<SOCKET, Client *>::iterator &iter);

	//network packet marshalling
	void		packetMarshalling(void);
	void		packetAnalysis(std::map<SOCKET, Client *>::iterator &iter);
	std::string	packetTrim(std::string& packet);

	//packet request :: PASS
	void		requestAuth(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//packet request :: NICK
	void		requestSetNickName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	bool		isOverlapNickName(std::string& search_nick);

	//packet request :: USER
	void		requestSetUserName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//packet request :: COMMAND
	void		requestCommand(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//packet request :: PRIVMSG
	void		requestPrivateMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//packet request :: JOIN
	void		requestJoin(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//packet request :: OPER
	void		requestMode(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//disconnect client
	void		clientDisconnect(void);

	//status
	bool		getStatus(void);

	//run
	void		Run(void);

private:
	void		insertSendBuffer(Client* target_client, const std::string& msg);
	std::string	buildErrPacket(std::string err_code, std::string user_name, std::string err_msg);
	std::string	buildReplyPacket(std::string reply_code, std::string user_name, std::string reply_msg);
	std::string	getUserInfo(std::string nickname, std::string username, std::string hostname);


public:	void partTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
public: void requestPartMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
public:	void kickTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
public: void requestKickMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
};
#endif
