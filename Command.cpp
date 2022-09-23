#include "Command.hpp"

Command::Command(Server* sv, Client* cl)
    : sv_(sv), cl_(cl) {}

Command::~Command(void) {}

void    Command::pass(Client* clnt)
{
    Protocol    proto(sv_);

    if (clnt->getCommand() == "CAP")
        return ;
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
