/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:35:38 by mgo               #+#    #+#             */
/*   Updated: 2022/09/25 17:57:56 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Protocol.hpp"
#include "Client.hpp"
#include "Server.hpp"

//Command

class Command
{
public:
    // todo: OCCF
    Command(Server* sv);
    ~Command(void);

    // PASS
    void    pass(Client* clnt);

    // NICK
    void    nick(Client* clnt);

    // USER
    void    user(Client* clnt);

    // JOIN
    void    join();

    // PART
    void    part();

    // PRIVMSG
    void    privmsg();

    // QUIT
    void    quit();

    // INVITE
    void    invite();

private:
    Server*     sv_;
    Protocol*   proto_;
};

#endif
