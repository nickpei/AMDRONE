#!/usr/bin/env python

import serial

import time

# Assign Arduino's serial port address
usbport = '/dev/cu.usbmodem1421'

# Set up serial baud rate
ser = serial.Serial(usbport, 9600, timeout=1)

# Save the angle

ang1 = 90

ang2 = 65

ang3 = 45

ang4 = 45

def move(servo, angle):
    '''Moves the specified servo to the supplied angle.

    Arguments:
        servo
          the servo number to command, an integer from 1-4
          base: 1
          left: 2
          right: 3
          claw: 4
        angle
          the desired servo angle, an integer from 0 to 180

    (e.g.) >>> servo.move(2, 90)
           ... # "move servo #2 to 90 degrees"'''

    if (0 <= angle <= 180):
        ser.write(chr(255))
        ser.write(chr(servo))
        ser.write(chr(angle))
    else:
        print("Servo angle must be an integer between 0 and 180.\n")

def init():
    
    time.sleep(2)
    move(1,ang1);
    move(2,ang2);
    move(3,ang3);
    move(4,ang4);

    while 1:
        control()

def control():

    global ang1
    global ang2
    global ang3
    global ang4
    
    user_input = raw_input('Your input:')
    
    #base move left
    if user_input.upper() == 'Z':
        ser.write(chr(255))
        ser.write(chr(1))
        ang1 = ang1 + 10
        ser.write(chr(ang1))
    #base move right
    if user_input.upper() == 'X':
        ser.write(chr(255))
        ser.write(chr(1))       
        ang1 = ang1 - 10
        ser.write(chr(ang1))

    #left move up
    if user_input.upper() == 'Q':
        ser.write(chr(255))
        ser.write(chr(2))
        ang2 = ang2 + 10
        check_angle()
        ser.write(chr(ang2))
    #left move down
    if user_input.upper() == 'A':
        ser.write(chr(255))
        ser.write(chr(2))       
        ang2 = ang2 - 10
        check_angle()
        ser.write(chr(ang2))

    #right extend
    if user_input.upper() == 'W':
        ser.write(chr(255))
        ser.write(chr(3))
        ang3 = ang3 + 10
        ser.write(chr(ang3))
    #right retrive
    if user_input.upper() == 'S':
        ser.write(chr
                  (255))
        ser.write(chr(3))       
        ang3 = ang3 - 10
        ser.write(chr(ang3))

    #claw close
    if user_input.upper() == 'C':
        ser.write(chr(255))
        ser.write(chr(4))
        ang4 = ang4 + 2
        ser.write(chr(ang4))
    #claw open
    if user_input.upper() == 'V':
        ser.write(chr(255))
        ser.write(chr(4))       
        ang4 = ang4 - 2
        ser.write(chr(ang4))

def check_angle():

    global ang1
    global ang2
    global ang3
    global ang4
    
    if ang1 >=255 or ang2 >=255 or ang3 >=255 or ang4 >=255:
        print 'Out of range'
        if ang1 >=255:
            ang1 = ang1 - 10
        if ang2 >=255:
            ang2 = ang2 - 10
        if ang3 >=255:
            ang3 = ang3 - 10
        if ang4 >=255:
            ang4 = ang4 - 10

    

init()

