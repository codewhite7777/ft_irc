#include "ModeClass.hpp"
#include "utils.hpp"
#include <vector>
#include <string>
#define STRING std::string
#define VECTOR std::vector
Mode::Mode(STRING input_mode)
	: mode(1)
{
	char oper = input_mode[0];
	for (unsigned int i = 1 ; i < input_mode.size() ; ++i)
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
			default:
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
			default:
				break;
			}
		}
	}
}

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

void Mode::ChangeChannelMode(STRING chan_name, STRING mode)
{
	char oper = mode[0];

	if (IsProperMode(mode) == false)
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
			default:
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
			default:
				break;
			}
		}
	}
}

void Mode::ChangeChannelMode(STRING chan_name, STRING mode, STRING param)
{
	char oper = mode[0];

	if (IsProperMode(mode) == false)
		return ;
	for (unsigned int i = 1 ; i < mode.size() ; ++i)
	{
		if (oper == '+')
		{
			switch (oper) {
			case 'o':
					this->mode |= (1 << O);
					break;
			case 'n':
					this->mode |= (1 << N);
					break;
			case 't':
					this->mode |= (1 << T);
					break;
			case 's':
					this->mode |= (1 << S);
					break;
			default:
				break;
			}
			
		}
		else {
			switch (oper) {
			case 'o':
					this->mode &= ~(1 << O);
					break;
			case 'n':
					this->mode &= ~(1 << N);
					break;
			case 't':
					this->mode &= ~(1 << T);
					break;
			case 's':
					this->mode &= ~(1 << S);
					break;
			default:
				break;
			}
		}
	}
}

void Mode::ClassifyMode(STRING input)
{
	VECTOR<STRING> inputSplit = split(input, ' ');
	int length = inputSplit.size();

	STRING channel_name = inputSplit[0];
	STRING input_mode	= inputSplit[1];
	
	// MODE #name +o nick
	if (length  == 3)
	{
		STRING target = inputSplit[2];
		ChangeChannelMode(channel_name, input_mode, target);
	}
	// MODE #name +s
	else if (length == 2)
		ChangeChannelMode(channel_name, input_mode);
	else
	 	return ;	
}
