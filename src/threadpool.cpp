/*************************************************************************
    > File Name: threadpool.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 20时20分41秒
 ************************************************************************/

#include "threadpool.h"

const int DEFAULT_SIZE=10;
bool STOP=false;
bool START=true;

ThreadPool::ThreadPool():pool_size(DEFAULT_SIZE),pool_state(START){}

ThreadPool::~ThreadPool()
{
	if(pool_state!=STOP)
		destroy();
}
void ThreadPool::destroy()
{
	mutexs.lock();
	pool_state=STOP;
	mutexs.unlock();
	conds.broadcast();//唤醒阻塞的线程，告诉他要结束使命了
	
	for(int i=0;i<pool_size;++i)
	{
		void *result;
		int res=pthread_join(threads.at(i),&result);
		cout<<threads.at(i)<<" thread exit,function return:"<<res<<"->"<<strerror(res)<<endl;
		conds.broadcast();//防止第一次没有完全唤醒线程，不过应该能完全唤醒
	}
	return ;
}
void *handle(void *args)
{
	cout<<"thread start:"<<pthread_self()<<endl;
	ThreadPool *tp=(ThreadPool *)args;
	tp->excute();
	//cout<<"pthread end:"<<pthread_self()<<endl;
	return nullptr;
}
int ThreadPool::initializer()
{
	pool_state=START;
	for(int i=0;i<pool_size;++i)
	{
		pthread_t pid;
		int res=pthread_create(&pid,nullptr,handle,(void *)this);
		if(res!=0)
		{
			cerr<<"pthread_create() error:"<<strerror(res)<<endl;
			return -1;
		}
		threads.push_back(pid);
	}
	return 0;
}
void ThreadPool::excute()
{
	Task *t=nullptr;
	while(true)
	{
		mutexs.lock();
		while((pool_state!=STOP)&&(tasks.empty()))
			conds.wait(mutexs.get_mutex_ptr());
		
		if(pool_state==STOP)
		{
			mutexs.unlock();
			pthread_exit(nullptr);
		}

		t=tasks.front();
		tasks.pop();
		t->run(pthread_self());
		delete t;
		mutexs.unlock();
	}
	return ;
}
void ThreadPool::add_task(Task *t)
{
	mutexs.lock();
	tasks.push(t);
	mutexs.unlock();
	conds.signal();
}
