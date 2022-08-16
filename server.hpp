/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/08/15 23:54:00 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <netinet/in.h>

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

	void	network_init(void);
	void	network_close(void);

	//status
	bool	getStatus(void);
	void	Run(void);
};

#endif
