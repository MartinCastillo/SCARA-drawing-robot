import serial, time
import cv2
arduino = serial.Serial('COM4', 9600)
time.sleep(2)
image = cv2.imread('src_imagen.png')
threshold = cv2.

drawing = False
(y_s,x_s,r) = threshold.shape()
for y in range(y_s):
  for x in range(x_s):
    #Dibuja punto
    while(drawing):
      #Espera una cadena para el siguente punto
      rawString = arduino.readline()
      if(rawString == "a"):
        drawing = False
    arduino.write(bytes("{}".format(), 'utf-8'))
    drawing = True
arduino.close()
