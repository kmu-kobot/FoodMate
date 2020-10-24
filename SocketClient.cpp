#include "SocketClient.h"
#include <iostream>
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

bool SocketClient::send(const Mat &mat) const
{   
    if(!mat.data)[
        std::cout<<"mat.data not found\n"<<std::endl;
        return -1;
    ]
    mat = mat.reshape(0,1);
    int imgSize = mat.total()*mat.elemSize();
    
	int bytes = ::send(client_socket, mat.data, imgSize, 0);
	if(bytes == -1) return false;
	else
		return true;
}

int SocketClient::recv(std::string& s) const
{
	char buf[MAXRECV + 1];

	s = "";

	memset(buf, 0, MAXRECV + 1);
 

	int status = ::recv(client_socket, buf, MAXRECV, 0);

	if(status == -1)
	{
		std::cout << "status == -1 errno == " << errno << " in SocketClient::recv\n";
		return 0; 
	}
	else if(status == 0)
	{
		return 0;
	}
	else
	{
		s = buf;
		return status;
	}
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
