/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:29:51 by mgo               #+#    #+#             */
/*   Updated: 2022/10/10 13:30:09 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
# define SINGLETON_HPP

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