/*************************************************************************
    > File Name: data.cpp
    > Author: Chen Tianzeng
    > Mail: 971859774@qq.com 
    > Created Time: 2019年04月08日 星期一 19时01分19秒
 ************************************************************************/
/*
#ifndef DATA_H
#define DATA_H

//线程池数量，是否开启
const int DEFAULT_SIZE=10;
bool STOP=true;
bool START=false;

//http状态码
#define  CONTINUE 100;//收到了请求的起始部分，应该继续请求

#define  OK 200//服务器成功处理请求
#define  ACCEPTED 202;//已接受请求，服务器尚未处理

#define  MOVED 301;//URL已经移走，响应应该包含location url
#define  FOUND 302;//URL临时移走
#define  SEEOTHER 303;//告诉客户应用另一个URL获取资源，响应应包含location URL
#define  NOTMODIFIED 304;//资源未发生变化

#define  BADREQUEST 400;//客户发送一条异常URL
#define  FORBIDDEN 403;//服务器拒绝请求
#define NOTFOUND 404;//URL未找到

#define  ERROR 500;//服务器出错
#define  NOIMPLEMENTED 501;//服务器不支持所请求的某个功能
#define  BADGATEWAY 502;//作为代理网关使用的服务器遇到了来自响应连上的无效响应
#define  SRVUNAVAILABLE 503;//服务器目前无法提供请求服务，过一段再相应
*/
char ok[]="OK";
char bad_request[]="Bad Request";
char forbidden[]="Forbiddend";
char not_found[]="Not Found";
char no_implemented[]="No Implemented";

