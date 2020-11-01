# write by kyoung chip , jang
#
# python 3.6
#
#
from ctypes import *
import numpy as np
import cv2
 
class Tracker(object):
 
    def __init__(self):
 
        self.track_lib = cdll.LoadLibrary('./libTracker.so')
        self.obj = self.track_lib.track_new()



    def get_track_point(self):
        track_point_x = self.track_lib.get_track_x(self.obj)
        track_point_y = self.track_lib.get_track_y(self.obj)

        point = [track_point_x, track_point_y]
        return point



if __name__ == '__main__':
 
    t = Tracker()
    pts = t.get_track_point()
    print(pts)
    
    


