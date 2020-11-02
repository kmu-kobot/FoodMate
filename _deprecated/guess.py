# coding=utf-8

import cv2
import tensorflow.keras
from PIL import Image, ImageOps
import numpy as np
import os
from playsound import playsound
from natsort import natsorted, ns
import sound


class Guess:
    def __init__(self):
        # Disable scientific notation for clarity
        np.set_printoptions(suppress=True)

        # Load the model
        self.model = tensorflow.keras.models.load_model('../data/model/keras_model.h5')

        # Create the array of the right shape to feed into the keras model
        # The 'length' or number of images you can put into the array is
        # determined by the first position in the shape tuple, in this case 1.
        self.data = np.ndarray(shape=(1, 224, 224, 3), dtype=np.float32)
        self.result = []  # 각 급식판의 조각마다 학습된 결과가 담김


    def realGuess(self):

        # 음식리스트 가져오기
        label = ['계란말이', '김치', '된장국', '멸치조림', '시금치','우엉조림' ,'흰밥']


        # Replace this with the path to your image
        path = '../data/img'
        images_notsorted = os.listdir(path)
        images = images_notsorted[:]
        images = natsorted(images_notsorted, key=lambda y: y.lower())
        print(images)

        for img in images:
            image = Image.open(path+'/'+img)
            #resize the image to a 224x224 with the same strategy as in TM2:
            #resizing the image to be at least 224x224 and then cropping from the center
            size = (224, 224)
            image = ImageOps.fit(image, size, Image.ANTIALIAS)

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

            print(img + "▶ " + label[max_pct_idx])
            self.result.append(label[max_pct_idx])



    # 음식과 이름을 위치에 따라서 확인해 값을 준다. (좌표위치와, box위치) => answer 정답을 반환
    def matchFood(self, x, y, w_lst, h_lst):
        answer = ''  # 정답을 반환
        if True:
            for i in range(len(w_lst)):
                if w_lst[i][0] < x < w_lst[i][1] and h_lst[i][0] < y < h_lst[i][1]:  # 어떤 한 박스의 영역에만 들어와도
                    answer = (self.result[i])  # 이부분이 케라스의 result와 연동되어야할 부분
                    break
                else:  # 파란 포인터는 있는데 반찬있는 곳에 있지 않을경우
                    answer = '더 이동해주세요'
            # 파란색 포인터의 좌표와 정답출력
            print(x)
            print(y)
            print(answer)
            sound1 =sound.Sound()
            sound1.play_sound(answer)
        return answer

if __name__ == '__main__':
    guess = Guess()
    print(guess.realGuess())
