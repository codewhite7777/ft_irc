#include "Protocol.hpp"

Protocol::Protocol(Server* sv)
    : sv_(sv) {}

Protocol::~Protocol(void) {}

std::string    Protocol::rplPass()
{
    std::string ret;

    ret = RPL_NONE + getServer_()->getHostName() + ":info) Successful Authentication.\r\n";
    return (ret);
}

std::string     Protocol::errPass()
{
    std::string ret;

    ret = ERR_PASSWDMISMATCH + getServer_()->getHostName() + ":ex) <PASS> <password>\r\n";
    return (ret);
}

Server* Protocol::getServer_(void)
{
    return sv_;
}
