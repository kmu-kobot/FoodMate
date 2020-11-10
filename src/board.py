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

class Board(object):

	def __init__(self):

		self.board_lib = cdll.LoadLibrary('../lib/libBoard.so')
		self.obj = self.board_lib.Board_new()
		self.box_x = []
		self.box_y = []
		self.count = 0

	def img_preproces(self):
		self.board_lib.board_img_preproces(self.obj)

	def get_target_area( self) :
		self.board_lib.board_get_target_area(self.obj)
	
	def board_frgm_board(self):
       
		self.board_lib.board_frgm_board(self.obj)
		self.get_crop_Rects()

	def board_rectArea(self, rect):

		self.board_lib.board_rectArea(self.obj, rect)


	def get_center_board(self):
		center_board_x = self.board_lib.center_board_x(self.obj)
		center_board_y = self.board_lib.center_board_y(self.obj)
		point = [center_board_x, center_board_y]
		return point


	def get_crop_Rects(self):
		count = self.board_lib.crop_Rects_size()
		for i in range(count):
			x = self.board_lib.crop_Rects_x(self.obj,i)
			y = self.board_lib.crop_Rects_y(self.obj,i)
			w = self.board_lib.crop_Rects_w(self.obj,i)
			h = self.board_lib.crop_Rects_h(self.obj,i)
			self.box_x.append([640-x-w, 640-x]) 
			self.box_y.append([y, h + y])



