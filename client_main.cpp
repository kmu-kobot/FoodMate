#include "SocketClient.h"
#include <opencv2/opencv.hpp>
using namespace std;

int main(){
    SocketClient client = SocketClient();
    client.create();
    string recv_str;
    if(client.connect("127.0.0.1",9001)){
        Mat image = imread("dog.jpg",0);

        string recv_str;
		client.recv(recv_str);
		cout << "received: " << recv_str << endl;

		client.sendImage(image);
    }

    return 0;

}