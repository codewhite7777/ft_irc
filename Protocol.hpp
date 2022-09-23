#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include "irc_protocol.hpp"
#include "Server.hpp"
#include <string>

class Protocol
{
public:
    Protocol(Server* sv);
    ~Protocol(void);

    std::string     rplPass();
    std::string     errWrongPasswd();
    std::string     errNotPassCmd();

private:
    Server* getServer_();

    Server* sv_;
};

#endif