# !/usr/local/bin/python3
# -*- coding:utf-8 -*-
import os

# "play sound in cmd" not using playsound module
class Sound:
    def __init__(self):
        pass

    def play_sound(self, dish_name):
        if dish_name == "none": # 아무 것도 들어오지 않은 경우
            os.system("canberra-gtk-play -f ~/Tracking-Food/data/sound/더 이동해주세요.wav")
            return
        elif dish_name == "scan_start": # 스캔 시작 알림
            os.system("canberra-gtk-play -f ~/Tracking-Food/data/sound/스캔중.wav")
            return

        elif dish_name =="scan_end": # 스캔 완료 알림
            os.system("canberra-gtk-play -f ~/Tracking-Food/data/sound/스캔완료.wav")
            return
        else: # 반찬 이름이 들어울 경우
            os.system("canberra-gtk-play -f ~/Tracking-Food/data/sound/" + dish_name + ".wav")
            return

if __name__ == '__main__':
    test = Sound()
    test.play_sound("none")
    test.play_sound("계란말이")
