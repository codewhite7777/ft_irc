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

# include "Chatbot.hpp" // todo: organize
# include <list>
# include <map>
# include <netinet/in.h>
# include <string>

typedef int	SOCKET;

typedef enum e_buf
{
	BUFFER_MAX = 1024 << 4,
}			t_buf;

class Command;
class Protocol;
class Client;
class Channel;

class Server
{
public:
	Server(int argc, char *argv[]);
	~Server(void);

	bool					getStatus(void) const;
	void					Run(void);

	void					equipCommandAndProtocol(\
								Command* cmd, Protocol* proto);
	Command*				getCommand() const;
	Protocol*				getProtocol() const;

	const std::string&		getName(void) const;
	std::string				getNamePrefix() const;
	const std::string&		getVersion() const;
	const std::string&		getPwd(void) const;

	std::string				getCreatedDateAsString() const;
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
	bool		configPort(std::string port);
	bool		configPwd(std::string pwd);

	void		networkInit(void);
	void		networkClose(void);

	int			getMaxFD(SOCKET sock);
	void		networkProcess(void);
	void		acceptClient(SOCKET listen_sock);
	void		recvPacket(std::map<SOCKET, Client *>::iterator &iter);
	void		sendPacket(std::map<SOCKET, Client *>::iterator &iter);
	void		processClientMessages(void);

	void		clientDisconnect(void);

	bool						status_;

	std::string					raw_port_;
	std::string					raw_pwd_;

	Command*					cmd_;
	Protocol*					proto_;

	std::string					name_;
	const std::string			version_;
	const time_t				created_time_;

	SOCKET						listen_sock_;
	struct sockaddr_in			s_addr_in_;
	unsigned short				s_port_;
	std::string					s_ip_;
	unsigned int				sock_count_;

	const std::string			oper_name_;
	const std::string			oper_pwd_;
	const std::string			oper_host_;
	const std::string			oper_type_;

	std::map<SOCKET, Client*>	client_map_;
	fd_set						read_set_;
	fd_set						write_set_;

	std::map<std::string, Channel*>	chann_map_;

	ChatBot night_bot;
};

#endif
