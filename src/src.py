import cv2
import numpy as np
import serial
from picamera2 import Picamera2, Preview

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
picam = Picamera2()
cam_config = picam.create_preview_configuration(main={"format": 'XRGB8888', 'size': (640, 480)})
picam.configure(cam_config)
picam.set_controls({"FrameDurationLimits": (33333, 33333)})
picam.start_preview(Preview.NULL)
picam.start()

lowerGreen = np.array([45, 150, 150])
upperGreen = np.array([70, 255, 255])

lowerRed1 = np.array([170, 150, 150])
upperRed1 = np.array([180, 255, 255])
lowerRed2 = np.array([0, 150, 150])
upperRed2 = np.array([10, 255, 255])

lowerMag = np.array([140, 150, 150])
upperMag = np.array([160, 255, 255])

kernel = np.ones((5, 5), np.uint8)
sent = False

while True:
	frame = picam.capture_array()
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	
	maskGreen = cv2.inRange(hsv, lowerGreen, upperGreen)
	maskGreen = cv2.morphologyEx(maskGreen, cv2.MORPH_OPEN, kernel)

	maskRed1 = cv2.inRange(hsv, lowerRed1, upperRed1)
	maskRed2 = cv2.inRange(hsv, lowerRed2, upperRed2)
	maskRed = cv2.bitwise_or(maskRed1, maskRed2)
	maskRed   = cv2.morphologyEx(maskRed, cv2.MORPH_OPEN, kernel)

	maskMag = cv2.inRange(hsv, lowerMag, upperMag)
	maskMag   = cv2.morphologyEx(maskMag, cv2.MORPH_OPEN, kernel)

	if not sent:
		if cv2.countNonZero(maskGreen) > 200:
			ser.write(b"G\n")
			sent = True
		elif cv2.countNonZero(maskRed) > 200:
			ser.write(b"R\n")
			sent = True
		elif cv2.countNonZero(maskMag) > 200:
			ser.write(b"M\n")
			sent = True
	else:
		if (cv2.countNonZero(maskGreen) <= 200 and 
			cv2.countNonZero(maskRed) <= 200 and 
			cv2.countNonZero(maskMag) <= 200):
			sent = False

	key = cv2.waitKey(1)
	if key == 27:
		break

picam.close()
ser.close()
cv2.destroyAllWindows()