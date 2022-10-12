/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:20:10 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:20:11 by mgo              ###   ########.fr       */
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
	Server*		sv;
	Protocol*	prtcl;

	sv = sngltn.getServerPtr();
	prtcl = sv->getProtocol();
	sv->requestAllClientsToDisconnect();
	if (sig == SIGINT)
		sv->sendErrorClosingLinkProtoToAllClientsWithMsg("SIGINT in server");
	else if (sig == SIGQUIT)
		sv->sendErrorClosingLinkProtoToAllClientsWithMsg("SIGQUIT in server");
	sv->setStatusOff();
	std::cout << "signum: [" << sig << "]\n";
}

static void	set_signal_handler(void)
{
	signal(SIGINT, set_server_off);
	signal(SIGQUIT, set_server_off);
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
