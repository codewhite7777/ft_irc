#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <string>
#define STRING std::string
#define END "\r\n"

class Message {
// Member Function
public:		Message(STRING userFullInfo, STRING replyNumber, STRING param);
public:		Message(STRING userFullInfo, STRING replyNumber, STRING param, STRING msg);
public:		Message(STRING userFullInfo, STRING replyNumber, STRING param1, STRING param2, STRING msg);

// get Now Message
public:		STRING getReplyMessage();

// Member Function
private:	Message(void);

// Member variable
private:
	STRING replyMessage;
};

#endif
