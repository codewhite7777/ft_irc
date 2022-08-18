/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/08/18 10:12:09 by alee             ###   ########.fr       */
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
	bool			isSetPass(void) const;
	bool			isSetNick(void) const;
	bool			isSetUserName(void) const;
	bool			isSetOperator(void) const;
};

#endif
