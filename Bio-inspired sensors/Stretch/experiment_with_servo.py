from machine import Pin, ADC, PWM
from utime import sleep

from time import sleep
from machine import Pin, PWM

#motor
servo_pin = Pin(2)
servo_pwm = PWM(servo_pin)
# Set the servo angle
servo_pwm.freq(50)  # Set PWM frequency to 50 Hz (standard for servos)
#stretch listen
st = ADC(Pin(27))
def setPosition(angle):
    # Define servo parameters (pulse width range for 180 degrees)
    min_duty = 40  # Adjust according to your servo
    max_duty = 9000  # Adjust according to your servo
    # Convert angle to duty cycle
    duty = min_duty + (max_duty - min_duty) * angle / 180.0
    
    servo_pwm.duty_u16(int(duty))
    
##v((sensor.read_u16()  * voltage)/voltage)

def get_raw():
    print(st.read_u16())


setPosition(0)