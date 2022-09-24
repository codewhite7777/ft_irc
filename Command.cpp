#include "Command.hpp"

Command::Command(Server* sv)
    : sv_(sv) {}

Command::~Command(void) {}

void    Command::pass(Client* clnt)
{
    Protocol    proto(sv_);

    if (clnt->getCommand() == "PASS")
    {
        if (clnt->getParam() == sv_->getPwd())
        {
            clnt->setPassFlag(true);
            clnt->appendToSendBuf(proto.rplPass());
        }
        else
        {
            clnt->appendToSendBuf(proto.errWrongPasswd());
        }
    }
    else
    {
        clnt->appendToSendBuf(proto.errNotPassCmd());
    }
}

void    Command::nick(Client* clnt)
{
    std::string     tmp_nick = clnt->getParam();
    Protocol        proto(sv_);

    if (sv_->isOverlapNickName(tmp_nick))
    {
        clnt->appendToSendBuf(proto.errNicknameInUse(tmp_nick));
    }
    else if (tmp_nick == "")
    {
        clnt->appendToSendBuf(proto.errNoNicknameGiven());
    }
    else
    {
        clnt->setNickname(tmp_nick);
        clnt->setNickFlagOn();
    }
}
