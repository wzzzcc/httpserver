//使用封装的tcpsocket类实现http服务端程序
//1.创建套接字
//2.绑定地址信息
//3.开始监听
//while(1){
//4.获取新连接---一个服务器不可能只跟一个客户端通信
//5.通过新连接接收数据
//6.通过新连接发送数据
//}
//7.关闭套接字
//
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "tcp_socket.hpp"

int main(int argc,char* argv[])
{
	if(argc!=3){
		printf("em:./tcp_srv host_ip host_port\n");
		return -1;
	}
	std::string ip=argv[1];
	uint16_t port=atoi(argv[2]);

	TcpSocket lst_sock;
	//if(lst_sock.Socket()==false){return -1;}
	//#define CHECK_RET(q) if((q)==false){return -1;}
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip,port));
	CHECK_RET(lst_sock.Listen());
	while(1){
		TcpSocket newsock;
		bool ret=lst_sock.Accept(&newsock);
		if(ret==false){
			continue;//服务端不会因为一次获取的失败而退出,而是继续获取下一个
		} 
		std::string buf;
		newsock.Recv(&buf);
		std::cout<<"http req:["<<buf<<"]\n";

		std::string body="<html><body><h1>Hello World</h1></body></html>";
		std::string blank="\r\n";
		std::stringstream header;//可以将数字转化成字符串写进去
		header<<"Content-Length:"<<body.size()<<"\r\n";
		header<<"Content-Type:text/html\r\n";
		header<<"Location:http://www.baidu.com/\r\n";//将请求重定向到百度
		std::string first="HTTP/1.1 302 Found\r\n";

		newsock.Send(first);//发送首行信息
		newsock.Send(header.str());//发送头部信息
		newsock.Send(blank);//发送空行
		newsock.Send(body);//发送正文
		newsock.Close();
	}
	lst_sock.Close();
	return 0;
}
