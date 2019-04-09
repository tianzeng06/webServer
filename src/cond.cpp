/*************************************************************************
    > File Name: cond.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 17时28分23秒
 ************************************************************************/

#include "cond.h"

Cond::Cond()
{
	pthread_cond_init(&cond,nullptr);
}
Cond::~Cond()
{
	pthread_cond_destroy(&cond);
}
void Cond::wait(pthread_mutex_t *mutex)
{
	pthread_cond_wait(&cond,mutex);
}
void Cond::signal()
{
	pthread_cond_signal(&cond);
}
void Cond::broadcast()
{
	pthread_cond_broadcast(&cond);
}
