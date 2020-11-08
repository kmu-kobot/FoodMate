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


from ctypes import *
import numpy as np
import cv2
 
class Tracker(object):
 
    def __init__(self):
        self.track_lib = cdll.LoadLibrary('../lib/libTracker.so')
        self.obj = self.track_lib.Tracker_new()

    def track_point(self):
        self.track_lib.track_point(self.obj)

    def get_track_point(self):
        track_point_x = self.track_lib.track_point_x(self.obj)
        track_point_y = self.track_lib.track_point_y(self.obj)

        point = [track_point_x, track_point_y]
        return track_point_x , track_point_y

    
