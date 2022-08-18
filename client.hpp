/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/08/18 17:01:28 by alee             ###   ########.fr       */
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

	//client getter
	SOCKET&			getSocket(void);
	std::string&	getSendBuf(void);
	std::string&	getRecvBuf(void);
	bool			getDisconnectFlag(void) const;
	void			setDisconnectFlag(bool flag);
	void			setPassFlag(bool flag);
	bool			getPassFlag(void) const;
	bool			getNickFlag(void) const;
	bool			getUserNameFlag(void) const;
	bool			getOperatorFlag(void) const;
};

#endif
