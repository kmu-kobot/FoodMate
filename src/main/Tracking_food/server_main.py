#!/usr/local/bin/python3
#-*- coding: utf-8 -*-
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
        TCP_PORT = 10008
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((TCP_IP, TCP_PORT))
        self.server.listen(1)

    def make_child_process(self):
        pid = os.fork()

        # 1.... Child process
        if pid == 0:
            subprocess.call(["./ResultFile"])


        # 2.... Parent process
        else:
            limit = 0
            result = [] # 결과값을 담는 리스트
            while True:  # server 의 while
                # .......... 2. Client 접속 성공 & data 받기
                conn, addr = self.server.accept()
                length = self.recvall(conn, 16)
                print("들어오는 데이터", length)
                if length.d:   # .......... 3. 이미지 데이터,  받은 이미지를 저장한다.
                    stringData = self.recvall(conn, int(length))

                    data = numpy.fromstring(stringData, dtype='uint8')
                    decimg = cv2.imdecode(data, 1)
                    print("hehehehe")
                    # 3. keras로 음식 맞추기
                    answer = self.guess.realGuess(decimg)  # 일단 급식판에 무슨 음식이 있는지 학습
                    result.append(answer)
                else:
                   # 숫자의 경우
                    mark = length.split(b'\n\b\n\b')[0].decode()
                    limit = int(mark)  # 들어올 총 이미지의 개수를 update
                    print("들어올 이미지수입니다", mark)

                if len(result) == limit:
                    # 4. 도출된 답 전송하기
                    result = "\n\b\n\b".join(result)
                    conn.send(result.encode('utf-8'))
                    result = []  # 다음 사진을 위해 비우기
            exit(0)

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

if __name__ == '__main__':
    main = Main()
    main.make_child_process()