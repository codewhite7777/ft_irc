/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:20:30 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:20:31 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "utils.hpp"
# include <netinet/in.h>
# include <map>
# include <list>

#include "Chatbot.hpp"

# include <ctime>
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

	std::string				getCreatedDateAsString() const;

	const std::string&		getName(void) const;
	std::string				getNamePrefix() const;
	const std::string&		getVersion() const;
	const std::string&		getPwd(void) const;

	bool					isOverlapNickName(std::string& search_nick);

	Channel*				findChannel(std::string chann_name);
	void					assignNewChannel(Channel* new_chann);

	Client*					findClient(std::string clnt_nickname);

	std::list<Client*>*		makeOtherClntListInSameChanns(Client* clnt);

	void					requestAllChannsToEraseOneUser(Client* clnt);
	void					requestAllChannsToReplaceKeyNickOfUser(Client* clnt, \
														std::string nick_to_key);

	bool					isOperName(std::string name);
	bool					isOperPassword(std::string password);
	bool					isOperHost(std::string hostname);
	const std::string&		getOperType() const;

	void					requestAllClientsToDisconnect();
	void					sendErrorClosingLinkProtoToAllClientsWithMsg(\
															std::string msg);
	void					setStatusOff();

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
	std::string					name_;
	const std::string			version_;
	const time_t				created_time_;

	//network
	SOCKET						listen_sock_;
	struct sockaddr_in			s_addr_in_;
	unsigned short				s_port_;
	std::string					s_ip_;
	unsigned int				sock_count_;

	//oper
	const std::string			oper_name_;
	const std::string			oper_pwd_;
	const std::string			oper_host_;
	const std::string			oper_type_;

	//client
	std::map<SOCKET, Client*>	client_map_;
	fd_set						read_set_;
	fd_set						write_set_;

	//channel
	std::map<std::string, Channel*>	chann_map_;

	// bot
	ChatBot night_bot;
};

#endif
