# write by kyoung chip , jang
#
# python 3.6
#
#
from ctypes import *
import numpy as np
import cv2
 
class Board(object):
 
    def __init__(self):
 
        self.board_lib = cdll.LoadLibrary('./libBoard.so')
        self.obj = self.board_lib.Board_new()
        self.box_x = []
        self.box_y = []
        self.count = 0
 
    def img_preproces(self, src):
        
        src = self.board_lib.board_img_preproces( self.obj, src )
        return src
 
    def get_target_area( self) :
        
        self.board_lib.board_get_target_area(self.obj)
   
    def board_frgm_board(self):
        
        self.board_lib.board_frgm_board(self.obj)
    
    def board_rectArea(self, rect):

        self.board_lib.board_rectArea(self.obj, rect)


    def get_center_board(self):
        center_board_x = self.board_lib.center_board_x(self.obj)
        center_board_y = self.board_lib.center_board_y(self.obj)
        point = [center_board_x, center_board_y]
        return point


    def get_crop_Rects(self):
        count = self.board_lib.crop_Rects_size()
        print(count)
        for i in range(count):
            x = self.board_lib.crop_Rects_x(self.obj,i)
            y = self.board_lib.crop_Rects_y(self.obj,i)
            w = self.board_lib.crop_Rects_w(self.obj,i)
            h = self.board_lib.crop_Rects_h(self.obj,i)
            self.box_x.append([x, x + w])
            self.box_y.append([y, h + y])    




