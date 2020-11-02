# !/usr/local/bin/python3
# -*- coding:utf-8 -*-
import cv2 as cv
import numpy as np
import time

def make_mask_image(img_bgr):
    img_hsv = cv.cvtColor(img_bgr, cv.COLOR_BGR2HSV)

    # img_h,img_s,img_v = cv.split(img_hsv)

    low = np.array([0, 130, 77], dtype = "uint8")
    high = np.array([255, 173, 127], dtype = "uint8")

    img_mask = cv.inRange(img_hsv, low, high)

    return img_mask

def detect_chopstic(img, thr, minLineLength, maxLineGap):
    cur_line=[]
    edges = cv.Canny(img, 50, 150, apertureSize=3)

    # lines = cv.HoughLines(edges, 1,np.pi/180,thr)
    lines = cv.HoughLinesP(edges, 1, np.pi / 180, thr, minLineLength=minLineLength, maxLineGap=maxLineGap)

    if lines is None:
        return None, None, None, None

    x1, y1, x2, y2 = lines[0][0]
    return x1, y1, x2, y2


def detect_spoon(img):
    circles = cv.HoughCircles(img, cv.HOUGH_GRADIENT, 1, 10,
                              param1=50, param2=12, minRadius=20, maxRadius=60)
    if circles is None:
        return None, None, None
    x = np.uint16(np.around(circles))[0][0][0]
    y = np.uint16(np.around(circles))[0][0][1]
    r = np.uint16(np.around(circles))[0][0][2]
    return x, y, r


cap = cv.VideoCapture(0)
ret = cap.set(3, 640)
ret = cap.set(4, 480)
fgbg = cv.createBackgroundSubtractorMOG2(varThreshold=350, detectShadows=0)

prevTime = 0
while (1):
    ret, frame = cap.read()
    if ret == False:
        break
    curTime = time.time()
    sec = curTime - prevTime
    prevTime = curTime
    fps = 1 / (sec)
    print("Time {0} ".format(sec))
    print("Estimated fps {0} ".format(fps))
    str = "FPS : %0.1f" % fps
    frame = cv.flip(frame, 1)
    # 살색 제거 마스크 이미지
    mask_img = make_mask_image(frame)
    mask_img = cv.medianBlur(mask_img, 5)
    # 차영상 검출
    """
    blur = cv.GaussianBlur(frame, (5, 5), 0)
    fgmask = fgbg.apply(blur, learningRate=0)
    kernel = cv.getStructuringElement(cv.MORPH_ELLIPSE, (5, 5))
    fgmask = cv.morphologyEx(fgmask, cv.MORPH_CLOSE, kernel, 2)

    x1,y1,x2,y2 = detect_chopstic(fgmask, 90, 15, 100)
    fgmask = cv.bitwise_and(mask_img, fgmask)
    fgmask = cv.medianBlur(fgmask,9)
    cv.imshow('mask', mask_img)
    """
    # 젓가락 끝점 좌표
    # x1, y1, x2, y2 = detect_chopstic(fgmask, 60, 50, 100)
    x1, y1, x2, y2 = detect_chopstic(mask_img, 50, 50, 100)
    #c_x, c_y, r = detect_spoon(fgmask)
    # 직선이 검출되면 선분과 끝점 그리기
    if x1 != None:
        cv.line(frame, (x1, y1), (x2, y2), (0, 255, 0), 5)
        cv.circle(frame, (x2,y2) if y1>y2 else (x1, y1), 10, (255, 0, 0), -1)
    # 원이 검출되면 원 그리기
    # if c_x != None:
    #     cv.circle(frame, (c_x, c_y), r, (0, 255, 0), 5)
    #     cv.circle(frame, (c_x, c_y), 10, (255, 0, 0), -1)
    # 원본 이미지
    cv.putText(frame, str, (0, 100), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0))
    cv.imshow('img', frame)
    # 차영상
    # cv.imshow('result', fgmask)
    cv.imshow('result', mask_img)
    key = cv.waitKey(30) & 0xff
    if key == 27:
        break

cap.release()
cv.destroyAllWindows()
