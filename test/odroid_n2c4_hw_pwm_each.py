import RPi.GPIO as GPIO
import time


if __name__ == '__main__':
    # Odroid-N2
    # pin = 13 # PWM_C, Phy #33, pwmchip4, pwm0
    # pin = 19 # PWM_D, Phy #35, pwmchip4, pwm1
    # pin = 18 # PWM_E, Phy #12, pwmchip8, pwm0
    # pin = 22 # PWM_F, Phy #15, pwmchip8, pwm1

    # Odroid-C4
    # pin = 13 # PWM_A, Phy #33, pwmchip0, pwm0
    # pin = 19 # PWM_B, Phy #35, pwmchip0, pwm1
    # pin = 18 # PWM_E, Phy #12, pwmchip4, pwm0
    # pin = 22 # PWM_F, Phy #15, pwmchip4, pwm1

    duty = 100 # 100% duty cycle
    freq = 1000 # 1KHz frequency

    duty_switch_flag = False
    print('Press Ctrl-C to exit')

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.HIGH)

    pwm = GPIO.PWM(pin, 1)
    pwm.ChangeFrequency(freq)
    pwm.start(duty)

    try:
        while True:
            pwm.ChangeDutyCycle(duty)

            if duty_switch_flag:
                duty = duty + 5
            else:
                duty = duty - 5

            if duty == 50 or duty == 100:
                duty_switch_flag = not duty_switch_flag

            print(f'duty: {duty}')
            time.sleep(0.05)

    except KeyboardInterrupt:
        pwm.stop()

        GPIO.output(pin, GPIO.LOW)
        GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        GPIO.cleanup()
