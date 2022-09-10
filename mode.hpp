#ifndef _MODE_HPP_
#define _MODE_HPP_
#include <string>

enum Type
{
    USER_MODE,
    CHANNEL_MODE
};
#define O 0
#define N 1
#define T 2
#define S 3

class Mode
{
public:
    Mode(int type)
    {
        if (type)
            mode = 15;
        else
            mode = 1;
    }

    void ChangeChannelMode(std::string mode)
    {
        char oper = mode[0];
        if (!(oper == '+' || oper == '-'))
        // error를 내야할 까?
            return ;
        for (unsigned int i = 1 ; i < mode.size() ; ++i)
        {
            if (oper == '+')
            {
                switch (oper) {
                case 'n':
                    this->mode |= (1 << N);
                    break;
                case 't':
                    this->mode |= (1 << T);
                    break;
                case 's':
                    this->mode |= (1 << S);
                    break;
                }
            }
            else {
                switch (oper) {
                case 'n':
                    this->mode &= ~(1 << N);
                    break;
                case 't':
                    this->mode &= ~(1 << T);
                    break;
                case 's':
                    this->mode &= ~(1 << S);
                    break;
                }
            }
        }
    }

    std::string getChannelMode(void)
    {
        std::string mode = "";
        if (this->mode == (1 << S))
            mode += "s";
        if (this->mode == (1 << T))
            mode += "t";
        if (this->mode == (1 << N))
            mode += "n";
        if (this->mode == (1 << O))
            mode += "o";
        return mode;
    }

    void ChangeUserMode(std::string mode)
    {
        char oper = mode[0];
        if (!(oper == '+' || oper == '-'))
        // error를 내야할 까?
            return ;
        for (unsigned int i = 1 ; i < mode.size() ; ++i)
        {
            if (oper == '+')
            {
                switch (oper) {
                case 'n':
                    this->mode |= (1 << N);
                    break;
                case 't':
                    this->mode |= (1 << T);
                    break;
                case 's':
                    this->mode |= (1 << S);
                    break;
                }
            }
            else {
                switch (oper) {
                case 'o':
                    this->mode &= ~(1 << O);
                    break;
                }
            }
        }
    }

private:
    Mode(void);
    unsigned short  mode;
    bool            modeType;
};

#endif