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

    bool connect(const string host, const int port);

    // data transmission
    void sendImage(Mat img) const;

    std::vector<std::string> recv() const;
    bool is_valid();

    void close_socket();

private:
    int client_socket;
    struct sockaddr_in server_addr;
    const int size_message_length_ = 16;  // Buffer size for the length
};