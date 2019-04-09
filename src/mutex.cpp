/*************************************************************************
    > File Name: mutex.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 17时42分57秒
 ************************************************************************/

#include "mutex.h"

Mutex::Mutex()
{
	is_locked=false;
	pthread_mutex_init(&mutex,nullptr);
}
Mutex::~Mutex()
{
	while(is_locked);
	unlock();
	pthread_mutex_destroy(&mutex);
}
void Mutex::lock()
{
	pthread_mutex_lock(&mutex);
	is_locked=true;
}
void Mutex::unlock()
{
	pthread_mutex_unlock(&mutex);
	is_locked=false;
}
pthread_mutex_t *Mutex::get_mutex_ptr()
{
	return &mutex;
}
