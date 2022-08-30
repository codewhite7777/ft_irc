/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/08/21 14:30:19 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <netinet/in.h>
# include "client.hpp"
# include <map>

typedef int	SOCKET;

/* ************************************************************************** */
/*   멤버 변수                                                                */
/*   Ex) int	value_;                                                       */
/*       SOCKET	sock_;                                                        */
/*                                                                            */
/* ************************************************************************** */

typedef enum e_port
{
	WELL_KNOWN_PORT,
	REGISTERED_PORT,
	DYNAMIC_PORT,
	INVALID_PORT,
}			t_port;

typedef enum e_buf
{
	BUFFER_MAX = 1024 << 4,
	// BUFFER_MAX = 100,
}			t_buf;

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

	std::string					serv_hostname_;
public:
	Server(int argc, char *argv[]);
	~Server(void);

	//configure port
	bool		configPort(std::string port);
	bool		isValidPort(std::string port);
	bool		getPortNumber(const char *str, int *o_value);
	t_port		getPortType(int value);

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
	void	nickSetting(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	void	userSetting(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);


	// hena
	void	joinChannel(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	void	privMessage(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	//packet request :: NICK
	void		requestSetNickName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	bool		isResetNickName(std::string& param);
	bool		isOverlapNickName(std::string& search_nick);

	//disconnect client
	void		clientDisconnect(void);

	//status
	bool		getStatus(void);

	//run
	void		Run(void);

private:
	void		insertSendBuffer(Client* target_client, const std::string& msg);
	std::string	buildErrReplyPacket(std::string errCode, std::string user_name, std::string replies);

};

#endif
