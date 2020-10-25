CC = g++
CFLAGS = -std=c++11
SRCS = SocketClient.cpp client_main.cpp
PROG = SocketClient

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)