/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:35:38 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:58:43 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

class Server;
class Protocol;
class Client;

class Command
{
public:
	Command(Server* sv, Protocol* proto);
	~Command(void);

	void    pass(Client* clnt);
	void    nick(Client* clnt);
	void    user(Client* clnt);
	void	ping(Client* clnt);
	void    join(Client* clnt);
	void    part(Client* clnt);
	void    privmsg(Client* clnt);
	void	notice(Client* clnt);
	void    quit(Client* clnt);
	void	kick(Client* clnt);
	void    invite(Client* clnt);
	void	oper(Client* clnt);
	void	kill(Client* clnt);
	void	die(Client* clnt);

private:
	Server*     sv_;
	Protocol*   proto_;
};

#endif
