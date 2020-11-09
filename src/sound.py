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


import os
import multiprocessing


# "play sound in cmd" not using playsound module
class Sound:
    def __init__(self,answer):

        p = multiprocessing.Process(name="SubProcess", target=self.play_sound, args=(answer,))
        p.start()


    def play_sound(self, dish_name):
        if dish_name == "none": # 아무 것도 들어오지 않은 경우
            os.system("canberra-gtk-play -f ../data/sound/더이동해주세요.wav")
            return
        elif dish_name == "scan_start": # 스캔 시작 알림
            os.system("canberra-gtk-play -f ../data/sound/스캔중.wav")
            return

        elif dish_name =="scan_end": # 스캔 완료 알림
            os.system("canberra-gtk-play -f ../data/sound/스캔완료.wav")
            return
        elif dish_name == "scan_find": # 스캔 시작 알림
            os.system("canberra-gtk-play -f ../data/sound/음식이보입니다.wav")
            return
        else: # 반찬 이름이 들어울 경우
            os.system("canberra-gtk-play -f ../data/sound/" + str(dish_name) + ".wav")
            return
        

