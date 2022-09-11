#include "ModeClass.hpp"
#define STRING std::string

Mode::Mode(void)
	: mode((1 << S) | (1 << T) | (1 << N) | (1 << O))
{ }

bool Mode::IsProperMode(STRING input)
{
	char oper = input[0];

	switch (oper) {
	case '+':
		return (true);
	case '-':
		return (true);
	default:
		return (false);
	}
}

void Mode::ChangeChannelMode(STRING input)
{
	char oper = input[0];

	if (IsProperMode(input) == false)
		return ;

	for (unsigned int i = 1 ; i < input.size() ; ++i)
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

// 4가지 기능
// // 
// // mode
// // join
// // invite

// // part
// // privmsg Notice
// snto
// // nick : 
// // user
// // pass