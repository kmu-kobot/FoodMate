import cv2 as cv
import numpy as np

"""
def make_mask_image(img_bgr):
    img_hsv = cv.cvtColor(img_bgr, cv.COLOR_BGR2HSV)

    # img_h,img_s,img_v = cv.split(img_hsv)

    low = (0, 30, 0)
    high = (15, 255, 255)

    img_mask = cv.inRange(img_hsv, low, high)
    img_mask = cv.bitwise_not(img_mask)
    return img_mask
"""

def detect_chopstic(img, thr, minLineLength, maxLineGap):
    edges = cv.Canny(img, 50, 150, apertureSize = 3)
    # lines = cv.HoughLines(edges, 1,np.pi/180,thr)
    lines = cv.HoughLinesP(edges, 1,np.pi/180,thr, minLineLength=minLineLength,maxLineGap=maxLineGap)
    if lines is None:
        return None,None,None,None
    """
    for line in lines:
        r, theta = line[0]
        length = 100
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a*r
        y0 = b*r
        x1 = int(x0 + length*(-b))
        y1 = int(y0 + length*a)
        x2 = int(x0 - length*(-b))
        y2 = int(y0 - length*a)
    """
    for line in lines:
        x1, y1, x2, y2 = line[0]
    return x1,y1,x2,y2


cap = cv.VideoCapture(0)

fgbg = cv.createBackgroundSubtractorMOG2(varThreshold=300, detectShadows=0)

index = 0

while (1):
    index = index + 1

    ret, frame = cap.read()
    if ret == False:
        break

    frame = cv.flip(frame, 1)
    # 살색 제거 마스크 이미지
    # mask_img = make_mask_image(frame)

    # 차영상 검출
    blur = cv.GaussianBlur(frame, (5, 5), 0)
    fgmask = fgbg.apply(blur, learningRate=0)
    kernel = cv.getStructuringElement(cv.MORPH_ELLIPSE, (5, 5))
    fgmask = cv.morphologyEx(fgmask, cv.MORPH_CLOSE, kernel, 2)
    #########################################
    # x1,y1,x2,y2 = detect_chopstic(fgmask, 80)
    # fgmask = cv.bitwise_and(mask_img, fgmask)
    # fgmask = cv.medianBlur(fgmask,9)
    # cv.imshow('mask', mask_img)
    ###########################################
    # 젓가락 끝점 좌표
    x1,y1,x2,y2 = detect_chopstic(fgmask, 90,20,100)
    # 직선이 검출되면 선분과 끝점 그리기
    if x1 != None:
        cv.line(frame, (x1, y1), (x2, y2), (0, 255, 0), 5)
        cv.circle(frame, (x1, y1), 10, (255, 0, 0), -1)
    # 원본 이미지
    cv.imshow('img', frame)
    # 차영상
    cv.imshow('result', fgmask)

    key = cv.waitKey(30) & 0xff
    if key == 27:
        break

cap.release()
cv.destroyAllWindows()