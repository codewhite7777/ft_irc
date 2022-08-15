/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/08/15 13:24:51 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <netinet/in.h>

typedef int	SOCKET;

class Server
{
private:
	std::string	raw_port;
	std::string	raw_pwd;
	bool		status;

	//network
	SOCKET				listen_sock;
	struct	sockaddr_in	s_addr_in;
	unsigned short		port;
	



public:
	Server(int argc, char *argv[]);
	~Server(void);

	//server init
	bool	is_vaild_port(std::string port);
	bool	is_vaild_pwd(std::string pwd);



	//status
	bool	getStatus(void);


};

#endif
