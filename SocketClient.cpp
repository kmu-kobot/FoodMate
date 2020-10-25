#include "SocketClient.h"
#include <iostream>
#include <cstddef>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string.h>

using namespace cv;
using namespace std;

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

// bool SocketClient::send(Mat &mat, const string mark) const
// {   
//     if(!mat.data){
//         cout<<"mat.data not found\n"<<endl;
//         return -1;
// 	}

//     mat = mat.reshape(0,1);
//     int imgSize = mat.total()*mat.elemSize();

// 	string matAsString(mat.datastart, mat.dataend);
// 	//mark-> '\n\b\n\bsave'or '\n\b\n\bend' or '\n\b\n\b'
// 	int status = ::send(client_socket, (matAsString+mark).c_str(), imgSize, 0); // send
// 	// int status = ::send(client_socket, img.data, imgSize, 0); // send

// 	if(status == -1) return false;
// 	else
// 		return true;
// }
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
		string(size_message_length_ - length_str.length(), '0') + length_str;

	send(client_socket, message_length.c_str(), size_message_length_, 0);
	send(client_socket, buf.data(), length, 0);
	
	
	// send(client_socket, "\n\b\n\bsave", 9, 0);
	// string s ="";
	// const char *inputD = (const char*)(img.data);
	// Size imageSiz = img.size(); //cv::Size
	// int w = imageSiz.width;
	// int h = imageSiz.height;
	// int ch = img.channels();
	// int total_length= w*h*ch;
	// s.append(inputD, w*h*ch);
	// // send(client_socket, (s.append("\n\b\n\bsave")).c_str(),total_length+9,0);
	// send(client_socket, s,total_length+9,0);
}


   

std::vector<std::string> SocketClient::recv() const
{
	char buf[MAXRECV + 1];

	std::vector<std::string> results;
	memset(buf, 0, MAXRECV + 1); // MAXRECV +1 만큼 0으로 채우고
 
	char* ptr = strtok(buf,"\n\b\n\b");
	int recv_length = ::recv(client_socket, buf, MAXRECV, 0);

	if(recv_length == -1)
	{
		cout << "status == -1 errno == " << errno << " in SocketClient::recv\n";
		// return 0; 
	}
	else if(recv_length== 0)
	{
		// return 0;
	}
	else
	{
		// char* ptr = strtok(buf,"\n\b\n\b");
		while(ptr!=NULL){
			std::string str;
			str = ptr;
			results.push_back(str);
			ptr = strtok(NULL," ");

		}
		return results; // stirng 벡터 리턴
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
void SocketClient::close_socket(){
	close(client_socket);
}
