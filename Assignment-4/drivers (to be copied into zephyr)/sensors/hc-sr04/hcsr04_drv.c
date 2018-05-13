#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <pinmux.h>
#include "pinmux_galileo.h"
#include <gpio.h>
#include <misc/printk.h>
#include <string.h>
#include <pwm.h>
#include <shell/shell.h>
#include <i2c.h>
#include <sensor.h>
#include <flash.h>
#include <sys_io.h>
#include <init.h>
#include <misc/util.h>
#include <misc/__assert.h>
#include <clock_control.h>
#include <misc/util.h>
#include <sys_clock.h>
#include <limits.h>
#include <kernel.h>
#include "hcsr04_drv.h"
#define EDGE    (GPIO_INT_EDGE|GPIO_INT_DEBOUNCE|GPIO_INT_ACTIVE_HIGH)
//uint32_t start_time, end_time;
uint64_t start_time, end_time;


//HCSR - 01 PARAMETERS
#define PINMUX_PIN1_TRIGGER 2
#define GPIO_PIN1_TRIGGER 5
#define PINMUX_PIN1_ECHO 12
#define GPIO_PIN1_ECHO 7

//HCSR - 02 PARAMETERS
#define PINMUX_PIN2_TRIGGER 3
#define GPIO_PIN2_TRIGGER 6
#define PINMUX_PIN2_ECHO 10
#define GPIO_PIN2_ECHO 2

struct hcsr_pin
{
	//PIN NO CONFIGURATION
	uint32_t PINMUX_PIN_TRIGGER;
	uint32_t GPIO_PIN_TRIGGER;
	uint32_t PINMUX_PIN_ECHO;
	uint32_t GPIO_PIN_ECHO;
};

struct hcsr_pin hpin1 = {
	.PINMUX_PIN_TRIGGER = PINMUX_PIN1_TRIGGER,
	.GPIO_PIN_TRIGGER = GPIO_PIN1_TRIGGER,
	.PINMUX_PIN_ECHO = PINMUX_PIN1_ECHO,
	.GPIO_PIN_ECHO = GPIO_PIN1_ECHO
};

struct hcsr_pin hpin2 = {
	.PINMUX_PIN_TRIGGER = PINMUX_PIN2_TRIGGER,
	.GPIO_PIN_TRIGGER = GPIO_PIN2_TRIGGER,
	.PINMUX_PIN_ECHO = PINMUX_PIN2_ECHO,
	.GPIO_PIN_ECHO = GPIO_PIN2_ECHO
};


void callback_function_hcsr(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
{
	uint32_t val = 0;
	uint32_t cycles_spent;
	uint32_t nanseconds_spent;
	uint32_t cm;
	struct hcsr04_data *drv_data =CONTAINER_OF(cb, struct hcsr04_data, input_pin_callback);
	struct hcsr_pin *drv_pin = drv_data->dev_original->config->config_info;
	//	printk("Interrupt called\n");
	//start_time = k_cycle_get_32();
	start_time = _tsc_read();
	gpio_pin_disable_callback(drv_data->input_pin_gpio_device, drv_pin->GPIO_PIN_ECHO);
	do
	{
		gpio_pin_read(drv_data->input_pin_gpio_device,drv_pin->GPIO_PIN_ECHO,&val);
		//end_time = k_cycle_get_32();
		end_time = _tsc_read();
	}while(val==1);
	cycles_spent = end_time - start_time;
	//nanseconds_spent = SYS_CLOCK_HW_CYCLES_TO_NS(cycles_spent);
	//cm = nanseconds_spent / 58000;
	cm = (end_time - start_time)/(400*58);
	drv_data->cm = cm;
	//printk("CM: %d START: %d END: %d NS: %d\n", cm, start_time, end_time, nanseconds_spent);
}

static int hcsr04_sample_fetch(struct device *dev, enum sensor_channel chan)
{
//	printk("SAMPLE FETCH START\n");
	struct hcsr04_data *drv_data = dev->driver_data;
	struct hcsr_pin *drv_pin = dev->config->config_info;
	gpio_pin_enable_callback(drv_data->input_pin_gpio_device, drv_pin->GPIO_PIN_ECHO);
	
//	printk("LStarted\n");
	gpio_pin_write(drv_data->output_pin_gpio_device, drv_pin->GPIO_PIN_TRIGGER,1);
//	printk("RISEN\n");
	k_sleep(K_MSEC(1));
	gpio_pin_write(drv_data->output_pin_gpio_device, drv_pin->GPIO_PIN_TRIGGER,0);
	k_sleep(K_MSEC(1));
//	printk("LEnded\n");

	//k_sem_take(&drv_data->data_sem, K_FOREVER);
//	printk("SAMPLE FETCH SUCCESS\n");
	return 0;
}


static int hcsr04_channel_get(struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	struct hcsr04_data *drv_data = dev->driver_data;

	__ASSERT_NO_MSG(chan == SENSOR_CHAN_ALL || chan == SENSOR_CHAN_DISTANCE);
	val->val1 = drv_data->cm;
	val->val2 = 0;
	return 0;
}

static const struct sensor_driver_api hcsr04_driver_api = {
	.sample_fetch = hcsr04_sample_fetch,
	.channel_get = hcsr04_channel_get,
};

static int hcsr04_init(struct device *dev)
{
	printk("HCSR INIT STARTED\n");
	struct hcsr04_data *drv_data = dev->driver_data;
	struct hcsr_pin  *drv_pin = dev->config->config_info;
	drv_data->dev_original = dev;

printk("S: %d %d %d %d\n", drv_pin->PINMUX_PIN_TRIGGER, drv_pin->GPIO_PIN_TRIGGER, drv_pin->PINMUX_PIN_ECHO, drv_pin->GPIO_PIN_ECHO);
//################################################################
	//IO10 -- Output pin -> ECHO PIN
	drv_data->input_gpio_pinmux = device_get_binding(CONFIG_PINMUX_NAME);
	pinmux_pin_set(drv_data->input_gpio_pinmux, drv_pin->PINMUX_PIN_ECHO, PINMUX_FUNC_A);
	drv_data->input_pin_gpio_device = device_get_binding(CONFIG_GPIO_DW_0_NAME);
	gpio_pin_configure(drv_data->input_pin_gpio_device, drv_pin->GPIO_PIN_ECHO, GPIO_DIR_OUT);
    //setting 0
    if(gpio_pin_write(drv_data->input_pin_gpio_device, drv_pin->GPIO_PIN_ECHO, 0))
    {printk("Output Failed");}


    //IO10 -- Input pin -> ECHO PIN
    drv_data->input_gpio_pinmux = device_get_binding(CONFIG_PINMUX_NAME);
    pinmux_pin_set(drv_data->input_gpio_pinmux, drv_pin->PINMUX_PIN_ECHO, PINMUX_FUNC_B);
    drv_data->input_pin_gpio_device = device_get_binding(CONFIG_GPIO_DW_0_NAME);
    gpio_pin_configure(drv_data->input_pin_gpio_device, drv_pin->GPIO_PIN_ECHO, GPIO_DIR_IN | GPIO_INT | EDGE);
    gpio_init_callback(&(drv_data->input_pin_callback), callback_function_hcsr, BIT(drv_pin->GPIO_PIN_ECHO));
    gpio_add_callback(drv_data->input_pin_gpio_device, &(drv_data->input_pin_callback));
    //gpio_pin_enable_callback(drv_data->input_pin_gpio_device, 2);

    //IO3 -- Output pin -> TRIGGER PIN
    drv_data->output_pin_gpio_device = device_get_binding(CONFIG_GPIO_DW_0_NAME);
    drv_data->output_pin_gpio_pinmux = device_get_binding(CONFIG_PINMUX_NAME);
    pinmux_pin_set(drv_data->output_pin_gpio_pinmux, drv_pin->PINMUX_PIN_TRIGGER, PINMUX_FUNC_A);
    gpio_pin_configure(drv_data->output_pin_gpio_device, drv_pin->GPIO_PIN_TRIGGER, GPIO_DIR_OUT);
    
    //IO3 -- Setting as 0
    if(gpio_pin_write(drv_data->output_pin_gpio_device, drv_pin->GPIO_PIN_TRIGGER, 0))
    {printk("Output Failed");}


//#########################################################################
	printk("HCSR INIT COMPLETE\n");
	return 0;
}

struct hcsr04_data hcsr0401_driver;
struct hcsr04_data hcsr0402_driver;

DEVICE_AND_API_INIT(HCSR_0, CONFIG_HCSR04_0_NAME, hcsr04_init, &hcsr0401_driver, &hpin1, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, &hcsr04_driver_api);
DEVICE_AND_API_INIT(HCSR_1, CONFIG_HCSR04_1_NAME, hcsr04_init, &hcsr0402_driver, &hpin2, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, &hcsr04_driver_api);
