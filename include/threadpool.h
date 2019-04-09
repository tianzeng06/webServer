/*************************************************************************
    > File Name: threadpool.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 20时11分51秒
 ************************************************************************/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "mutex.h"
#include "cond.h"
#include "task.h"


class ThreadPool
{
	private:
		int pool_size;
		Mutex mutexs;
		Cond conds;
		vector<pthread_t> threads;
		queue<Task *> tasks;
		bool pool_state;

		void destroy();
	public:
		ThreadPool();
		ThreadPool(int size):pool_size(size){}
		~ThreadPool();
		void excute();
		int  initializer();
		void add_task(Task *t);
};
#endif
