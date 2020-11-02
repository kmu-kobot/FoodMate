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
        print(0)
        self.track_lib = cdll.LoadLibrary('./libTrack.so')
        print(10)
        self.obj = self.track_lib.Tracker_new()
        print(1)

    def track_point(self):
        self.track_lib.track_point(self.obj)

    def get_track_point(self):
        print(2)
        track_point_x = self.track_lib.track_point_x(self.obj)
        track_point_y = self.track_lib.track_point_y(self.obj)

        point = [track_point_x, track_point_y]
        return point



if __name__ == '__main__':

    t = Tracker()
    pts = t.track_point()   

#    while(\True):
#        t = Tracker()
#        pts = t.get_track_point()
#        print(pts)
        
    
