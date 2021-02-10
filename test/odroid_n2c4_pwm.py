import RPi.GPIO as GPIO
import time


if __name__ == '__main__':
    duty = 100 # 100% duty cycle
    freq = 1000 # 1KHz frequency

    duty_switch_flag = False

    print('Press Ctrl-C to exit')

    GPIO.setmode(GPIO.BCM)

    # Exclude BCM 0, 1 since Odroid-N2Plus uses those pins for the RTC modules
    # that is an on-board component
    # Also exclude BCM 20, 21, 26 that are used for the other functions
    # on Odroid
    bcm_pin_list = set(list(range(0,27))) - set([0, 1, 20, 21, 26])
    pwm_list = list()

    # These BCM pin numbers are corresponded to physical pin number
    # 33, 12, 35, 15
    # Before using these, MAKE SURE THAT YOU HAVE ENABLED PWM DTBOS.
    # - Odroid-N2: pwm_cd, pwm_ef
    # - Odroid-C4: pwm_ab, pwm_ef
    n2_hw_pwm_bcm_pins = [ 13, 18, 19, 22 ]
    bcm_pin_list = n2_hw_pwm_bcm_pins

    for pin in bcm_pin_list:
        GPIO.setup(pin, GPIO.OUT)
        GPIO.output(pin, GPIO.HIGH)
        pwm_list.append(GPIO.PWM(pin, 1))

    for pwm in pwm_list:
        pwm.ChangeFrequency(freq)
        pwm.start(duty)

    try:
        while True:
            for pwm in pwm_list:
                pwm.ChangeDutyCycle(duty)

            if duty_switch_flag:
                duty = duty + 10
            else:
                duty = duty - 10

            if duty == 50 or duty == 100:
                duty_switch_flag = not duty_switch_flag

            print(f'duty: {duty}')
            time.sleep(0.2)

    except KeyboardInterrupt:
        for pwm in pwm_list:
            pwm.stop()

        for pin in bcm_pin_list:
            GPIO.output(pin, GPIO.LOW)
            GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

        GPIO.cleanup()
