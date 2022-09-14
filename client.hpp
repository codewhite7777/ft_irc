/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 01:02:07 by alee              #+#    #+#             */
/*   Updated: 2022/09/15 02:11:17 by alee             ###   ########.fr       */
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
	std::string	mode_;
	std::string	unused_;
	std::string	user_real_name_;
	std::string	host_name_;
	bool		disconnect_flag_;
	bool		pass_flag_;
	bool		nick_flag_;
	bool		user_flag_;
	bool		user_real_name_flag_;
	bool		operator_flag_;
	bool		file_trans_flag;
public:
	Client(SOCKET s, std::string host_name);
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

	//client file setter/getter
	void			setUserFileTransFlag(bool flag);
	bool			getUserFileTransFlag(void) const;

	bool			getOperatorFlag(void) const;
};

#endif
