CC = g++
CFLAGS = -std=c++11
SRCS = Tracking.cpp Matching.cpp Sound.cpp SocketClient.cpp Board.cpp Consumer1.cpp Consumer2.cpp producer_consumer_semaphore.cpp  
PROG = ResultFile
LDFLAGS =-lpthread

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(LDFLAGS)
