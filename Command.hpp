#ifndef COMMAND_HPP
# define COMMAND_HPP

//Command

class Command
{
public:
    // PASS
    bool    pass();

    // NICK
    bool    nick();

    // USER
    bool    user();

    // JOIN
    bool    join();

    // PART
    bool    part();

    // PRIVMSG
    bool    privmsg();

    // QUIT
    bool    quit();

    // INVITE
    bool    invite();

private:

};

#endif
