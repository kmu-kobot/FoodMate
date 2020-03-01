import cv2
import numpy as np
import time
from board import Board
from track_blue import Track_blue
from guess import Guess


class MainDish:

    def __init__(self, fname=None):  # 데모할때만 파일이름 넣어주고 실제는 캠으로


        # self.img = cv2.imread(fname)  # 사진 사용
        # self.cap = cv.VideoCapture(0) # 웹캠 사용
        self.cap = cv2.VideoCapture(fname)  # 동영상 사용
        ret, self.img = self.cap.read()

        # 젓가락의 위치
        self.Cx, self.Cy = 0, 0

        # 모든 클래스의 객체생성
        # 1. 급식판 좌표
        board = Board(self.img)
        self.x_strt, self.x_end, self.y_strt, self.y_end = board.edgeOfBoard()
        board.frgm_board(board.edgeOfBoard)  # 급식판 메뉴따기 함수 callback

        # 2. 추적할 파란색 범위
        trackBlue = Track_blue(self.img)
        self.hsv, self.lower_blue1, self.upper_blue1, self.lower_blue2, self.upper_blue2,\
                            self.lower_blue3, self.upper_blue3 = trackBlue.find_target()

        # 3. keras로 음식 맞추기 self로 객체선언 자주 'maindish' 클래스에 의해 호출되어야돼
        self.guess = Guess()
        self.guess.realGuess()  # 일단 급식판에 무슨 음식이 있는지 학습
        self.answer = self.guess.matchFood(self.Cx, self.Cy, board.box_x, board.box_y)




        # # img_result 이름의 윈도우창에 트랙바를 생성
        # cv2.namedWindow('img_result')
        # cv2.createTrackbar('threshold', 'img_result', 0, 255, self.nothing)
        # cv2.setTrackbarPos('threshold', 'img_result', 30)

    def nothing(self):  # 트랙바때매 생성
        pass

    def goingOn(self):  # 여기 callback으로 matchfood 넣을지 고민
        while (True):
            # img_color = self.img.copy()
            time.sleep(0.2)
            ret, img_color = self.cap.read()
            height, width = img_color.shape[:2]
            # img_color = cv.resize(img_color, (width, height), interpolation=cv.INTER_AREA)

            # 원본 영상을 HSV 영상으로 변환합니다.
            img_hsv = cv2.cvtColor(img_color, cv2.COLOR_BGR2HSV)

            # 범위 값으로 HSV 이미지에서 마스크를 생성합니다.
            img_mask1 = cv2.inRange(img_hsv, self.lower_blue1, self.upper_blue1)
            img_mask2 = cv2.inRange(img_hsv, self.lower_blue2, self.upper_blue2)
            img_mask3 = cv2.inRange(img_hsv, self.lower_blue3, self.upper_blue3)
            img_mask = img_mask1 | img_mask2 | img_mask3  # 이진화된 이미지 get

            # 잡음제거
            kernel = np.ones((11, 11), np.uint8)
            img_mask = cv2.morphologyEx(img_mask, cv2.MORPH_OPEN, kernel)
            img_mask = cv2.morphologyEx(img_mask, cv2.MORPH_CLOSE, kernel)

            # 등고선 따기
            contours, hierarchy = cv2.findContours(img_mask, cv2.RETR_TREE,
                                                   cv2.CHAIN_APPROX_SIMPLE)
            max = 0
            if len(contours) != 0:
                for i in contours:
                    if len(i) > max:
                        max = len(i)
                        cnt = i
                x, y, w, h = cv2.boundingRect(cnt)
                cv2.rectangle(img_color, (x, y), (x + w, h + y), (0, 0, 255), 2)
                # 무게중심
                self.Cx = x + w // 2
                self.Cy = y + h // 2
                cv2.line(img_color, (self.Cx, self.Cy), (self.Cx, self.Cy), (0, 0, 255), 10)
            # 급식판 테두리 그리기
            cv2.rectangle(img_color, (self.x_strt, self.y_strt), (self.x_end, self.y_end), (0, 0, 0), 2)

            # 마스크 이미지로 원본 이미지에서 범위값에 해당되는 영상 부분을 획득합니다.
            img_result = cv2.bitwise_and(img_color, img_color, mask=img_mask)
            cv2.imshow('img_color', img_color)
            cv2.imshow('img_result', img_result)  # 트랙바가 생성될 창

            # ESC 키누르면 종료
            if cv2.waitKey(1) & 0xFF == 27:
                break

        cv2.destroyAllWindows()






if __name__ == '__main__':
    fname = 'C:/Users/82108/Downloads/openCV/opencv_/capstick.mp4'
    maindish = MainDish(fname)
    maindish.goingOn()
