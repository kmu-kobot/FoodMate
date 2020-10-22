import os, os.path
import time
from guess import Guess  # Keras load
import sys
import socket
import numpy
import cv2
import ctypes  # c++ 파일 사용하기



class Main:
    def __init__(self):

        # 0.... Keras 미리 로딩
        self.guess = Guess()

        # .......... 1. Server  TCP 소켓 열고 수신 대기
        if os.path.exists("/tmp/socket_test.s"):
            os.remove("/tmp/socket_test.s")
        self.server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.server.bind("/tmp/socket_test.s")
        self.server.listen(1)

    def make_child_process(self):
        child_cnt = 0
        pid = 0
        while True:  # Child 프로세스 오류 생길까봐
            if child_cnt == 0:
                # fork 함수는 부모 프로세스에게는 자식프로세스의 PID를 반환하며 자식 프로세스에게는 0을 반환
                pid = os.fork()

            # 1.... Child process
            if pid == 0:
                child_cnt += 1
                self.get_img = self.connect_C_client()  # c++ 클라이언트 생성하기


            # 2.... Parent process
            else:
                img_cnt = 0
                while True:  # server 의 while
                    # .......... 2. Client 접속 성공 & data 받기
                    conn, addr = self.server.accept()
                    length = self.recvall(conn, 16)
                    stringData = self.recvall(conn, int(length))
                    # .......... 3. 이미지 데이터
                    data = numpy.fromstring(stringData, dtype='uint8')
                    decimg = cv2.imdecode(data, 1)
                    # img_cnt += 1
                    # path = '../data/img'
                    # cv2.imwrite(path + '/'+ img_cnt, decimg)
                    # images_notsorted = os.listdir(path)

                    # # 3. keras로 음식 맞추기
                    # self.guess.realGuess()  # 일단 급식판에 무슨 음식이 있는지 학습
                    # self.answer = self.guess.matchFood(self.Cx, self.Cy, self.board.box_x, self.board.box_y)

                    # 4. 도출된 답 전송하기



    # socket 수신 버퍼를 읽어서 반환하는 함수
    def recvall(self, sock, count):
        buf = b''
        while count:
            newbuf = sock.recv(count)
            if not newbuf: return None
            buf += newbuf
            count -= len(newbuf)
        return buf

    def connect_C_client(self):
        pass
        img = None
        return img