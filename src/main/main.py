import os, os.path
import time
from guess import Guess  # Keras load
import sys
import socket
import numpy
import cv2
import ctypes  # c++ 파일 사용하기

import subprocess


class Main:
    def __init__(self):

        # 0.... Keras 미리 로딩
        self.guess = Guess()

        # .......... 1. Server  TCP 소켓 열고 수신 대기
        TCP_IP = 'localhost'
        TCP_PORT = int(sys.argv[1])
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((TCP_IP, TCP_PORT))
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
                file = 'C:\\Exe\\First Version\\filename.exe'
                subprocess.call([file])

            # 2.... Parent process
            else:
                img_cnt = 0
                while True:  # server 의 while
                    # .......... 2. Client 접속 성공 & data 받기
                    conn, addr = self.server.accept()
                    length = self.recvall(conn, 16)
                    stringData = self.recvall(conn, int(length))
                    endMark = stringData.split(b'\n\b\n\b')[-1].decode()
                    if endMark == "save":
                        # .......... 3. 이미지 데이터,  받은 이미지를 저장한다.
                        data = numpy.fromstring(stringData, dtype='uint8')
                        decimg = cv2.imdecode(data, 1)
                        img_cnt += 1
                        path = '../data/img'
                        cv2.imwrite(path + '/image' + str(img_cnt), decimg)
                    elif endMark == "end":
                        # 3. keras로 음식 맞추기
                        result = self.guess.realGuess()  # 일단 급식판에 무슨 음식이 있는지 학습
                        # 4. 도출된 답 전송하기
                        result = "\n\b\n\b".join(result)
                        conn.send(result.encode('utf-8'))


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