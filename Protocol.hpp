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

# include <string>

class Server;
class Client;
class Channel;
class ChatBot;
class Protocol
{
public:
	Protocol(Server* sv);
	~Protocol(void);

	std::string		rplPass();
	std::string		rplWelcome(Client* clnt);
	std::string		rplYourHost(Client* clnt);
	std::string		rplCreated(Client* clnt);
	std::string		rplMyInfo(Client* clnt);
	std::string		rplNamReply(Client* clnt, Channel* chann);
	std::string		rplEndOfNames(Client* clnt, Channel* chann);
	std::string		rplInviting(Client* clnt, \
									Client* clnt_to_be_invtd, Channel* chann);
	std::string		rplYoureOper(Client* clnt);

	std::string		svPrivmsgClntWhenInit(Client* clnt);
	std::string		clntJoinChann(Client* clnt, Channel* chann);
	std::string		clntPartChann(Client* clnt, std::string msg);
	std::string		clntPrivmsgToChann(Client* clnt, std::string msg, \
										Channel* chann);
	std::string		clntPrivmsgToClnt(Client* clnt_send, std::string msg, \
										Client* clnt_recv);
	std::string		clntNoticeToChann(Client* clnt, std::string msg, \
										Channel* chann);
	std::string		clntNoticeToClnt(Client* clnt_send, std::string msg, \
										Client* clnt_recv);
	std::string		clntQuit(Client* clnt, std::string msg);
	std::string		clntNick(Client* clnt, std::string nick);
	std::string		errorClosingLink(Client* clnt, std::string msg);
	std::string		svPong(std::string token);
	std::string		clntKickUserInChann(Client* clnt, \
								Channel* chann, Client* user, std::string msg);
	std::string		clntInviteClnt(Client* clnt, \
									Client* clnt_to_be_invtd, Channel* chann);
	std::string		svModeClntAddOper(Client* clnt);
	std::string		clntKillClnt(Client* clnt, \
									Client* target_clnt, std::string msg);

	std::string		errWrongPasswd();
	std::string		errNotPassCmd();
	std::string		errNicknameInUse(Client* clnt, std::string nick);
	std::string		errNoNicknameGiven(Client* clnt);
	std::string		errErroneusNickname(Client* clnt, std::string nick);
	std::string		errNeedMoreParams();	
	std::string		errNoSuchNick(Client* clnt, std::string nick);
	std::string		errNoSuchChannel(Client* clnt, std::string chann_name);
	std::string		errNotOnChannel(Client* clnt, Channel* chann);	
	std::string		errChanOPrivsNeeded(Client* clnt, Channel* chann);
	std::string		errUserNotInChannel(Client* clnt, \
										Client* user, Channel *chann);
	std::string		errUserOnChannel(Client* clnt, \
										Client* user, Channel* chann);
	std::string		errNoOperHost(Client* clnt);
	std::string		errNoPrivileges(Client* clnt);
	std::string		errBadChanMask(Client* clnt, std::string chann_name);

	std::string		chatPrivOthers(ChatBot& chat, Channel *chann, std::string msg);

private:
	Server* getServer_();

	Server* sv_;
};

#endif