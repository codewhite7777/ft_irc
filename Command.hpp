#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "client.hpp"
#include "Protocol.hpp"

//Command

class Command
{
public:
    // todo: OCCF
    Command(Server* sv);
    ~Command(void);

    // PASS
    void    pass(Client* clnt);
    void    passwdMismatch(Client* clnt);

    // NICK
    bool    nick();

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
};

#endif
