/*************************************************************************
    > File Name: parse.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年04月06日 星期六 22时24分29秒
 ************************************************************************/

#ifndef PARSE_H
#define PARSE_H

#include "tool.h"

extern string http_docroot;
extern string http_domain;

//存的是请求头部的数据
typedef map<string,string> http_head;

typedef struct http_head_t
{
	string method;
	string url;
	string ver;
	
	http_head head;

	string body;
}http_head_t;

class Parse
{
	private:
		//打印http_head内容
		void print_http_head(const http_head &head);
	public:

		//打印http_head_t
		void print_http_head_t(const http_head_t *head_t);

		//分配给http_head_t内存
		http_head_t *new_http_head_t();

		//回收分配给http_head_t的内存
		void del_http_head_t(http_head_t *head_t);

		//解析http请求
		bool parse_http_request(const string &request,http_head_t * const &head_t);
		
		//根据key查找http_head_t中相应的值
		string get_value_from_http_head_t(const string &key,const http_head &head);
		
		//根据解析http_head_t中的数据来处理http请求
		int handle_http_request(const http_head_t * head_t,string &response);
};

#endif
