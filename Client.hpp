/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:19:55 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:19:57 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

typedef int	SOCKET;

class Server;
class Command;
class Protocol;
class Channel;

class Client
{
public:
	Client(SOCKET s, std::string host_name, Server* sv);
	~Client(void);

	SOCKET&					getSocket(void);

	void					appendToRecvBuf(unsigned char* buf);
	size_t					getRecvBufLength(void);
	void					appendToSendBuf(const std::string& str);
	const char*				getSendBufCStr(void);
	size_t					getSendBufLength(void);
	void					eraseSendBufSize(int size);
	void					processMessageInRecvBuf();
	const std::string&		getCommand(void) const;
	const std::string&		getParam(void) const;

	bool					isWelcomed(void) const;
	bool					getDisconnectFlag(void) const;
	void					setDisconnectFlag(bool flag);
	void					setPassFlag(bool flag);
	bool					getPassFlag(void) const;
	void					setNickname(std::string nickname);
	const std::string&		getNickname(void) const;
	void					setNickFlagOn(void);
	bool					getNickFlag(void) const;
	void					setUsername(std::string username);
	void					setHostname(std::string hostname);
	void					setRealname(std::string username);
	const std::string&		getUsername(void) const;
	const std::string&		getHostname(void) const;
	const std::string&		getRealname(void) const;
	const std::string		getUserRealHostNamesInfo(void) const;
	void					setUserFlagOn(void);
	bool					getUserFlag(void) const;
	std::string				getNamesPrefix(void) const;

	void					setSvOperFlagOn(void);
	bool					isSvOper(void) const;

	void					promptRecvedMsg(void);
	void					promptSendedMsg(void);

private:
	void					marshalMessageToCmdAndParam(void);
	std::string				extractFirstMsg(std::string& recv_buf);
	void					processProtocol(void);
	void					processAuthToWelcome(void);
	bool					isPassed(void) const;
	void					processCommand(void);
	void					clearCommandAndParam(void);
	std::string&			getRecvBuf(void);
	std::string&			getSendBuf(void);

	SOCKET					sock_des_;
	std::string				send_buf_;
	std::string				recv_buf_;
	std::string				command_;
	std::string				param_;
	bool					pass_flag_;
	bool					nick_flag_;
	bool					user_flag_;
	std::string				nickname_;
	std::string				username_;
	std::string				hostname_;
	std::string				realname_;
	bool					disconnect_flag_;
	bool					sv_oper_flag_;
	bool					passed_;
	bool					welcomed_;

	Server*					sv_;
	Command*				cmd_;
	Protocol*				proto_;
};

#endif
