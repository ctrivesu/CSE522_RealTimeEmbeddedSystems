#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <pinmux.h>
#include "pinmux_galileo.h"
#include <gpio.h>
#include <misc/printk.h>
#include <string.h>
#include <pwm.h>

#define GPIO_PORT "GPIO_0"
#define PIN 0
#define PINMUX CONFIG_PINMUX_NAME
#define PINMUX_PIN 5
#define PINFUNC PINMUX_FUNC_B


#define SLEEP_TIME 500
#define EDGE    (GPIO_INT_LEVEL | GPIO_INT_ACTIVE_HIGH)
static struct gpio_callback gpio_cb;

void gpio_callfunc(struct device *gpiob, struct gpio_callback *cb,
		    u32_t pins)
{
	printk("Interrupt triggered\n");
}


void main(void)
{
	struct device *dev_gpio;
	struct device *dev_pinmux;
	printk("Hello Board Started\n");
	
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


	dev_gpio = device_get_binding(GPIO_PORT);
	if (!dev_gpio) {
		printk("Error: Get Binding\n");
		return;
	}

	gpio_pin_configure(dev_gpio, PIN, GPIO_DIR_IN | GPIO_INT |  PULL_DOWN | EDGE);
	gpio_init_callback(&gpio_cb, gpio_callfunc, BIT(PIN));
	gpio_add_callback(dev_gpio, &gpio_cb);
	gpio_pin_enable_callback(dev_gpio, PIN);
	while(1)
	{
		u32_t val=0;
		gpio_pin_read(dev_gpio, PIN, &val);
		k_sleep(SLEEP_TIME);
	}
}