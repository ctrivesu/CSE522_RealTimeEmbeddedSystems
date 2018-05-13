#include <i2c.h>
#include <init.h>
#include <gpio.h>
#include <string.h>
#include <device.h>
#include "eeprom.h"
#include <zephyr.h>
#include <flash.h>
#include <board.h>

#define MEM_ADDRESS 0x54
uint8_t read[64], write[66];

static int eeprom_read(struct device *dev, off_t offset, void *data, size_t len)
{
	int i;
	uint8_t addr[2];
	struct eeprom_data *drv_data = dev->driver_data;
	addr[0] = (offset>>8)&0xFF;
	addr[1] = (offset&0xFF);
	gpio_pin_write(drv_data->wp, 8, 0);
	if(i2c_write(drv_data->i2c, addr, sizeof(addr), MEM_ADDRESS))
	{
		printk("I2C Address Error\n");
	}
	if(i2c_read(drv_data->i2c, data, len, MEM_ADDRESS))
	{
		printk("I2C Read Error\n");
	}
	/*for(i=0;i<len;i++)
	{
		printk("Iteration %d: %d\n", i, read[i]);
		data++;
	}*/
	return 0;
}


static int eeprom_write(struct device *dev, off_t offset, const void *data, size_t len)
{
	uint8_t addr[2];
	struct eeprom_data *drv_data = dev->driver_data;
	//FIRST PAGE
	addr[0] = (offset>>8)&0xFF;
	addr[1] = (offset&0xFF);
	//memset(write, 0x10, sizeof(write));
/*	write[0] = addr[0];
	write[1] = addr[1];
//NEEDS MODIFICATION	
	write[2] = 0x44;
	write[3] = 0x55;
	write[4] = 0x66;
	write[5] = 0x77;
	printk("D: %d %d %d %d/\n", data, &data, *val, (*(uint8_t *[66])data));

	memcpy(write+2, data, len);
	int i;
	for(i=0; i<len+2; i++)
	{printk("%d\n", write[i]);}	
*/
	gpio_pin_write(drv_data->wp, 8, 0);
	if(i2c_write(drv_data->i2c, data, len+2, MEM_ADDRESS))
	{
		printk("I2C Write Error\n");
	}
	k_sleep(1);
//	printk("WRITING COMPLETED\n");
	return 0;
}


static const struct flash_driver_api eeprom_api = {
	.read = eeprom_read,
	.write = eeprom_write,
	// .erase = eeprom_erase,
	// .write_protection = eeprom_write_protection_set,
};

static int eeprom_init(struct device *dev)
{
	struct eeprom_data *drv_data = dev->driver_data;
	printk("EEPROM INIT\n");

	//CONFIGURATION ACTIVITY
	dev->driver_api = &eeprom_api;
	drv_data->gpio = device_get_binding(CONFIG_GPIO_PCAL9535A_2_DEV_NAME);
	gpio_pin_write(drv_data->gpio, 12, 0);
	gpio_pin_configure(drv_data->gpio, 12, GPIO_DIR_OUT);
	drv_data->i2c = device_get_binding(CONFIG_GPIO_PCAL9535A_1_I2C_MASTER_DEV_NAME);
	i2c_configure(drv_data->i2c, (I2C_SPEED_FAST<<1)|I2C_MODE_MASTER);

	//Write Protection
	drv_data->wp = device_get_binding(PINMUX_GALILEO_EXP1_NAME);
	gpio_pin_configure(drv_data->wp, 8, GPIO_DIR_OUT);
	gpio_pin_write(drv_data->wp, 8, 1);

	printk("EEPROM COMPLETED\n");
	return 0;
}

static struct eeprom_data eeprom_24f256_data;
DEVICE_INIT(eeprom_dev, CONFIG_EEPROM_24FC256_DRV_NAME, eeprom_init, &eeprom_24f256_data, NULL, POST_KERNEL, CONFIG_EEPROM_24FC256_INIT_PRIORITY);


