#include "SocketClient.h"
#include <iostream>
#include <cstddef>
#include <opencv2/opencv.hpp>

using namespace cv;

SocketClient::SocketClient():client_socket(-1)
{
    memset(&server_addr, 0, sizeof(server_addr));
}

SocketClient::~SocketClient(){
    if  (is_valid()){
        ::close(client_socket);
    }
}
bool SocketClient::create()
{
	client_socket = socket(PF_INET, SOCK_STREAM, 0);

	if(!is_valid())
		return false;

	int on = 1;
	if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)))
		return false;

	return true;
}

bool SocketClient::is_valid(){
    return client_socket !=-1;
}

bool SocketClient::send(Mat &mat, const std::string mark) const
{   
    if(!mat.data)[
        std::cout<<"mat.data not found\n"<<std::endl;
        return -1;
    ]

    mat = mat.reshape(0,1);
    int imgSize = mat.total()*mat.elemSize();

	std::string matAsString(mat.datastart, mat.dataend);
	//mark-> '\n\b\n\bstart'or '\n\b\n\bend' or '\n\b\n\b'
	int status = ::send(client_socket, matAsString+mark, imgSize, 0); // send
	// int status = ::send(client_socket, img.data, imgSize, 0); // send

	if(status == -1) return false;
	else
		return true;
}

int SocketClient::recv(std::string& s) const
{
	char buf[MAXRECV + 1];

	s = "";

	memset(buf, 0, MAXRECV + 1); // MAXRECV +1 만큼 0으로 채우고
 

	int recv_length = ::recv(client_socket, buf, MAXRECV, 0);

	if(recv_length == -1)
	{
		std::cout << "status == -1 errno == " << errno << " in SocketClient::recv\n";
		return 0; 
	}
	else if(recv_length== 0)
	{
		return 0;
	}
	else
	{
		s = buf; // 
		return recv_length; // \n\b\n\b는 밖에서 분리하는건?
	}

	//std::string recv_str;
	// conn_sock.recv(recv_str);
	// std::cout << "recved: " << recv_str << std::endl;


}


bool SocketClient::connect(const std::string host, const int port)
{
	if(!is_valid()){
        return false;
    }
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	int status = inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

	if(errno == EAFNOSUPPORT)
		return false;

	status = ::connect(client_socket, (sockaddr *)&server_addr, sizeof(server_addr));

	if(status == 0)
		return true;
	else
		return false;
}
