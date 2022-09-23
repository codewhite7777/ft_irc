#include "Protocol.hpp"

Protocol::Protocol(Server* sv)
    : sv_(sv) {}

Protocol::~Protocol(void) {}

std::string    Protocol::rplPass()
{
    std::string ret;

    ret = RPL_NONE;
    ret += " " +  getServer_()->getHostName() + " :info) Successful Authentication.\r\n";
    return (ret);
}

std::string     Protocol::errWrongPasswd()
{
    std::string ret;

    ret = ERR_PASSWDMISMATCH;
    ret += " " + getServer_()->getHostName() + " :info) Wrong password\r\n";
    return (ret);
}

std::string     Protocol::errNotPassCmd()
{
    std::string ret;

    ret = ERR_PASSWDMISMATCH;
    ret += " " + getServer_()->getHostName() + " :ex) <PASS> <password>\r\n";
    return (ret);
}

Server* Protocol::getServer_(void)
{
    return sv_;
}
