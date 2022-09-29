/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/09/26 16:54:49 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

typedef int	SOCKET; // todo: set typedefs

class Server;
class Command;
class Protocol;
class Channel;

class Client
{
public:
	Client(SOCKET s, std::string host_name, Server* sv);
	~Client(void);

	//client socket
	SOCKET&					getSocket(void);

	//client network buf
		// recvBuf
	void					appendToRecvBuf(unsigned char* buf);
	size_t					getRecvBufLength();
		// sendBuf
	void					appendToSendBuf(const std::string& str);
	const char*				getSendBufCStr();
	size_t					getSendBufLength();
	void					eraseSendBufSize(int size);

	// processMessage
	void					processMessageInRecvBuf();

	const std::string&		getParam() const;

	//client disconnect flag getter/setter
	bool					getDisconnectFlag(void) const;
	void					setDisconnectFlag(bool flag);

	//client PASS getter/setter
	void					setPassFlag(bool flag);
	bool					getPassFlag(void) const;

	//client NICK getter/setter
	void					setNickname(std::string nickname);
	const std::string&		getNickname() const;
	void					setNickFlagOn();
	bool					getNickFlag(void) const;

	//client USER getter/setter
	void					setUsername(std::string username);
	void					setHostname(std::string hostname);
	void					setRealname(std::string username);
	const std::string&		getUsername() const; // todo: private
	const std::string&		getHostname() const; // todo: private
	const std::string&		getRealname() const; // todo: private
	const std::string		getUserRealHostNamesInfo() const;
	void					setUserFlagOn();
	bool					getUserFlag() const;

	std::string				getNamesPrefix() const;

private:
	void					marshalMessage(std::string& command, std::string& param);
	std::string				extractFirstMsg(std::string& recv_buf);

	void					processProtocol();
	void					processToWelcome();
	bool					isPassed() const;
	bool					isWelcomed() const;
	void					processCommand();

	std::string&			getRecvBuf(void);
	std::string&			getSendBuf(void);


	SOCKET					sock_des_;

	std::string				send_buf_;
	std::string				recv_buf_;

	std::string				command_;
	std::string				param_;

	bool					pass_flag_;
	bool					nick_flag_; // nick_regi_flag
	bool					user_flag_;

	std::string				nickname_;
	std::string				username_;
	std::string				hostname_; // add ip_addr ?
	std::string				realname_;

	bool					disconnect_flag_;
	//bool					operator_flag_;

	bool					passed_;
	bool					welcomed_; // refistered
	
	Server*					sv_;
	Command*				cmd_;
	Protocol*				proto_;
};

#endif
