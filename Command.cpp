#include "Command.hpp"

Command::Command(Server* sv)
    : sv_(sv) {}

Command::~Command(void) {}

void    Command::pass(Client* clnt)
{
    Protocol    proto(sv_);

    clnt->setPassFlag(true);
    clnt->appendToSendBuf(proto.rplPass());
}

void    Command::passwdMismatch(Client* clnt)
{
    Protocol    proto(sv_);

    clnt->appendToSendBuf(proto.errPass());
}
