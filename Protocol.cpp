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

// 001
std::string     Protocol::rplWelcome(Client* clnt)
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret += RPL_WELCOME;
    ret += " " + clnt->getNickname();
    ret += " :Welcome to the IRC Network ";
    ret += clnt->getUserRealHostNamesInfo();
    ret += "\r\n";
    return ret;
}

// 002
std::string     Protocol::rplYourHost(Client* clnt)
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret += RPL_YOURHOST;
    ret += " " + clnt->getNickname();
    ret += " :Your host is ";
    ret += sv_->getHostName();
    ret += ", running version ";
    ret += sv_->getVersion();
    ret += "\r\n";
    return ret;
}
// 003
std::string     Protocol::rplCreated(Client* clnt)
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret += RPL_CREATED;
    ret += " " + clnt->getNickname();
    ret += " :This server was created ";
    ret += "05:22:40 Sep 25 2022"; // todo: sv_->getCreatedDate()
    ret += "\r\n";
    return ret;
}

// 004
std::string     Protocol::rplMyInfo(Client* clnt)
{
    std::string ret;

    ret = sv_->getHostNamePrefix();
    ret += RPL_MYINFO;
    ret += " " + clnt->getNickname();
    ret += " " + sv_->getHostName();
    ret += " " + sv_->getVersion();
    ret += " iosw"; // available user modes
    ret += " biklmnopstv"; // available channel modes
    ret += " :bklov";
    ret += "\r\n";
    return ret;
}

Server* Protocol::getServer_(void)
{
    return sv_;
}
