#include "Singleton.hpp"

Singleton::Singleton(void)
{
    ptr_sv_ = NULL;
}

Singleton::Singleton(const Singleton&) {}

void    Singleton::operator=(Singleton const&) {}

Singleton::~Singleton(void) {}

void			Singleton::setServerPtr(Server* ptr_sv)
{
    ptr_sv_ = ptr_sv;
}

Server*			Singleton::getServerPtr(void)
{
    return ptr_sv_;
}
