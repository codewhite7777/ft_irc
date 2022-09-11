#ifndef _MODECLASS_HPP_
#define _MODECLASS_HPP_

#include <string>

#define O 0
#define N 1
#define T 2
#define S 3

enum  PARAM_TYPE
{
	CHANNEL,
	USER
};
#define STRING std::string
class Mode
{
// 생성자
public:	Mode(void);

// 멤버 함수들
public: bool IsProperMode(STRING input);

void ChangeChannelMode(std::string mode);

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

// Member variable
private:
	unsigned short  mode;
};

#endif
