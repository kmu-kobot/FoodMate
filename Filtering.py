import cv2

class Filter:
    def __init__(self):
        # 스캔한 이미지 읽기
        self.img = cv2.imread('../data/img/Scanned_image.jpg')

    def check_right(self, img):
        count = 0  # 등고선 처리 진행 정도 확인용 변수

        # 이진화된 이미지 얻어서 등고선 처리
        imgray = cv2.GaussianBlur(self.img, (5, 5), 0)
        imgray = cv2.cvtColor(imgray, cv2.COLOR_BGR2GRAY)
        thr = cv2.Laplacian(imgray, cv2.CV_8U, ksize=3)  # thr = cv2.Canny(imgray, 80, 200) 이 방법도 존재
        contours, hierarchy = cv2.findContours(thr, cv2.RETR_TREE,
                                               cv2.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area > 3000:
                count += 1
        return True if count > 5 else False