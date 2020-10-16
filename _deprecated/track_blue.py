# coding=utf-8
import cv2
import numpy as np


class Track_blue:
    def __init__(self, image):
        self.img = image.copy()

    def findBlue(self):  # x, y는 파란포인터의 위치
        Cx, Cy = 0, 0
        return Cx, Cy

    def nothing(self): # 트랙바때매 생성
        pass

    def find_target(self):
        hsv, lower_blue1, upper_blue1, lower_blue2, upper_blue2, lower_blue3, upper_blue3 = 0,0,0,0,0,0,0

        # 젓가락과 숟가락의 색 값 
        color = [210, 100, 80]

        one_pixel = np.uint8([[color]])
        hsv = cv2.cvtColor(one_pixel, cv2.COLOR_BGR2HSV)
        hsv = hsv[0][0]

        # HSV 색공간에서 마우스 클릭으로 얻은 픽셀값과 유사한 필셀값의 범위를 정합니다.
        if hsv[0] < 10:
            lower_blue1 = np.array([hsv[0] - 10 + 180, 30, 30])
            upper_blue1 = np.array([180, 255, 255])
            lower_blue2 = np.array([0, 30, 30])
            upper_blue2 = np.array([hsv[0], 255, 255])
            lower_blue3 = np.array([hsv[0], 30, 30])
            upper_blue3 = np.array([hsv[0] + 10, 255, 255])

        elif hsv[0] > 170:
            lower_blue1 = np.array([hsv[0], 30, 30])
            upper_blue1 = np.array([180, 255, 255])
            lower_blue2 = np.array([0, 30, 30])
            upper_blue2 = np.array([hsv[0] + 10 - 180, 255, 255])
            lower_blue3 = np.array([hsv[0] - 10, 30, 30])
            upper_blue3 = np.array([hsv[0], 255, 255])

        else:
            lower_blue1 = np.array([hsv[0], 30, 30])
            upper_blue1 = np.array([hsv[0] + 10, 255, 255])
            lower_blue2 = np.array([hsv[0] - 10, 30, 30])
            upper_blue2 = np.array([hsv[0], 255, 255])
            lower_blue3 = np.array([hsv[0] - 10, 30, 30])
            upper_blue3 = np.array([hsv[0], 255, 255])
        return hsv, lower_blue1, upper_blue1, lower_blue2, upper_blue2, lower_blue3, upper_blue3
