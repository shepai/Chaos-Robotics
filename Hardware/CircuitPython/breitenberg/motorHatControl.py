
import time
import board
import digitalio
import pwmio
import simpleio
from adafruit_motor import motor
en1 = digitalio.DigitalInOut(board.GP17)
en1.direction = digitalio.Direction.OUTPUT
en2 = digitalio.DigitalInOut(board.GP16)
en2.direction = digitalio.Direction.OUTPUT

def Enable_motor():
    en1.value=1  # motor 1 enable, set value 0 to disable
    en2.value=1  # motor 2 enable, set value 0 to disable
    

button = digitalio.DigitalInOut(board.GP22)
button.direction = digitalio.Direction.INPUT

PWM_M1A = board.GP21  # pick any pwm pins on their own channels
PWM_M1B = board.GP20
PWM_M2A = board.GP19  # pick any pwm pins on their own channels
PWM_M2B = board.GP18

# DC motor setup
# DC Motors generate electrical noise when running that can reset the microcontroller in extreme
# cases. A capacitor can be used to help prevent this.
pwm_1a = pwmio.PWMOut(PWM_M1A, frequency=10000)
pwm_1b = pwmio.PWMOut(PWM_M1B, frequency=10000)
motor1 = motor.DCMotor(pwm_1a, pwm_1b)

pwm_2a = pwmio.PWMOut(PWM_M2A, frequency=10000)
pwm_2b = pwmio.PWMOut(PWM_M2B, frequency=10000)
motor2 = motor.DCMotor(pwm_2a, pwm_2b)

NOTE_C4 = 261
NOTE_G4 = 392



def Motor1_forward(speed=0.8):
    motor1.throttle = -1*speed
    
def Motor1_reverse():
    motor1.throttle = 0.8
    
def Motor2_forward(speed=0.7):
    motor2.throttle = -1*speed
    
def Motor2_reverse():
    motor2.throttle = 0.8
    
def Motor_stop():
    motor1.throttle = 0
    motor2.throttle = 0


#Enable_motor()
#Motor1_forward()
#Motor2_forward()

