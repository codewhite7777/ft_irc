#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include "irc_protocol.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <string>

class Protocol
{
public:
    Protocol(Server* sv);
    ~Protocol(void);

    std::string     rplPass();
    std::string     errWrongPasswd();
    std::string     errNotPassCmd();

    std::string     errNicknameInUse(const std::string& nick);
    std::string     errNoNicknameGiven();

    std::string     errNeedMoreParams();

    // 001
    std::string     rplWelcome(Client* clnt);
    // 002
    std::string     rplYourHost(Client* clnt);
    // 003
    std::string     rplCreated(Client* clnt);
    // 004
    std::string     rplMyInfo(Client* clnt);

private:
    Server* getServer_();

    Server* sv_;
};

#endif