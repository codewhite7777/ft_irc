/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/08/18 06:50:06 by alee             ###   ########.fr       */
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
	unsigned int				sock_count;
	std::map<SOCKET, Client *>	client_map_;
	fd_set						read_set;
	fd_set						write_set;

public:
	Server(int argc, char *argv[]);
	~Server(void);

	//configure port
	bool	config_port(std::string port);
	bool	is_valid_port(std::string port);
	bool	getPortNumber(const char *str, int *o_value);
	t_port	getPortType(int value);

	//configure pwd
	bool	config_pwd(std::string pwd);
	bool	is_valid_pwd(std::string pwd);


	//network init, close
	void	network_init(void);
	void	network_close(void);

	//select
	int		getMaxFD(SOCKET sock);

	//network process
	void	network_process(void);
	void	accept_client(SOCKET listen_sock);
	void	recv_client(std::map<SOCKET, Client *>::iterator &iter);
	void	send_client(Client& c_session);

	//network packet marshalling
	void	packet_marshalling(void);
	void	packet_analysis(std::map<SOCKET, Client *>::iterator &iter);

	//status
	bool	getStatus(void);

	//run
	void	Run(void);
};

#endif
