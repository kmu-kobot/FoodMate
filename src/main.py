# coding=utf-8
import cv2
import os
import numpy as np
import time
from board import Board
from guess import Guess
from playsound import playsound
from sound import Sound
from tracker import Tracker



class MainDish:

    def __init__(self):

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
        self.guess = Guess()

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
        self.guess.realGuess()  # 일단 급식판에 무슨 음식이 있는지 학습
        self.answer = self.guess.matchFood(self.Cx, self.Cy, self.board.box_x, self.board.box_y)


    def goingOn(self):  # 여기 callback으로 matchfood 넣을지 고민
        t = Tracker()
        while (True):
            t.track_point()
            self.Cx , self.Cy = t.get_track_point()
            # 젓가락의 위치랑 이미지의 픽셀값안에 있나 확인해서 음식명 출력
            self.answer = self.guess.matchFood(self.Cx, self.Cy, self.board.box_x, self.board.box_y)
            sound = Sound(self.answer)


            # 파란색 부분 테두리 그리기
            #cv2.rectangle(img_color, (self.x_strt, self.y_strt), (self.x_end, self.y_end), (0, 0, 0), 2)

            # 마스크 이미지로 원본 이미지에서 범위값에 해당되는 영상 부분을 획득합니다.





if __name__ == '__main__':
    sound = Sound("scan_start")
    maindish = MainDish() 
    sound = Sound("scan_end")
    maindish.goingOn()
