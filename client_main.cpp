#include "SocketClient.h"
#include <opencv2/opencv.hpp>
using namespace std;

int main(){
    SocketClient client = SocketClient();
    client.create();
    string recv_str;

    if(client.connect("127.0.0.1",9948)){
        // Mat image = imread("dog.jpg",0);
        
        client.sendNumber(500);
        // vector<string> s_v=client.recv();
		// cout<<s_v[0]<<endl;
        cout<<"why"<<endl;
		cout << "received: " << endl;

		// client.sendImage(image);

    }

    return 0;

}