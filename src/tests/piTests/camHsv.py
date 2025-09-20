from picamera2 import Picamera2, Preview 
import time 
import cv2
import numpy as np

picam2 = Picamera2() 
camera_config = picam2.create_preview_configuration() 
picam2.configure(camera_config) 
picam2.start_preview(Preview.QTGL) 
picam2.start() 
time.sleep(2) 
picam2.capture_file("test.jpg")

red_bgr = np.uint8([[[55, 39, 238]]])   # OpenCV uses BGR not RGB
green_bgr = np.uint8([[[44, 214, 68]]])
magenta_bgr = np.uint8([[[255, 0, 255]]])

red_hsv = cv2.cvtColor(red_bgr, cv2.COLOR_BGR2HSV)
green_hsv = cv2.cvtColor(green_bgr, cv2.COLOR_BGR2HSV)
magenta_hsv = cv2.cvtColor(magenta_bgr, cv2.COLOR_BGR2HSV)

print("Red HSV:", red_hsv)
print("Green HSV:", green_hsv)
print("Magenta HSV:", magenta_hsv)
