#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int MAXHOSTNAME = 200;
const int MAXCONNECTION = 5;
const int MAXRECV = 1024;

class SocketClient{
public:
    SocketClient();
    virtual ~SocketClient();

    // socket initialization
    bool create(); 
    // socket connect
    bool connect(const string host, const int port);
    // sending imageData
    void sendImage(Mat img) const;
    // sending number
    void sendNumber(int number) const;
    // receive vector that includes dishes from server	
    std::vector<std::string> recv() const;
    // valid check
    bool is_valid();

    void close_socket();

private:
    int _client_socket;
    struct sockaddr_in _server_addr;
    const int _size_message_length_ = 16;  // Buffer size for the length
};
