#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <pinmux.h>
#include "pinmux_galileo.h"
#include <gpio.h>
#include <misc/printk.h>
#include <string.h>
#include <pwm.h>

#define PINMUX CONFIG_PINMUX_NAME
#define PORT PINMUX_GALILEO_PWM0_NAME
#define PINMUX_PIN 3
#define PWM_PIN 1	
#define PINFUNC PINMUX_FUNC_D

#define Period 2000

void main(void)
{
int status=1;
struct device *dev_pinmux;
struct device *dev_pwm_gpio;
u32_t pulse_width, pin_no;
printk("PWM demo\n");

dev_pinmux = device_get_binding(PINMUX);
if(!dev_pinmux)
{
		printk("Cannot find PinMux device!\n");
		return;
}

if(pinmux_pin_set(dev_pinmux, PINMUX_PIN, PINFUNC))
{
	printk("Failed to Set Pinmux Pin");
	return;
}

dev_pwm_gpio = device_get_binding(PORT);
if(!dev_pwm_gpio)
{
	printk("Cannot find PWM0 device!\n");
	return;
}

while(1)
{
	status = pwm_pin_set_cycles(dev_pwm_gpio, PWM_PIN, 4095, Period);
	if (status!=0)
	{
		printk("PWM Pin Set failed");
	}
	printk("Loop Running");
k_sleep(MSEC_PER_SEC);
}
}
