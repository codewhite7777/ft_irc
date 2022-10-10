#ifndef SINGLETON_HPP
# define SINGLETON_HPP

# include <unistd.h>

class Server;

class Singleton
{
public:
    static  Singleton&  getInstance()
    {
        static Singleton    instance;

        return instance;
    }

	void					setServerPtr(Server* ptr_sv);
	Server*					getServerPtr(void);

private:
	Singleton(void);
    Singleton(const Singleton&);
    void    operator=(Singleton const&);
    ~Singleton(void);

	Server*				ptr_sv_;
};

#endif