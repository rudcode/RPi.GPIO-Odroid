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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#include "odroid.h"
#include "odroid_hw_pwm.h"

static int *procd_num_to_pwm_chip;
static int *procd_num_to_pwm_pin;

int hw_pwm_init() {
    if (!hw_pwm_is_board_supported())
        return -1;

    if (procd_num_to_pwm_chip != 0 && procd_num_to_pwm_pin != 0)
        return 0;

    switch (piModel) {
        case PI_MODEL_ODROIDN2:
            procd_num_to_pwm_chip = &gpio_subtracted_base_to_pwm_chip_N2;
            procd_num_to_pwm_pin = &gpio_subtracted_base_to_pwm_pin_N2;
        break;
        case PI_MODEL_ODROIDC4:
            procd_num_to_pwm_chip = &gpio_subtracted_base_to_pwm_chip_C4;
            procd_num_to_pwm_pin = &gpio_subtracted_base_to_pwm_pin_C4;
        break;
        default:
        break;
    }

    if (procd_num_to_pwm_chip != 0 && procd_num_to_pwm_pin != 0)
        return 0;
    else
        return -1;
}

int hw_pwm_is_board_supported() {
    return (piModel == PI_MODEL_ODROIDN2 ||
            piModel == PI_MODEL_ODROIDC4);
}

int hw_pwm_is_pin_supported(unsigned int gpio) {
    gpio = _bcm_subtract_gpio_base_number(gpio);

    if (procd_num_to_pwm_chip == 0 && procd_num_to_pwm_pin == 0)
        return 0;
    else
        return procd_num_to_pwm_pin[gpio] >= 0;
}

int hw_pwm_set_duty_cycle(unsigned int gpio, float dutycycle) {
    int fd, len, ret;
    int cur_freq;
    char str_duty[17], str_freq[17];
    char pwm_duty_path[65];
    char pwm_freq_path[65];

    gpio = _bcm_subtract_gpio_base_number(gpio);

    if (dutycycle < 0 || dutycycle > ODROID_AMLOGIC_MAX_SPI_FREQ)
        return -1;

    if ((ret = _toggle_pwm_pin(gpio, PWM_PIN_DISABLE)) < 0)
        return ret;

    snprintf(pwm_freq_path, sizeof(pwm_freq_path), "/sys/class/pwm/pwmchip%d/pwm%d/period",
        procd_num_to_pwm_chip[gpio], procd_num_to_pwm_pin[gpio]);
     if ((fd = open(pwm_freq_path, O_RDONLY)) < 0)
        return -1;

    read(fd, str_freq, sizeof(str_freq) - 1);
    close(fd);

    cur_freq = atoi(str_freq);
    dutycycle = cur_freq * dutycycle / 100;
    len = snprintf(str_duty, sizeof(str_duty), "%d", (int) round(dutycycle));

    snprintf(pwm_duty_path, sizeof(pwm_duty_path), "/sys/class/pwm/pwmchip%d/pwm%d/duty_cycle",
        procd_num_to_pwm_chip[gpio], procd_num_to_pwm_pin[gpio]);
    if ((fd = open(pwm_duty_path, O_WRONLY)) < 0)
        return -1;

    write(fd, str_duty, len);
    close(fd);

    if ((ret = _toggle_pwm_pin(gpio, PWM_PIN_ENABLE)) < 0)
        return ret;

    return 0;
}

int hw_pwm_set_frequency(unsigned int gpio, float freq) {
    int fd, len, ret;
    char str_freq[17];
    char pwm_freq_path[65];

    gpio = _bcm_subtract_gpio_base_number(gpio);

    if (freq <= 0 || freq > ODROID_AMLOGIC_MAX_SPI_FREQ)
        return -1;

    len = snprintf(str_freq, sizeof(str_freq), "%d", (int) round(freq));

    if ((ret = _toggle_pwm_pin(gpio, PWM_PIN_DISABLE)) < 0)
        return ret;

    snprintf(pwm_freq_path, sizeof(pwm_freq_path), "/sys/class/pwm/pwmchip%d/pwm%d/period",
        procd_num_to_pwm_chip[gpio], procd_num_to_pwm_pin[gpio]);
    if ((fd = open(pwm_freq_path, O_WRONLY)) < 0)
        return -1;

    write(fd, str_freq, len);
    close(fd);

    if ((ret = _toggle_pwm_pin(gpio, PWM_PIN_ENABLE)) < 0)
        return ret;

    return 0;
}

int hw_pwm_start(unsigned int gpio) {
    int fd, len, ret;
    char str_pwm_pin[3];
    char pwm_export_path[65];

    gpio = _bcm_subtract_gpio_base_number(gpio);

    len = snprintf(str_pwm_pin, sizeof(str_pwm_pin), "%d", procd_num_to_pwm_pin[gpio]);

    snprintf(pwm_export_path, sizeof(pwm_export_path), "/sys/class/pwm/pwmchip%d/export", procd_num_to_pwm_chip[gpio]);
    if ((fd = open(pwm_export_path, O_WRONLY)) < 0)
        return -1;

    write(fd, str_pwm_pin, len);
    close(fd);

    if ((ret = _toggle_pwm_pin(gpio, PWM_PIN_ENABLE)) < 0)
        return ret;

    return 0;
}

int hw_pwm_stop(unsigned int gpio) {
    int fd, len, ret;
    char str_pwm_pin[3];
    char pwm_unexport_path[65];

    gpio = _bcm_subtract_gpio_base_number(gpio);

    len = snprintf(str_pwm_pin, sizeof(str_pwm_pin), "%d", procd_num_to_pwm_pin[gpio]);

    if ((ret = _toggle_pwm_pin(gpio, PWM_PIN_DISABLE)) < 0)
        return ret;

    snprintf(pwm_unexport_path, sizeof(pwm_unexport_path), "/sys/class/pwm/pwmchip%d/unexport", procd_num_to_pwm_chip[gpio]);
    if ((fd = open(pwm_unexport_path, O_WRONLY)) < 0)
        return -1;

    write(fd, str_pwm_pin, len);
    close(fd);

    return 0;
}

// Return 0 if the file cannot be opened or actually disabled
int hw_pwm_exists(unsigned int gpio) {
    int fd;
    char pwm_enable_path[65];
    char enabled[2];

    gpio = _bcm_subtract_gpio_base_number(gpio);

    snprintf(pwm_enable_path, sizeof(pwm_enable_path), "/sys/class/pwm/pwmchip%d/pwm%d/enable",
        procd_num_to_pwm_chip[gpio], procd_num_to_pwm_pin[gpio]);
    if ((fd = open(pwm_enable_path, O_RDONLY)) < 0)
        return 0;

    read(fd, enabled, sizeof(enabled) - 1);

    return atoi(enabled);
}

int _toggle_pwm_pin(unsigned int gpio, int mode) {
    int fd;
    char pwm_enable_path[65];
    char mode_str[5];

    gpio = _bcm_subtract_gpio_base_number(gpio);

    snprintf(mode_str, sizeof(mode_str), "%d", mode);
    snprintf(pwm_enable_path, sizeof(pwm_enable_path), "/sys/class/pwm/pwmchip%d/pwm%d/enable",
        procd_num_to_pwm_chip[gpio], procd_num_to_pwm_pin[gpio]);
    if ((fd = open(pwm_enable_path, O_WRONLY)) < 0)
        return -1;

    write(fd, mode_str, sizeof(mode_str));
    close(fd);

    return 0;
}

int _bcm_subtract_gpio_base_number(unsigned int gpio) {
    switch (piModel) {
        case PI_MODEL_ODROIDN2:
            return (gpio >= N2_GPIO_PIN_BASE) ? (gpio - N2_GPIO_PIN_BASE) : gpio;
        case PI_MODEL_ODROIDC4:
            return (gpio >= C4_GPIO_PIN_BASE) ? (gpio - C4_GPIO_PIN_BASE) : gpio;
        default:
            // This has not be occured
            return gpio;
    }
}
