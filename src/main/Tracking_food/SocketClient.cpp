#include "SocketClient.h"
#include <iostream>
#include <cstddef>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

SocketClient::SocketClient():_client_socket(-1)
{
    memset(&_server_addr, 0, sizeof(_server_addr));
}

SocketClient::~SocketClient(){
    if  (is_valid()){
        ::close(_client_socket);
    }
}
bool SocketClient::create()
{
	_client_socket = socket(PF_INET, SOCK_STREAM, 0);

	if(!is_valid())
		return false;

	int on = 1;
	if (setsockopt(_client_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)))
		return false;

	return true;
}

bool SocketClient::is_valid(){
    return _client_socket !=-1;
}

void SocketClient::sendImage(Mat img) const // image send success but not
{   
    // if(!img.data){
    //     cout<<"mat.data not found\n"<<endl;
    //     return -1;
	// }

	int pixel_number = img.rows * img.cols / 2;
	vector<uchar> buf(pixel_number);
	imencode(".jpg", img, buf);

	int length = buf.size();
	string length_str = to_string(length);
	string message_length =
	 	string(_size_message_length_ - length_str.length(), '0') + length_str;

	send(_client_socket, message_length.c_str(), _size_message_length_, 0);
	send(_client_socket, buf.data(), length, 0);
}

void SocketClient::sendNumber(int number) const
{
	string tmp_string = to_string(number);
	string mark = "\n\b\n\b";
	tmp_string = tmp_string + mark;
	
	char ch[16]; 
	strcpy(ch, tmp_string.c_str());

	send(_client_socket, ch, 16, 0);
}

std::vector<std::string> SocketClient::recv() const
{
	char buf[MAXRECV + 1];

	std::vector<std::string> results;
	memset(buf, 0, MAXRECV + 1); // MAXRECV +1 만큼 0으로 채우고
 	
	int recv_length = ::recv(_client_socket, buf, MAXRECV, 0);
	char* ptr = strtok(buf,"\n\b\n\b");
	if(recv_length == -1)
	{
		cout << "status == -1 errno == " << errno << "l in SocketClient::recv\n";
		// return 0; 
	}
	else if(recv_length== 0)
	{
		// return 0;
	}
	else
	{
		while(ptr!=NULL){
			std::string str;
			str = ptr;
			results.push_back(str);
			ptr = strtok(NULL,"\n\b\n\b");
		}
		return results; // stirng 벡터 리턴
	}
}

bool SocketClient::connect(const std::string host, const int port)
{
	if(!is_valid()){
        return false;
    }
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(port);

	int status = inet_pton(AF_INET, host.c_str(), &_server_addr.sin_addr);

	if(errno == EAFNOSUPPORT)
		return false;

	status = ::connect(_client_socket, (sockaddr *)&_server_addr, sizeof(_server_addr));

	if(status == 0)
		return true;
	else
		return false;
}
void SocketClient::close_socket(){
	close(_client_socket);
}
