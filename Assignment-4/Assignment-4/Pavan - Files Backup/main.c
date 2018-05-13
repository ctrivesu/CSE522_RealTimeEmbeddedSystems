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
#include <hcsr04_drv.h>
#include <sensor.h>

#define PINMUX CONFIG_PINMUX_NAME
#define GPIO_PORT PINMUX_GALILEO_GPIO_DW_NAME
#define PINMUX_SDA 18
#define PINMUX_CLK 19
#define PINMUX_WP 3
#define WP_PIN 6
#define PINFUNC_WP PINMUX_FUNC_A
#define PINFUNC_I2C PINMUX_FUNC_C

struct device *dev_i2c;
struct device *dev_gpio;
struct device *dev_pinmux;
struct device *dev_hcsr04;

void main (void)
{
	printk("CODE STARTED: 14th April---\n");
	dev_pinmux = device_get_binding(PINMUX);
  if(!dev_pinmux)
  {
    printk("Cannot find PinMux device!\n");
    return;
  }
	//WP GPIO PIN SET UP
	dev_gpio = device_get_binding(GPIO_PORT);
	if (!dev_gpio)
	{
		printk("Error: Get Binding\n");
		return;
	}
  //I2C Get binding
  dev_i2c = device_get_binding(CONFIG_GPIO_PCAL9535A_1_I2C_MASTER_DEV_NAME);
  if(!dev_i2c)
  {
    printk("Cannot find PinMux device!\n");
    return;
  }
	//hcsr04 binding
	dev_hcsr04 = device_get_binding(CONFIG_HCSR04_0_NAME);
  if(!dev_hcsr04)
  {
    printk("Cannot find HCSR04 device! ---Error: %d\n",dev_hcsr04);
    return;
  }
	//WRITE PROTECTION
  if(pinmux_pin_set(dev_pinmux, PINMUX_WP, PINFUNC_WP))
  {
    printk("Failed to Set Pinmux Pin");
    return;
  }
    //GPIO PIN CONFIGURE
  gpio_pin_configure(dev_gpio, WP_PIN, GPIO_DIR_OUT);

    //printk("STEP4: 14th April\n");
    //I2C Pin Configured
	if(i2c_configure(dev_i2c, (I2C_SPEED_FAST) | I2C_MODE_MASTER))
	{
		printk("Failed to Config I2C");
    return;
	}
	//WRITE COMMAND
  if(gpio_pin_write(dev_gpio, WP_PIN, 0))
    {printk("Output Write Failed");}


    uint8_t buffer[3];
		//uint8_t temp_buffer[2];
    uint8_t read1[1];
		//uint8_t data[1];
		int ret;
		memset(read1, 0x00, 1);
		memset(buffer, 0x00, 1);

		struct sensor_value *val=NULL;
		ret =sensor_channel_get(dev_hcsr04,1,val);
		printk("return value from channel get: %d",ret);

    //buffer[0] = 0x00;
    //buffer[1] = 0x00;
    //buffer[2] = 0x50;
		//temp_buffer[0] = 0x00;
		//temp_buffer[1] = 0x00;
		//
		// data[0] = 0xAE;
		// ret = write_bytes(dev_i2c, 0x00, &data[0], 1);
		// if (ret) {
		// 	printk("Error writing to FRAM! error code (%d)\n", ret);
		// 	return;
		// } else {
		// 	printk("Wrote 0xAE to address 0x00.\n");
		// }

		// data[0] = 0x00;
  	// ret = read_bytes(dev_i2c, 0x00, &data[0], 1);
	  // if (ret) {
		//   printk("Error reading from FRAM! error code (%d)\n", ret);
		//   return;
	  // } else {
		//   printk("Read 0x%X from address 0x00.\n", data[0]);
	  // }
		//
		//
		// // if(i2c_write(dev_i2c, temp_buffer, 2, 0x54))
    // // {
    // // 	printk("I2C Write Error2\n");
    // // }
		// // //
		// // // if(gpio_pin_write(dev_gpio, WP_PIN, 0))
    // // //     {printk("Output Write Failed");}
		// //
		// // printk("dummy read addr enabled\n");
		// //
    // // if(i2c_read(dev_i2c, read1, 1, 0x54))
    // // {
    // // 	printk("I2C Read Error\n");
		// //
    // // }
		// //
    // // if(i2c_write(dev_i2c, buffer, 3, 0x54))
    // // {
    // // 	printk("I2C Write Error1\n");
    // // }
		// //
		// // // if(gpio_pin_write(dev_gpio, WP_PIN, 1))
    // // //     {printk("Output Write Failed");}
		// // k_sleep(100);
		// //
		// // if(i2c_write(dev_i2c, temp_buffer, 2, 0x54))
    // // {
    // // 	printk("I2C Write Error2\n");
    // // }
		// // //
		// // // if(gpio_pin_write(dev_gpio, WP_PIN, 0))
    // // //     {printk("Output Write Failed");}
		// //
		// // printk("read addr enabled\n");
		// //
    // // if(i2c_read(dev_i2c, read1, 1, 0x54))
    // // {
    // // 	printk("I2C Read Error\n");
		// //
    // // }
    // printk("STEP6: 14th April\n");
    // printk("%d", read1[0]);

}
