# coding=utf-8
import cv2
from playsound import playsound
import os

class Board:

    def __init__(self, image=None):

        # 스캔한 이미지 읽기 
        self.img = cv2.imread('../data/img/Scanned_image.jpg')  
    
        # 조각의 위치를 기록, 반찬의 가지수
        self.box_x = []
        self.box_y = []
        self.count = 0



    # 급식판 틀 잡기: 급식판 좌표를 반환
    def edgeOfBoard(self):

        canny = cv2.Canny(self.img, 100, 200)

        # 이미지의 기본 속성 (행, 열, channel정보)
        row = canny.shape[0]  # 행 y
        cal = canny.shape[1]  # 열 x

        # 좌표 구하기
        x_strt, x_end, y_strt, y_end = 0, 0, 0, 0
        for x in range(1, cal):
            if canny[row // 8, x] != 0 and x_strt == 0:
                x_strt = x
            elif canny[row // 8, cal - x] != 0 and x_end == 0:
                x_end = cal - x
            if x_strt != 0 and x_end != 0:
                break
        for y in range(1, row):
            if canny[y, cal // 2] != 0 and y_strt == 0:
                y_strt = y
            elif canny[row - y, cal // 2] != 0 and y_end == 0:
                y_end = row - y
            if y_strt != 0 and y_end != 0:
                break

        return x_strt, x_end, y_strt, y_end


    # 급식판의 메뉴를 조각 이미지로 저장, 반찬의 가지수 구함
    def frgm_board(self, callback):
        count = 0    # 등고선 처리 진행 정도 확인용 변수
        x_strt, x_end, y_strt, y_end = callback()
        cut_img = self.img[y_strt: y_end, x_strt: x_end]
        cut_img = self.img.copy()
        all_menu = cut_img.copy()

        # 이미지의 기본 속성 (행, 열, channel정보)
        row = cut_img.shape[0]  # 행 y
        cal = cut_img.shape[1]  # 열 x


        # 이진화된 이미지 얻어서 등고선 처리
        imgray = cv2.GaussianBlur(cut_img, (5, 5), 0)
        imgray = cv2.cvtColor(imgray, cv2.COLOR_BGR2GRAY)
        #thr = cv2.Canny(imgray, 10, 200)
        thr = cv2.Laplacian(imgray, cv2.CV_8U, ksize=3)  # thr = cv2.Canny(imgray, 80, 200) 이 방법도 존재
        contours, hierarchy = cv2.findContours(thr, cv2.RETR_TREE,
                                               cv2.CHAIN_APPROX_SIMPLE)

        # 등고선 전처리 ('contours'는 리스트로써 각각의 등고선들(등고선도 리스트로 각각의 점을 담음)을 담고있다)
        print(len(contours))
        for i in range(len(contours)):
            
            # 각 등고선마다의 사각형 포인트를 구해, 일정 크기 이상의 사각형만을 반찬으로써 도출
            cnt = contours[i]
            x, y, w, h = cv2.boundingRect(cnt)
            if (cal // 2) > w > (cal // 10) and (row // 1.5) > h > (row // 4):
                cv2.rectangle(all_menu, (x, y), (x + w, h + y), (0, i, 9 * i), 2)
                # 이미지 저장
                each_menu = cut_img.copy()  # 새로운 그림에 찍어야 선그은게 안보임
                self.count += 1  # 자른 조각 카운트
                path = '../data/img'  # 자른 이미지들 저장 경로 및 저장
                cv2.imwrite(path +'/'+ str(self.count) + '.jpg', each_menu[y:h + y, x:x + w])  # 자른 급식판 저장
                self.box_x.append([x, x + w])  # 조각의 위치를 기록
                self.box_y.append([y, h + y])

            cv2.imshow('all_menu', all_menu)
            
            # 스캔 시작과 끝에 음성 출력
            if count == 0 or count == len(contours)-1:
                playsound('../data/sound/스캔중.mp3')
            count += 1

        cv2.destroyAllWindows()



if __name__ == '__main__':
    make = Board()
    x, y, z, w = make.edgeOfBoard()
    print(x, y, z, w)
    count = make.frgm_board(make.edgeOfBoard)
    print(make.count)
    print(make.box_x)
