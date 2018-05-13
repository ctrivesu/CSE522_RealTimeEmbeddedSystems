#ifndef __EEPROM_H__
#define __EEPROM_H__

struct eeprom_data 
{
	struct device *i2c;
	struct device *gpio;
	struct device *wp;
};

#endif