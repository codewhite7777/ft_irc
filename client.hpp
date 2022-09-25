/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/08/28 11:50:42 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

//#include "server.hpp"
# include <iostream>
# include <list>
#include <vector>

typedef int	SOCKET;

class Channel;
class Server;

class Client
{
public:
	Client(SOCKET s, std::string host_name, Server* sv);
	~Client(void);

	//client socket
	SOCKET&			getSocket(void);

	//client network buf
	
	// recvBuf
	void		appendToRecvBuf(unsigned char* buf);
	size_t		getRecvBufLength();

	std::string&	getRecvBuf(void);	// todo: private

	// sendBuf
	void		appendToSendBuf(const std::string& str);
	void		appendToSendBuf(unsigned char* buf); // todo: remove
	const char*	getSendBufCharStr();
	size_t		getSendBufLength();
	void		eraseSendBufSize(int size);
	

	std::string&	getSendBuf(void);	// todo: private
	
	// processMessage
	void	processMessageInBuf();
	

	//client disconnect getter/setter
	bool			getDisconnectFlag(void) const;
	void			setDisconnectFlag(bool flag);

	//client pass getter/setter
	void			setPassFlag(bool flag);
	bool			getPassFlag(void) const;

	//client nickname getter/setter
	bool			getNickFlag(void) const;
	void			setNickName(bool flag, std::string& nickname);
	std::string&	getNickName(void);
	
	//mgo nickname
	void				setNickname(std::string nickname);
	const std::string&	getNickname() const;
	void				setNickFlagOn();

	//client user getter/setter
	bool			getUserNameFlag(void) const;
	void			setUserName(bool flag, std::string &username);
	std::string&	getUserName(void);


	//client user mode setter
	void			setMode(std::string &mode);

	//client user unused setter
	void			setUnused(std::string &unused);

	//client realname getter/setter
	bool			getUserRealNameFlag(void) const;
	void			setUserRealName(bool flag, std::string &realname);
	std::string&	getUserRealName(void);

	//client user host getter
	std::string&	getHostName(void);



	//mgo username, hostname, realname
	void				setUsername(std::string username);
	void				setHostname(std::string hostname);
	void				setRealname(std::string username);

	const std::string&	getUsername() const;
	const std::string&	getHostname() const;
	const std::string&	getRealname() const;
	const std::string	getUserRealHostNamesInfo() const;

	void				setUserFlagOn();
	bool				getUserFlag() const;

	//mgo command, param
	std::string&	getCommand();
	std::string&	getParam();

private:
	void	marshalMessage(std::string& command, std::string& param);
	void	processProtocol();

	bool	isPassed() const;
	bool	isWelcomed() const;

	void	processToWelcome();
	void	processCommand();

	std::string				s_buf_;	//network send_buf
	std::string				r_buf_;	//network recv_buf
	SOCKET					client_sock_;	//network socket

	std::string				nick_name_;
	std::string				user_name_;
	std::string				host_name_;

	//mgo names
	std::string				nickname_;
	std::string				username_;
	std::string				hostname_;
	std::string				realname_;

	std::string				mode_;
	std::string				unused_;
	std::string				user_real_name_;

	bool					disconnect_flag_;
	bool					pass_flag_;
	bool					nick_flag_;
	bool					user_flag_;
	bool					user_real_name_flag_;
	//bool					operator_flag_;

	bool					passed_;
	bool					welcomed_;
	Server*					sv_;

	std::string				command_;
	std::string				param_;

};

#endif
