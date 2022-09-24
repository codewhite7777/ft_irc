#include "Protocol.hpp"

Protocol::Protocol(Server* sv)
    : sv_(sv) {}

Protocol::~Protocol(void) {}

std::string    Protocol::rplPass()
{
    std::string ret;

    ret = RPL_NONE;
    ret += " " +  sv_->getHostName() + " :info) Successful Authentication.\r\n";
    return ret;
}

std::string     Protocol::errWrongPasswd()
{
    std::string ret;

    ret = ERR_PASSWDMISMATCH;
    ret += " " + sv_->getHostName() + " :info) Wrong password\r\n";
    return ret;
}

std::string     Protocol::errNotPassCmd()
{
    std::string ret;

    ret = ERR_PASSWDMISMATCH;
    ret += " " + sv_->getHostName() + " :ex) <PASS> <password>\r\n";
    return ret;
}

std::string     Protocol::errNicknameInUse(const std::string& nick)
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret += ERR_NICKNAMEINUSE;
    ret += " *";
    ret += " " + nick;
    ret += " :Nickname is already in use.\r\n";
    return ret;
}

std::string     Protocol::errNoNicknameGiven()
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret = ERR_NONICKNAMEGIVEN;
    ret += " :No nickname is given.\r\n";
    return ret;
}

std::string     Protocol::errNeedMoreParams()
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret = ERR_NEEDMOREPARAMS;
    ret += " :Not enough parameters.\r\n";
    return ret;
}

Server* Protocol::getServer_(void)
{
    return sv_;
}
