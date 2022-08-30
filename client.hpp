/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/08/19 16:17:13 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
#include <string>

typedef int	SOCKET;

class Client
{
private:
	std::string	s_buf_;	//network send_buf
	std::string	r_buf_;	//network recv_buf
	SOCKET		client_sock_;	//network socket
	std::string	nick_name_;
	std::string	user_name_;
	bool		disconnect_flag_;
	bool		pass_flag_;
	bool		nick_flag_;
	bool		user_flag_;
	bool		operator_flag_;
	std::string	hostname_;
public:
	Client(SOCKET s, std::string hostname_);
	~Client(void);

	//client socket
	SOCKET&			getSocket(void);

	//client network buf
	std::string&	getSendBuf(void);
	std::string&	getRecvBuf(void);

	//client disconnect getter/setter
	bool			getDisconnectFlag(void) const;
	void			setDisconnectFlag(bool flag);

	//client pass getter/setter
	void			setPassFlag(bool flag);
	void			setNickFlag(bool flag, std::string param);
	void			setUserFlag(bool flag, std::string param);
	bool			getPassFlag(void) const;
	

	//client nickname getter/setter
	bool			getNickFlag(void) const;
	void			setNickName(bool flag, std::string& nickname);
	std::string&	getNickName(void);
	std::string	getHostName(void) const;

	//
	bool			getUserNameFlag(void) const;
	bool			getOperatorFlag(void) const;
};

#endif
