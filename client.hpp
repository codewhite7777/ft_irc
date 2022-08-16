/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/08/17 02:05:48 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>

typedef int	SOCKET;

class Client
{
private:
	std::string	s_buf_;
	std::string	r_buf_;
	SOCKET		client_sock_;
	std::string	nick_name_;
	std::string	user_name_;
public:
	Client(SOCKET s);
	~Client(void);

	//client getter
	SOCKET&			getSocket(void);
	std::string&	getSendBuf(void);
	std::string&	getRecvBuf(void);
};

#endif
