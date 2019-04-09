/*************************************************************************
    > File Name: tool.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年04月06日 星期六 10时50分02秒
 ************************************************************************/

#include "tool.h"

//解析存储文件的键值
//key=docroot value=/home/ctz/work/web
map<string,int> keyword_map;

string Tool::get_time()
{
	time_t now=time(nullptr);
	struct tm *tm=localtime(&now);
	string res_time=asctime(tm);
	res_time=res_time.substr(0,res_time.size()-1);
	res_time=res_time.append(" GMT");

	return res_time;
}

//根据http请求中的url和配置文件中的docroot构造真正的url
string Tool::get_real_url(const string &url)
{
	size_t n=0;
	string tmp;

	//若包含域名,路径中不应该包含域名，截取不含域名的字符串
	if((n=url.find(http_domain,0))!=string::npos)
		tmp=url.substr(http_domain.size(),url.size()-http_domain.size());
	else
		tmp=url;
	
	string real_url;
	//看配置文件中http_docroot中是否有/
	if(http_docroot.at(http_docroot.size()-1)=='/')
	{
		if(tmp.at(0)=='/')
		{
			real_url=http_docroot;
			real_url.append(tmp.erase(0,1));
		}
		else
		{
			real_url=http_docroot;
			real_url.append(tmp);
		}
	}
	else
	{
		if(tmp.at(0)=='/')
		{
			real_url=http_docroot;
			real_url.append(tmp);
		}
		else
		{
			real_url=http_docroot;
			real_url.append("/").append(tmp);
		}
	}

	return real_url;
}

//获取文件长度
int Tool::get_file_length(const char *path)
{
	struct stat buf;
	int res=stat(path,&buf);
	if(res==-1)
	{
		cerr<<"get file length error: "<<strerror(errno)<<endl;
		exit(res);
	}
	return (int)buf.st_size;
}

//获取文件最后修改时间
string Tool::get_file_modified_time(const char *path)
{
	struct stat buf;
	int res=stat(path,&buf);
	if(res==-1)
	{
		cerr<<"get file moidified error: "<<strerror(errno)<<endl;
		exit(res);
	}
	
	char t[25]={0};
	time_t tt=buf.st_mtim.tv_sec;
	snprintf(t,sizeof(t),"%s",ctime(&tt));
	
	return string(t,strlen(t));
}

//初始化keyword_map
void Tool::init_keyword_map()
{
	keyword_map.insert({"docroot",HTTP_DOCROOT});
	keyword_map.insert({"domain",HTTP_DOMAIN});
}

//解析配置文件，path为绝对路径+文件名
//初始化对对象domain和docroot
bool Tool::parse_config(const char *path)
{
	init_keyword_map();

	fstream in(path,fstream::in);
	if(!in)
	{
		cerr<<"can't open file: "<<path<<endl;
		in.close();
		return false;
	}
	
	string line,key;
	while(getline(in,line))
	{
		stringstream ss;
		ss.str(line);
		ss>>key;
		
		map<string,int>::const_iterator it=keyword_map.find(key);
		if(it==keyword_map.end())
		{
			cerr<<"can't find key"<<endl;
			in.close();
			return false;
		}
		switch(it->second)
		{
			case HTTP_DOCROOT:
				 ss>>http_docroot;
				 break;
			case HTTP_DOMAIN:
				 ss>>http_domain;
				 break;
			default:
			{
				 in.close();
				 return false;
			}
		}
		ss.str("");
	}

	in.close();
	return true;
}
//设置套接字SO_RCVTIMEO
void Tool::set_rcv_timeo(const int sockfd,const int sec,const int usec)
{
	struct timeval time={sec,usec};
	int res=setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&time,sizeof(time));
	if(res<0)
	{
		cerr<<"setsockopt SO_RCVTIMEO error: "<<strerror(errno)<<endl;
		exit(res);
	}
	return ;
}

//设置SO_SNDTIMEO
void Tool::set_snd_timeo(const int sockfd,const int sec,const int usec)
{

	struct timeval time={sec,usec};
	int res=setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&time,sizeof(time));
	if(res<0)
	{
		cerr<<"setsockopt SO_SNDTIMEO error: "<<strerror(errno)<<endl;
		exit(res);
	}
}

struct servent *Tool::web_getservbyname(const char *name,
												const char *proto)
{
	struct servent *res;
	res=getservbyname(name,proto);
	if(res==nullptr)
	{
		cerr<<"getservbyname error:"<<strerror(errno)<<endl;
		exit(-1);
	}
	return res;
}

char *Tool::get_state_by_http(const int http_code)
{
	switch(http_code)
	{
		case OK:
			return ok;
		case BADREQUEST:
			return bad_request;
		case FORBIDDEN:
			return forbidden;
		case NOTFOUND:
			return not_found;
		case NOIMPLEMENTED:
			return no_implemented;
		default:
			break;
	}
	return nullptr;
}
