import serial, time
import cv2
image_src = 'image.jpg'
image_threshold = 127
resolution = 400
arduino = serial.Serial('COM4', 9600)
time.sleep(2)
image = cv2.imread(image_src)
image = cv2.resize(image,resolution,image.shape[1]//image.shape[0]*resolution)

ret,threshold = cv2.threshold(image,image_threshold,255,cv2.THRESH_BINARY)
drawing = False
(y_s,x_s,r) = threshold.shape()
arm_span = 20 #cm, max range of the arm
margin = 5

for y in range(y_s):
  for x in range(x_s):
    #Dibuja punto
    while(drawing):
      #Espera una cadena para el siguente punto
      rawString = arduino.readline()
      if(rawString == "a"):
        drawing = False
    #Transform from pixels to cm
    x = (x/x_s)*(arm_span-margin) + margin
    y = (y/y_s)*(arm_span-margin) + margin
    arduino.write(bytes("x{x}y{y}f".format(), 'utf-8'))
    drawing = True
arduino.close()
