#include "Sound.h"
#include <iostream>
#include <cstdlib>


Sound::Sound(){}
Sound::~Sound(){}
void Sound::play_sound(std::string dish){
    // cmd 창에서 실행된다고 생각
    // wav 파일만. mp3 파일 x
    if(dish == "none"){
        system("canberra-gtk-play -f ~/Tracking-Food/data/sound/더 이동해주세요.wav");
        return;
    }
    if(dish == "계란말이"){
        system("canberra-gtk-play -f ~/Tracking-Food/data/sound/계란말이.wav");
        return;
    }


}