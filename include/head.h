/*************************************************************************
    > File Name: head.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 17时21分34秒
 ************************************************************************/
#ifndef HEAD_H
#define HEAD_H

#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <utility>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <ctype.h>
using namespace std;

enum Type
{
	HEART,DATA
};
typedef struct PACKET_HEAD
{
	Type type;
	//int len;
	char s[500];
}PACKET_HEAD;

#endif
