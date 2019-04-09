/*************************************************************************
    > File Name: tool.h
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年04月06日 星期六 10时19分59秒
 ************************************************************************/

//工具类
#ifndef TOOL_H
#define TOOL_H
#include "head.h"
#include "data.h"

extern string http_docroot;
extern string http_domain;
const int  HTTP_DOCROOT=1;
const int  HTTP_DOMAIN=2;

class Server;
class Tool
{
	public:
		//得到系统时间
		//返回值类型:Sat Apr 6 10:31:05 2019
		string get_time();

		//根据http请求中的url和配置文件中的docroot配置真正的url
		//返回值类型:真正的url
		string get_real_url(const string &url);

		//测试文件是否存在
		//参数:path为绝对文件路径+文件名
		bool file_is_existed(const char *path)
		{
			int res=open(path,O_RDONLY);
			close(res);
			return res>0?true:false;
		}

		//获得文件长度
		//参数:path为绝对路径名+文件名
		int get_file_length(const char *path);

		//获得文件最后修改时间
		//参数:path为绝对路径名+文件名
		string get_file_modified_time(const char *path);

		//初始化keyword_map
		void init_keyword_map();
		
		//解析配置文件
		//参数:path为绝对路径+文件名
		bool parse_config(const char *path);

		//设置套接字SO_RCVTIMEO
		void set_rcv_timeo(const int sockfd,const int sec,const int usec);

		//设置套接字SO_SNDTIMEO选项
		void set_snd_timeo(const int sockfd,const int sec,const int usec);

		//根据服务名和协议获得端口号
		struct servent *web_getservbyname(const char *name,const char *proto);

		//根据http状态码返回信息
		char *get_state_by_http(const int http_code);

		friend class Server;
		friend class Parse;
};
#endif
