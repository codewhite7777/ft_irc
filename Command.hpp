/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:35:38 by mgo               #+#    #+#             */
/*   Updated: 2022/10/01 16:45:21 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Protocol.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Command
{
public:
	// todo: OCCF
	Command(Server* sv, Protocol* proto);
	~Command(void);

	// PASS
	void    pass(Client* clnt);

	// NICK
	void    nick(Client* clnt);

	// USER
	void    user(Client* clnt);

	// JOIN
	void    join(Client* clnt);

	// PART
	void    part(Client* clnt);

	// PRIVMSG
	void    privmsg(Client* clnt);

	// QUIT
	void    quit(Client* clnt);

	// INVITE
	void    invite();

	void	ping(Client* clnt);
	void	pong(Client* clnt);

private:
	Server*     sv_;
	Protocol*   proto_;
};

#endif
