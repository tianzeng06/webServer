/*************************************************************************
    > File Name: mutex.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 17时41分00秒
 ************************************************************************/

#pragma once
#include "head.h"
class Mutex
{
	private:
		pthread_mutex_t mutex;
		volatile bool is_locked;
	public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
		pthread_mutex_t *get_mutex_ptr();
};
