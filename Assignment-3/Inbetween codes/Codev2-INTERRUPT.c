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
//INPUT PIN - INTERRUPT TRIGGERED
#define GPIO_PORT PINMUX_GALILEO_GPIO_INTEL_CW_NAME
#define PIN 0
#define PINMUX_PIN 5
#define PINFUNC PINMUX_FUNC_B

//OUTPUT PIN - LED ON
#define GPIO_PORT2 PINMUX_GALILEO_GPIO_DW_NAME 
#define PIN2 6
#define PINMUX_PIN2 3
#define PINFUNC2 PINMUX_FUNC_A 



#define SLEEP_TIME 500
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH)
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
	struct device *dev_gpio2;
	struct device *dev_pinmux2;
	printk("Hello Board Started\n");
	
	// INPUT PIN 
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

	//OUTPUT PIN
	dev_pinmux2 = device_get_binding(PINMUX2);
	if(!dev_pinmux2)
	{
		printk("Cannot find PinMux2 device!\n");
		return;
	}

	if(pinmux_pin_set(dev_pinmux2, PINMUX_PIN2, PINFUNC2))
	{
		printk("Failed to Set Pinmux2 Pin");
		return;
	}

	dev_gpio2 = device_get_binding(GPIO_PORT2);
	if (!dev_gpio) {
		printk("Error: Get Binding\n");
		return;
	}


	//SETTING UP OUTPUT PIN
	gpio_pin_configure(dev_gpio2, PIN2, GPIO_DIR_OUT);
	if(gpio_pin_write(dev_gpio2, PIN2, 0))
		{printk("Output Write Failed");}

	//SETTING UP INPUT INTERRUPT PIN
	gpio_pin_configure(dev_gpio, PIN, GPIO_DIR_IN | GPIO_INT |  PULL_DOWN | EDGE);
	gpio_init_callback(&dev_gpio, gpio_callfunc, BIT(PIN));
	gpio_add_callback(dev_gpio, &gpio_cb);
	gpio_pin_enable_callback(dev_gpio, PIN);

while(1)
{

	if(gpio_pin_write(dev_gpio2, PIN2, 0))
	{printk("Output Write Failed");}

	if(gpio_pin_write(dev_gpio2, PIN2, 1))
	{printk("Output Write Failed");}
	
	if(gpio_pin_write(dev_gpio2, PIN2, 0))
	{printk("Output Write Failed");}

		u32_t val=0;
//		k_sleep(SLEEP_TIME);
}
}