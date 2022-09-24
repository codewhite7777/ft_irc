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
    bool    user();

    // JOIN
    bool    join();

    // PART
    bool    part();

    // PRIVMSG
    bool    privmsg();

    // QUIT
    bool    quit();

    // INVITE
    bool    invite();

private:
    Server* sv_;
    Client* cl_;
};

#endif
