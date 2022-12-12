#Micropython code for reading sensor values

from machine import Pin, ADC, PWM
from utime import sleep


st = ADC(Pin(27)) #connect to GP27

def get():
    print(st.read_u16()) #output reading
