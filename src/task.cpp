/*************************************************************************
    > File Name: task.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年03月29日 星期五 19时17分09秒
 ************************************************************************/

#include "task.h"

void Task::run(const pthread_t tid)
{
	cout<<"thread: "<<tid<<" working"<<endl;
	
	//先读取消息长度
	PACKET_HEAD head;
	int index=0;
	/*while(true)
	{
		int n=recv(fd,&head.len+index,4-index,0);
		if(n<0)
		{
			if(errno==EINTR)
				continue;
			else if(errno==EAGAIN)
				break;
			else if(errno==EWOULDBLOCK)
			{
				cerr<<"recv message len timeout"<<endl;
				close(fd);
			}
			else
			{
				cerr<<"recv message len error"<<endl;
				exit(1);
			}
		}
		else //n>=0
		{
			index+=n;
			if(index==4)
				break;
		}
	}
	cout<<head.len<<endl;*/
	//为http头部分配1M的空间，仿Nginx系统
	char *buff=new char[ONEM];
	memset(buff,'\0',ONEM);
	index=0;
	//读内容
	while(true)
	{
		int n=recv(fd,buff+index,ONEM-1-index,0);
		if(n<0)
		{
			if(errno==EINTR)
				continue;
			else if(errno==EAGAIN)
				break;
			else if(errno==EWOULDBLOCK)
			{
				cerr<<"socket recv error"<<endl;
				delete []buff;
				buff=nullptr;
				return ;
			}
			else
			{
				cerr<<"recv http requset error"<<endl;
				delete []buff;
				buff=nullptr;
				return ;
			}
		}
		else //n>=0
		{
			index+=n;
			if(index==sizeof(head)-4)
				break;
		}
	}
	int len=strlen(buff);
	buff[len]='\0';
	
	http_head_t *head_t=new http_head_t;
	if(index!=0)
	{
		string req(buff,buff+index);
		//http_head_t *head_t=new http_head_t;
		Parse pe;
		Tool tl;
		
		//解析http请求，放在head_t中
		if(!pe.parse_http_request(req,head_t))
		{
			cerr<<"parse http_head_t error"<<endl;
			delete head_t;
			delete []buff;
			head_t=nullptr;
			buff=nullptr;
			close(fd);
			return;
		}
		
		cout<<"parse http request packet is:"<<endl;
		pe.print_http_head_t(head_t);
	
		//处理http请求
		string response;
		int http_code=pe.handle_http_request(head_t,response);
		//exec get_real_url
		cout<<"http response packet is:"<<endl<<response<<endl;

		char out_buf[response.size()];
		copy(response.begin(),response.end(),out_buf);
		if(http_code==BADREQUEST||http_code==NOIMPLEMENTED||
		   http_code==NOTFOUND||
		   (http_code==OK&&head_t->method=="HEAD"))
		{
			int n=0;
			index=0;
			while(true)
			{
				n=send(fd,out_buf+index,response.size()-index,0);
				if(n==0)
					break;
				else
				{
					if(n<0)
					{
						if(errno==EINTR)
							continue;
						else if(errno==EAGAIN||errno==EWOULDBLOCK)
							break;
					}
					else
						index+=n;
				}
			}
		}
		else if(http_code==OK&&head_t->method=="GET")
		{
			int n=0;
			index=0;
			while(true)
			{
				n=send(fd,out_buf+index,response.size()-index,0);
				if(n==0)
					break;
				else
				{
					if(n<0)
					{
						if(errno==EINTR)
							continue;
						else if(errno==EAGAIN||errno==EWOULDBLOCK)
							break;
						else
						{
							cerr<<"send error: "<<strerror(errno)<<endl;
							break;
						}
					}
					else
						index+=n;
				}
			}
			string real_url=tl.get_real_url(head_t->url);
			cout<<"real_url: "<<real_url<<endl;
			int fds=open(real_url.c_str(),O_RDONLY);
			int file_size=tl.get_file_length(real_url.c_str());
			cout<<"file_size is: "<<file_size<<endl;
			
			off_t nwrite=0;
			while(true)
			{
				ssize_t n=sendfile(fd,fds,&nwrite,file_size);
				if(n==-1)
				{
					if(errno==EAGAIN)
						continue;
					else
					{
						cerr<<"sendfile error: "<<strerror(errno)<<endl;
						break;
					}
				}
				if(nwrite<file_size)
					continue;
				else
				{
					cout<<"sendfile ok: "<<nwrite<<endl;
					break;
				}
			}
		}
	}
	
	delete []buff;
	buff=nullptr;
	delete head_t;
	head_t=nullptr;
	cout<<"thread: "<<tid<<" working end"<<endl;
	return;
}
