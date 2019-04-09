/*************************************************************************
    > File Name: cond.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 17时25分25秒
 ************************************************************************/
#pragma once
#include "head.h"
class Cond
{
	private:
		pthread_cond_t cond;
	public:
		Cond();
		~Cond();
		void wait(pthread_mutex_t *mutex);
		void signal();
		void broadcast();
};
