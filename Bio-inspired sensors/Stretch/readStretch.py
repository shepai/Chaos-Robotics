from machine import Pin, ADC, PWM
from utime import sleep


st = ADC(Pin(27))

def get():
    print(st.read_u16())
