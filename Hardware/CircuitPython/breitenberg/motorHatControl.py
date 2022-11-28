import board
import digitalio
import time

m1 = digitalio.DigitalInOut(board.GP21)
m1.direction = digitalio.Direction.OUTPUT
m2 = digitalio.DigitalInOut(board.GP20)
m2.direction = digitalio.Direction.OUTPUT
m3 = digitalio.DigitalInOut(board.GP19)
m3.direction = digitalio.Direction.OUTPUT
m4 = digitalio.DigitalInOut(board.GP18)
m4.direction = digitalio.Direction.OUTPUT

en1 = digitalio.DigitalInOut(board.GP17)
en1.direction = digitalio.Direction.OUTPUT
en2 = digitalio.DigitalInOut(board.GP16)
en2.direction = digitalio.Direction.OUTPUT

def Enable_motor():
    en1.value=1  # motor 1 enable, set value 0 to disable
    en2.value=1  # motor 2 enable, set value 0 to disable

def Motor1_forward():
    m1.value=1
    m2.value=0
    
def Motor1_reverse():
    m1.value=0
    m2.value=1
    
def Motor2_forward():
    m3.value=1
    m4.value=0
    
def Motor2_reverse():
    m3.value=0
    m4.value=1
    
def Motor_stop():
    m1.value=0
    m2.value=0
    m3.value=0
    m4.value=0

"""
while True:
    Enable_motor()
    Motor1_forward()
    Motor2_forward()
    time.sleep(2) # Both motor in Forward direction for 2 seconds.
    Motor_stop()
    time.sleep(2) # Both motor in Stop position.
    Motor1_reverse()
    Motor2_reverse()
    time.sleep(2) # Both motor in Reverse direction for 2 seconds.
"""