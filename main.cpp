/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:47:12 by alee              #+#    #+#             */
/*   Updated: 2022/08/22 19:48:26 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include "Protocol.hpp"
#include "Singleton.hpp"

#include <iostream>

static void	singleton_equip_server(Server* ptr_sv)
{
	Singleton&	sngltn(Singleton::getInstance());

	sngltn.setServerPtr(ptr_sv);
}

static void	set_server_off(int sig)
{
	Singleton&	sngltn(Singleton::getInstance());

	(void)sig;
	sngltn.getServerPtr()->setStatusOff();
	std::cout << "\nServer status offed in ser_server_off!\n";
}

static void	set_signal_handler(void)
{
	signal(SIGINT, set_server_off);
}

int	main(int argc, char *argv[])
{
	Server		irc_server(argc, argv);
	Protocol	proto(&irc_server);
	Command		cmd(&irc_server, &proto);

	singleton_equip_server(&irc_server);
	set_signal_handler();
	irc_server.equipCommandAndProtocol(&cmd, &proto);
	while (irc_server.getStatus())
		irc_server.Run();
	return (0);
}
