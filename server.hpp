/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/08/16 18:37:57 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <netinet/in.h>
# include <list>

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

class Server
{
private:
	std::string	raw_port_;
	std::string	raw_pwd_;
	bool		status_;

	//network
	SOCKET				listen_sock_;
	struct sockaddr_in	s_addr_in_;
	unsigned short		s_port_;
	std::string			s_ip_;

	int					client_max;

	//client
	std::list<SOCKET>	client_list_;
	fd_set				read_set;
	fd_set				write_set;

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

	//status
	bool	getStatus(void);

	//run
	void	Run(void);
};

#endif
