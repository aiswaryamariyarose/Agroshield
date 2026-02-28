import serial
import time

arduino = serial.Serial('COM3', 9600, timeout=1)  # use the port from step 2
time.sleep(2)  # allow Arduino to reset

while True:
    if arduino.in_waiting > 0:
        line = arduino.readline().decode('utf-8').strip()
        print("Arduino says:", line)