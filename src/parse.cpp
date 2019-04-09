/*************************************************************************
    > File Name: parse.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年04月06日 星期六 22时37分16秒
 ************************************************************************/

#include "parse.h"

//打印http_head
void Parse::print_http_head(const http_head &head)
{
	if(!head.empty())
	{
		http_head::const_iterator it=head.begin();
		for(;it!=head.end();++it)
			cout<<it->first<<" : "<<it->second<<endl;
	}
	return ;
}

void Parse::print_http_head_t(const http_head_t* head_t)
{
	if(head_t==nullptr)	
	{
		cerr<<"head_t is nullptr"<<endl;
		return ;
	}
	cout<<head_t->method<<" "<<head_t->url<<" "<<head_t->ver<<endl;
	print_http_head(head_t->head);
	cout<<head_t->body<<endl;
	return;
}

http_head_t *Parse::new_http_head_t()
{
	http_head_t *head_t=(http_head_t*)new http_head_t;
	if(head_t==nullptr)
	{
		cerr<<"new http_head_t fail"<<endl;
		exit(-1);
	}

	return head_t;
}

void Parse::del_http_head_t(http_head_t * head_t)
{
	if(head_t==nullptr)
	{
		cerr<<"head_t is nullptr,not need to release heat_t"<<endl;
		return ;
	}

	delete head_t;
	head_t=nullptr;
	return;
}

//解析http request
bool Parse::parse_http_request(const string &request,http_head_t *const &head_t)
{
	if(request.empty())
	{
		cerr<<"http request empty"<<endl;
		return false;
	}
	if(head_t==nullptr)
	{
		cerr<<"http head_t is empty"<<endl;
		return false;
	}

	//1.解析请求行
	string rn("\r\n"),rn2("\r\n\r\n");
	size_t pre=0,next=0;

	//查找\r\n
	if((next=request.find(rn,pre))!=string::npos)
	{
		string first_line(request.substr(pre,next-pre));
		pre=next;
		stringstream ss(first_line);
		ss>>head_t->method;
		ss>>head_t->url;
		ss>>head_t->ver;
	}
	else
	{
		cerr<<"request not contain \r\n"<<endl;
		return false;
	}

	//查找\r\n\r\n
	size_t pos=request.find(rn2,pre);
	if(pos==string::npos)
	{
		cerr<<"request not contain \r\n\r\n"<<endl;
		return false;
	}
	
	//2.解析请求头部
	while(true)
	{
		next=request.find(rn,pre+2);
		if(next<=pos)
		{
			string buff(request.substr(pre+2,next-pre-2));

			int i=0;
			//跳过空白字符到达首部关键字位置
			for(;isblank(buff.at(i));++i);

			//到达首部关键字位置
			int start=i;
			for(;buff.at(i)!=':'&&!isblank(buff.at(i));++i);
			string key=buff.substr(start,i-start);

			//到达首部值的起始位置
			for(;(!isalpha(buff.at(i))&&!isdigit(buff.at(i)))||isblank(buff.at(i));++i);
			start=i;
			//next存的是首部的结束位置
			string value(buff.substr(start,next-start));
			head_t->head.insert({key,value});

			pre=next;
		}
		else
			break;
	}
	//3.解析求求数据,一般不存在
	head_t->body=request.substr(pos+4,request.size()-pos-4);

	return true;
}

string Parse::get_value_from_http_head_t(const string &key,const http_head &head)
{
	if(head.empty())
	{
		cerr<<"http_head is empty"<<endl;
		return "";
	}
	if(key.empty())
	{
		cerr<<"key is empty"<<endl;
		return "";
	}

	http_head::const_iterator it=head.find(key);
	if(it==head.end())
	{
		cerr<<"not find value by to key"<<endl;
		return "";
	}
	return it->second;
}

int Parse::handle_http_request(const http_head_t *head_t,string &response)
{
	string status("http/1.1");
	string rn("\r\n");
	stringstream ss;
	Tool tl;

	if(head_t==nullptr)
	{
		ss<<BADREQUEST<<" "<<tl.get_state_by_http(BADREQUEST);
		response=status;
		string t;
		while(ss>>t)
			response.append(" ").append(t);
		response.append(rn).append(rn);
		ss.str("");

		return BADREQUEST;
	}

	string server("Server:ctz-http\r\n");
	string Public("Public: GET,HAND\r\n");
	string content_length("Content-Length: ");
	string content_location("Content-Location: ");
	string last_modified("Last-Modified: ");

	string content_base("Content-Base: ");
	content_base.append(http_domain).append(rn);

	string date("Date: ");
	date.append(tl.get_time()).append(rn);
	
	string method=head_t->method;
	string real_url=tl.get_real_url(head_t->url);
	string ver=head_t->ver;
	
	if(method=="GET"||method=="HEAD")
	{
		if(!tl.file_is_existed(real_url.c_str()))
		{
			ss<<NOTFOUND<<" "<<tl.get_state_by_http(NOTFOUND);
			response=status;
			string t;
			while(ss>>t)
				response.append(" ").append(t);
			response.append(rn).append(server).append(date).append(rn);
			ss.str("");
	
			return NOTFOUND;
		}
		else
		{
			int len=tl.get_file_length(real_url.c_str());
			ss<<OK<<" "<<tl.get_state_by_http(OK);
			response=status;
			string t;
			while(ss>>t)
				response.append(" ").append(t);
			response.append(rn);
			ss.str("");
			
			ss<<len;
			ss>>t;
			response.append(content_length).append(t).append(rn);
			
			response.append(server).append(content_base).append(date);
			
			response.append(last_modified).append(tl.get_file_modified_time(real_url.c_str())).append(rn).append(rn);
		}
		ss.str("");
		return OK;
	}
	else if(method=="PUT")
	{
		ss<<NOIMPLEMENTED<<" "<<tl.get_state_by_http(NOIMPLEMENTED);
		response=status;
		string t;
		while(ss>>t)
			response.append(" ").append(t);
		response.append(rn);
		response.append(server).append(Public).append(date).append(rn);
		
		ss.str("");
		return NOIMPLEMENTED; 
	}
	else if(method=="POST")
	{
		ss<<NOIMPLEMENTED<<" "<<tl.get_state_by_http(NOIMPLEMENTED);
		response=status;
		string t;
		while(ss>>t)
			response.append(" ").append(t);
		response.append(rn);
		response.append(server).append(Public).append(date).append(rn);
		
		ss.str("");
		return NOIMPLEMENTED; 
	}
	else if(method=="DELETE")
	{
		ss<<NOIMPLEMENTED<<" "<<tl.get_state_by_http(NOIMPLEMENTED);
		response=status;
		string t;
		while(ss>>t)
			response.append(" ").append(t);
		response.append(rn);
		response.append(server).append(Public).append(date).append(rn);
		
		ss.str("");
		return NOIMPLEMENTED; 
	}
	else
	{
		ss<<BADREQUEST<<" "<<tl.get_state_by_http(BADREQUEST);
		response=status;
		string t;
		while(ss>>t)
			response.append(" ").append(t);
		response.append(rn).append(rn);

		ss.str("");
		return BADREQUEST;
	}
	return OK;
}
