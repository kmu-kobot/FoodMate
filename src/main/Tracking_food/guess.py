# coding=utf-8

import cv2
import tensorflow.keras
from PIL import Image, ImageOps
import numpy as np
import os

class Guess:
    def __init__(self):
        # Disable scientific notation for clarity
        np.set_printoptions(suppress=True)
        
        # Load the model
        self.model = tensorflow.keras.models.load_model('../data/model/keras_model.h5')

        self.data = np.ndarray(shape=(1, 224, 224, 3), dtype=np.float32)


    # 해당 path 에 있는 이미지들을 labeling 한다.
    def realGuess(self, decimg):

        # 음식리스트 가져오기
        label = ['계란말이', '김치', '된장국', '멸치조림', '시금치','우엉조림','흰밥']

        # Replace this with the path to your image
        size = (224, 224)
        image = ImageOps.fit(decimg, size, Image.ANTIALIAS)

        #turn the image into a numpy array
        image_array = np.asarray(image)
        # Normalize the image
        normalized_image_array = (image_array.astype(np.float32) / 127.0) - 1
        # Load the image into the array
        self.data[0] = normalized_image_array
        # run the inference
        prediction = self.model.predict(self.data)
        print(prediction[0])  # 세부 예측빈도
        max_pct = np.max(prediction[0]) # max_percentage
        max_pct_idx = np.argmax(prediction[0]) # max_percentage's index
        answer = label[max_pct_idx]

        # 디버깅용 코드
        print("▶ " + answer)
        cv2.imshow("input", decimg)
        cv2.waitKey(0)
        return answer



if __name__ == '__main__':
    print(1)
    guess = Guess()
    print(guess.realGuess())
