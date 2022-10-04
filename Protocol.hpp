/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Protocol.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:35:56 by mgo               #+#    #+#             */
/*   Updated: 2022/10/01 16:54:46 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include "irc_protocol.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <string>

class Protocol
{
public:
	Protocol(Server* sv);
	~Protocol(void);

	std::string		rplPass();
	std::string		errWrongPasswd();
	std::string		errNotPassCmd();

	std::string		errNicknameInUse(const std::string& nick);
	std::string		errNoNicknameGiven();

	std::string		errNeedMoreParams();

	std::string		rplWelcome(Client* clnt);
	std::string		rplYourHost(Client* clnt);
	std::string		rplCreated(Client* clnt);
	std::string		rplMyInfo(Client* clnt);

	std::string		clntJoinChann(Client* clnt, Channel* chann);
	std::string		rplNamReply(Client* clnt, Channel* chann);
	std::string		rplEndOfNames(Client* clnt, Channel* chann);

	std::string		clntPartChann(Client* clnt, Channel* chann);

	std::string		msgPong(std::string token);

	std::string		clntPrivmsgToChann(Client* clnt, std::string msg, \
										Channel* chann);
	std::string		clntPrivmsgToClnt(Client* clnt_send, std::string msg, \
										Client* clnt_recv);

	std::string		errNoSuchNick(Client* clnt, std::string nick);
	std::string		errNoSuchChannel(Client* clnt, std::string chann_name);

	std::string		rplErrorClosing(Client* clnt, std::string msg);

private:
	Server* getServer_();

	Server* sv_;
};

#endif