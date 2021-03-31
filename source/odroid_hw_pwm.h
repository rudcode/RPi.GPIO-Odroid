/*
 * Hardware PWM is only supported by the following Odroid board list
 * - Odroid-N2
 * - Odroid-C4
 * Please refer to the Odroid wiki websites to get further information
 * - https://wiki.odroid.com/common/application_note/gpio/pwm

Copyright (c) 2020 Deokgyu Yang <secugyu@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ODROID_HW_PWM_H
#define ODROID_HW_PWM_H

#define ODROID_AMLOGIC_MAX_SPI_FREQ 80000000
#define PWM_PIN_ENABLE 1
#define PWM_PIN_DISABLE 0

/*****************************************************************************/
/*
 * These tables are containing subtracting GPIO base pin number from given
 * GPIO pin number. This becuase this RPi.GPIO-Odroid is supporting various
 * Odroid boards that each have the different GPIO pin number. So that the
 * given pin number will be a real number that the SoC regards.
 *
 * e.g, In C4, if a user codes using BCM pin #13 then this number will be
 * converted to #482 that is the corresponding number C4 uses. And as the base
 * number for 40 pin header on Odroid-C4 is 460, (482 - 460) is the index for
 * the following tables.
 */
/*****************************************************************************/
/*
 * Odroid-N2
 NET  |   GPIO   | 40 PIN | BCM | GPIO - BASE |  PWMCHIP  | PWM PIN
PWM_C | GPIOX. 5 |   33   | 13  |     71      | PWMCHIP.4 | 0
PWM_D | GPIOX. 6 |   35   | 19  |     72      | PWMCHIP.4 | 1
PWM_E | GPIOX.17 |   12   | 18  |     82      | PWMCHIP.8 | 0
PWM_F | GPIOX. 7 |   15   | 22  |     73      | PWMCHIP.8 | 1
*/
static int gpio_subtracted_base_to_pwm_chip_N2[129] = {
    -1,     // Index 0, that will not be used
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 20
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 30
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 40
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 50
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 60
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 70
     4,  4,  8, -1, -1, -1, -1, -1, -1, -1, // 80
    -1,  8, -1, -1, -1, -1, -1, -1, -1, -1, // 90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 100
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 110
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 120
    -1, -1, -1, -1, -1, -1, -1, -1          // 128
};

static int gpio_subtracted_base_to_pwm_pin_N2[129] = {
    -1,     // Index 0, that will not be used
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 20
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 30
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 40
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 50
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 60
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 70
     0,  1,  1, -1, -1, -1, -1, -1, -1, -1, // 80
    -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, // 90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 100
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 110
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 120
    -1, -1, -1, -1, -1, -1, -1, -1          // 128
};

/*
 * Odroid-C4
 NET  |   GPIO   | 40 PIN | BCM | GPIO - BASE |  PWMCHIP  | PWM PIN
PWM_A | GPIOX. 6 |   33   | 13  |     22      | PWMCHIP.0 | 0
PWM_B | GPIOX.19 |   35   | 19  |     35      | PWMCHIP.0 | 1
PWM_E | GPIOX.16 |   12   | 18  |     32      | PWMCHIP.4 | 0
PWM_F | GPIOX. 7 |   15   | 22  |     23      | PWMCHIP.4 | 1
*/
static int gpio_subtracted_base_to_pwm_chip_C4[129] = {
    -1,     // Index 0, that will not be used
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 20
    -1,  0,  4, -1, -1, -1, -1, -1, -1, -1, // 30
    -1,  4, -1, -1,  0, -1, -1, -1, -1, -1, // 40
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 50
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 60
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 70
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 80
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 100
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 110
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 120
    -1, -1, -1, -1, -1, -1, -1, -1          // 128
};

static int gpio_subtracted_base_to_pwm_pin_C4[129] = {
    -1,     // Index 0, that will not be used
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 10
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 20
    -1,  0,  1, -1, -1, -1, -1, -1, -1, -1, // 30
    -1,  0, -1, -1,  1, -1, -1, -1, -1, -1, // 40
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 50
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 60
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 70
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 80
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 90
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 100
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 110
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 120
    -1, -1, -1, -1, -1, -1, -1, -1          // 128
};

int hw_pwm_init();
int hw_pwm_is_board_supported();
int hw_pwm_is_pin_supported(unsigned int gpio);
int hw_pwm_set_duty_cycle(unsigned int gpio, float dutycycle);
int hw_pwm_set_frequency(unsigned int gpio, float freq);
int hw_pwm_start(unsigned int gpio);
int hw_pwm_stop(unsigned int gpio);
int hw_pwm_exists(unsigned int gpio);

#endif
