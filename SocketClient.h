#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTION = 5;
const int MAXRECV = 1024;

class SocketClient{
public:
    SocketClient();
    virtual ~SocketClient();

    // initialization
    bool create();

    bool connect(const std::string host, const int port);

    // data transmission
    bool send(const std::string) const;

    int recv(std::string &) const;
    bool is_valid();

private:
    int client_socket;
    struct sockaddr_in server_addr;
};