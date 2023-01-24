from machine import Pin, ADC, PWM
from utime import sleep

from time import sleep
from machine import Pin, PWM

#motor
pwm = PWM(Pin(2))
pwm.freq(50)
#stretch listen
st = ADC(Pin(27))

def setPosition(angle):
    position=1000+50*angle
    if position>9000: position=9000
    pwm.duty_u16(position)
    sleep(0.01)

av=0
for i in range(100):
    av+=st.read_u16()
av/=100

def get():
    return min(max(0,(st.read_u16() - av)/400),1)#*3.3
##v((sensor.read_u16()  * voltage)/voltage)

def get_raw():
    print(st.read_u16())

"""angle=0
while True:
    setPosition(angle)
    sleep(0.5)
    print(angle,get())
    if angle==180: break
    angle+=1"""
