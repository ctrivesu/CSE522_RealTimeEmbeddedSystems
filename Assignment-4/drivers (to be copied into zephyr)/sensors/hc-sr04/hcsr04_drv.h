#ifndef _SENSOR_HCSR04
#define _SENSOR_HCSR04

#include <kernel.h>

struct hcsr04_data
{
	struct device *dev_original;
	struct device *input_gpio_pinmux;
	struct device *input_pin_gpio_device;
	struct device *output_pin_gpio_device;
	struct device *output_pin_gpio_pinmux;
	struct gpio_callback input_pin_callback;
	uint32_t cm;

	//DECIDE WHETHER TO KEEP IT OR NOT
	struct k_sem data_sem;
	
	
};
#endif
