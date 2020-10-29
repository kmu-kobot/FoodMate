CC = g++
CFLAGS = -std=c++11
SRCS = Tracker.cpp Matcher.cpp Sound.cpp SocketClient.cpp Board.cpp ThDetectRecognizer.cpp ThTracker.cpp main.cpp  
PROG = ResultFile
LDFLAGS =-lpthread

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(LDFLAGS)
