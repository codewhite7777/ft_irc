/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:50:44 by alee              #+#    #+#             */
/*   Updated: 2022/09/26 14:52:12 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "utils.hpp"
# include <netinet/in.h>
# include <map>
# include <list>

//# include "Command.hpp"
//# include "Protocol.hpp"

class Command;
class Protocol;
class Client;
class Channel;

class Server
{
public:
	Server(int argc, char *argv[]);
	~Server(void);

	void					equipCommandAndProtocol(\
								Command* cmd, Protocol* proto);

	bool					getStatus(void) const;
	void					Run(void);

	Command*				getCommand() const;
	Protocol*				getProtocol() const;

	const std::string&		getName(void) const;
	std::string				getNamePrefix() const;
	const std::string&		getVersion() const;
	const std::string&		getPwd(void) const;

	bool					isOverlapNickName(std::string& search_nick);

	Channel*				findChannel(std::string chann_name);
	void					assignNewChannel(Channel* new_chann);

	Client*					findClient(std::string clnt_nickname);

	std::list<Client*>*		makeOtherClntListInSameChanns(Client* clnt);

	void					requestChannsToEraseOne(Client* clnt);

private:
	//configure port
	bool		configPort(std::string port);

	//configure pwd
	bool		configPwd(std::string pwd);

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

	//disconnect client
	void		clientDisconnect(void);

	//mgo.refactor
	void		processClientMessages(void);


	bool						status_;

	//config value
	std::string					raw_port_;
	std::string					raw_pwd_;

	Command*					cmd_;
	Protocol*					proto_;

	//server info
	//std::string					s_operator_pwd_;
	std::string					name_;
	const std::string			version_;

	//network
	SOCKET						listen_sock_;
	struct sockaddr_in			s_addr_in_;
	unsigned short				s_port_;
	std::string					s_ip_;

	//client
	unsigned int				sock_count_;
	std::map<SOCKET, Client*>	client_map_;
	fd_set						read_set_;
	fd_set						write_set_;

	//channel
	std::map<std::string, Channel*>	chann_map_;
};


	// ---------------------------------------------------------------------
	// to remove
	//network packet marshalling
	// void		packetMarshalling(void);
	// void		packetAnalysis(std::map<SOCKET, Client *>::iterator &iter);
	// std::string	takeFirstProtocol(std::string& packet);
	/*
	//packet request :: PASS
	void		requestAuth(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	//packet request :: NICK
	void		requestSetNickName(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	

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
						
	void		quitTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);

	void		inviteTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	*/
	//void		insertSendBuffer(Client* target_client, const std::string& msg);
	//std::string	buildErrPacket(std::string err_code, std::string user_name, std::string err_msg);
	//std::string	buildReplyPacket(std::string reply_code, std::string user_name, std::string reply_msg);
	//std::string	getUserInfo(std::string nickname, std::string username, std::string hostname);
	/*
	void requestPart(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	void requestPartMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	void kickTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	void requestKickMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param);
	*/
	// ---------------------------------------------------------------------


#endif
