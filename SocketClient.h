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

    // initialization
    bool create();

    bool connect(const :string host, const int port);

    // data transmission
    bool send(Mat &mat,const string mark) const;

    int recv(string &) const;
    bool is_valid();

private:
    int client_socket;
    struct sockaddr_in server_addr;
};