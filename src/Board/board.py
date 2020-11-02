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
 
    def img_preproces(self, src):
        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
        result, imgencode = cv2.imencode('.jpg', src, encode_param)
        data = numpy.array(imgencode)
        stringData = data.tostring()
        
        data1 = self.board_lib.board_img_preproces( self.obj, stringData, src.shape[0], src.shape[1])
        
        cv2.imshow("test2", data1)
 
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
        idex = self.board_lib.crop_Rects_size()
        print(idex)
        rects = []
        for i in range(idex):
            x = self.board_lib.crop_Rects_x(self.obj,i)
            y = self.board_lib.crop_Rects_y(self.obj,i)
            w = self.board_lib.crop_Rects_y(self.obj,i)
            h = self.board_lib.crop_Rects_h(self.obj,i)
            rect = [x, y, w, h]
            rects.append(rect)

        return rects
 
class PT_data(Structure) : 
    _fields_ = [("x", c_int), ("y", c_int), ("w", c_int), ("h", c_int)] 

if __name__ == '__main__':
 
    b = Board()
    b.get_target_area()
    b.board_frgm_board()
    rects = b.get_crop_Rects()
    print(rects)
    
    


