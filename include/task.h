/*************************************************************************
    > File Name: task.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 19时04分20秒
 ************************************************************************/

#pragma once
#include "parse.h"

const long long TIMEOUT=1000*60*4;
#define ONEK 1024
#define ONEM 1024*ONEK
#define ONEG 1024*ONEM
/*
enum TYPE
{
	HEART,DATA
};
typedef struct PACKET_HEAD
{
	TYPE type;
	char s[500];
}HEAD;
*/
class Task
{
	private:
		int fd;//套接字
		int pos;//要读取得位置
	public:
		Task(int f,int p):fd(f),pos(p){}
		void run(const pthread_t tid);
};
