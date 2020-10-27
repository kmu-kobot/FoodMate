#include "Sound.h"
#include <iostream>
#include <cstdlib>
#include <string>

Sound::Sound(){}
Sound::~Sound(){}
void Sound::play_sound(std::string dish){
    // cmd 창에서 실행된다고 생각
    // wav 파일만. mp3 파일 x
    if(dish == "none"){
        system("canberra-gtk-play -f ~/Tracking-Food/data/sound/더 이동해주세요.wav");
        return;
    }
    else if(dish =="scan_start"){
        system("canberra-gtk-play -f ~/Tracking-Food/data/sound/스캔중.wav");s
        return;
    }
    else if(dish=="scan_end"){
        system("canberra-gtk-play -f ~/Tracking-Food/data/sound/스캔완료.wav");
        return;
    }
    else{
        system(("canberra-gtk-play -f ~/Tracking-Food/data/sound/"+dish+".wav").c_str());
        return;
    }

    // if(dish == "계란말이"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/계란말이.wav");
    //     return;
    // }

    // if(dish == "김치"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/김치.wav");
    //     return;
    // } 

    // if(dish == "김치국"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/김치국.wav");
    //     return;
    // }

    // if(dish == "된장국"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/된장국.wav");
    //     return;
    // }

    // if(dish == "두부조림"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/두부조림.wav");
    //     return;
    // }
    
    // if(dish == "멸치조림"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/멸치조림.wav");
    //     return;
    // }
    // if(dish == "미역국"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/미역국.wav");
    //     return;
    // }
    // if(dish == "북엇국"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/북엇국.wav");
    //     return;
    // }
    // if(dish == "불고기"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/불고기.wav");
    //     return;
    // }
    // if(dish == "시금치"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/시금치.wav");
    //     return;
    // }
    // if(dish == "우엉조림"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/우엉조림.wav");
    //     return;
    // }
    // if(dish == "육개장"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/육개장.wav");
    //     return;
    // }
    
    // if(dish == "장조림"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/장조림.wav");
    //     return;
    // }

    // if(dish == "현미밥"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/현미밥.wav");
    //     return;
    // }
    // if(dish == "흰밥"){
    //     system("canberra-gtk-play -f ~/Tracking-Food/data/sound/흰밥.wav");
    //     return;
    // }




}
