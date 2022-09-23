#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP

#include "irc_protocol.hpp"
#include "server.hpp"
#include <string>

class Protocol
{
public:
    Protocol(Server* ptr_srvr);
    ~Protocol(void);

    std::string     rplPass();
    std::string     errPass();

private:
    Server* getServer_();

    Server* sv_;
};

#endif