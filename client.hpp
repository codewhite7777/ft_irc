/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/08/19 06:42:18 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>

typedef int	SOCKET;

class Client
{
private:
	std::string	s_buf_;	//network send_buf
	std::string	r_buf_;	//network recv_buf
	SOCKET		client_sock_;	//network socket
	std::string	nick_name_;
	std::string	user_name_;
	bool		disconnect_flag;
	bool		pass_flag;
	bool		nick_flag;
	bool		user_flag;
	bool		operator_flag;
public:
	Client(SOCKET s);
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
	bool			getPassFlag(void) const;

	//client nickname getter/setter
	bool			getNickFlag(void) const;
	void			setNickName(bool flag, std::string& nickname);
	std::string&	getNickName(void);

	//
	bool			getUserNameFlag(void) const;
	bool			getOperatorFlag(void) const;
};

#endif
