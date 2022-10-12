/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Singleton.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:29:55 by mgo               #+#    #+#             */
/*   Updated: 2022/10/10 13:30:04 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Singleton.hpp"
#include <unistd.h>

Singleton::Singleton(void)
    : ptr_sv_(NULL) {}

Singleton::~Singleton(void) {}

void			Singleton::setServerPtr(Server* ptr_sv)
{
    ptr_sv_ = ptr_sv;
}

Server*			Singleton::getServerPtr(void)
{
    return ptr_sv_;
}
