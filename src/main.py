#!/usr/local/bin/python3
# coding=utf-8

# Copyright (c) 2020 KOBOT kobot9@naver.com.
# All rights reserved.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import cv2
import os
import numpy as np
import time
from board import Board
# from guess import Guess
from playsound import playsound
from sound import Sound
from tracker import Tracker
import os
from threading import Thread
import Jetson.GPIO as GPIO



class MainDish:

    def __init__(self):

        self.button_pin = 33

        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(self.button_pin, GPIO.IN) 
        GPIO.add_event_detect(self.button_pin, GPIO.FALLING)
	
        # img 폴더 안에 있던 이미지 모두 삭제
        for file in os.scandir('../data/img'):
            os.remove(file.path)

        # 웹캠 사용해서 식판 스캔
        self.cap = cv2.VideoCapture(-1) 
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        ret, self.img = self.cap.read()
        cv2.imwrite('../data/img/Scanned_image.jpg',self.img, params=[cv2.IMWRITE_JPEG_QUALITY,70])
        self.cap.release()

        # 젓가락의 위치
        self.Cx, self.Cy = 0, 0

        # keras 미리 로딩
        # self.guess = Guess()

        # 모든 클래스의 객체생성
        # 1. 급식판 좌표

        self.board = Board()
        self.board.get_target_area()
        self.board.board_frgm_board()

        # 2. 추적할 파란색 범위
        #trackBlue = Track_blue(self.img)
        #self.hsv, self.lower_blue1, self.upper_blue1, self.lower_blue2, self.upper_blue2,\
        #                    self.lower_blue3, self.upper_blue3 = trackBlue.find_target()
        # 3. keras로 음식 맞추기 
        # self.guess.realGuess()  # 일단 급식판에 무슨 음식이 있는지 학습
        # self.answer = self.guess.matchFood(self.Cx, self.Cy, self.board.box_x, self.board.box_y)


    def goingOn(self):  # 여기 callback으로 matchfood 넣을지 고민
        t = Tracker()
        Tracker_thread = Thread(target = t.track_point) 
        Tracker_thread.start()
        while (True):
            if GPIO.event_detected(self.button_pin):

                self.Cx , self.Cy = t.get_track_point()
                # 젓가락의 위치랑 이미지의 픽셀값안에 있나 확인해서 음식명 출력
                print(self.Cx , self.Cy)
                # self.answer = self.guess.matchFood(self.Cx, self.Cy, self.board.box_x, self.board.box_y)
                print(self.answer)
                sound = Sound(self.answer)

                GPIO.remove_event_detect(self.button_pin)
                time.sleep(0.3)
                GPIO.add_event_detect(self.button_pin, GPIO.FALLING)



if __name__ == '__main__':
    sound = Sound("scan_start")
    maindish = MainDish() 
    sound = Sound("scan_end")
    maindish.goingOn()
