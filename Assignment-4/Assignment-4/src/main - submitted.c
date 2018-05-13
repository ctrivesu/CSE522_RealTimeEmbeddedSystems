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
#include <stdio.h>
#include <stdlib.h>

#define MY_SHELL_MODULE "RESULTS"
#define EEPROM_OFFSET 0x0000
struct device *dev_hcsr04;
struct device *dev_eeprom;
uint8_t pages_to_write=4;
uint32_t enable_time = 0;
uint32_t cstamp = 0;

//VARIABLES STUCK
int ret,i,j,buffer_to_write=1,count=0,temp_test=100;
bool buffer_select=0,buffer_ready=0;
struct sensor_value val_dist;
struct sensor_value val_time;


//BUFFERS BEING USED
static uint8_t write_buf1[66], next_page_addr[2];
static uint8_t read_buf1[64],  read_addr[2];
uint16_t next_addr=0;
uint32_t time_stamp =0;
uint8_t start_page=0,end_page =0;


void HCSR_enable()
{
	//k_thread_suspend(&EEPROM_data);
	printk("EEPROM about to clean\n");
	next_addr = 0;
	next_page_addr[0] = 0x00;
	next_page_addr[1] = 0x00;
	for(i=0;i<512;i++)
	{
		write_buf1[0] = next_page_addr[0];
		write_buf1[1] = next_page_addr[1];
		for(j=2;j<66;j++)
		{
			write_buf1[j] = 0x00;
		}
		ret =flash_write(dev_eeprom, EEPROM_OFFSET, write_buf1,sizeof(write_buf1));       //send the address+data to write
    	k_sleep(1);
		next_addr = next_addr+64;
		next_page_addr[0] = (next_addr >> 8) & 0xFF;
		next_page_addr[1] = (next_addr ) & 0xFF;
	}

	printk("EEPROM clean succesful\n");
	next_addr = 0;
	next_page_addr[0] = 0x00;
	next_page_addr[1] = 0x00;
	j=0;

	while(j<pages_to_write)
	{
		count=2;
		printk("inside hcsr while\n");
		write_buf1[0] = next_page_addr[0];
		write_buf1[1] = next_page_addr[1];
		for(i=0;i<8;i++)
		{
			cstamp = _tsc_read();
			ret = sensor_sample_fetch(dev_hcsr04);
			k_sleep(1);
			ret =sensor_channel_get(dev_hcsr04, SENSOR_CHAN_ALL ,&val_dist);
			printk("hscr dist         :%llu\n",val_dist.val1);
			cstamp = cstamp - enable_time;			
			//printk("Measurement_time  :%llu\n",val_time.val1);
			write_buf1[count+0] = ((cstamp) & 0xFF);
			write_buf1[count+1] = ((cstamp>>8) & 0xFF);
			write_buf1[count+2] = ((cstamp>>16) & 0xFF);
			write_buf1[count+3] = ((cstamp>>24) & 0xFF);
			write_buf1[count+4] = ((val_dist.val1) & 0xFF);
			write_buf1[count+5] = ((val_dist.val1 >> 8) & 0xFF);
			write_buf1[count+6] = ((val_dist.val1 >> 16) & 0xFF);
			write_buf1[count+7] = ((val_dist.val1 >> 24) & 0xFF);
			count=count+8;
			k_sleep(10);
		}

		ret =flash_write(dev_eeprom, EEPROM_OFFSET, write_buf1,sizeof(write_buf1));       //send the address+data to write
		//printk("EEPROM write_buff1 succesful\n");
    	k_sleep(1);


		next_addr = next_addr+64;
		next_page_addr[0] = (next_addr >> 8) & 0xFF;
		next_page_addr[1] = (next_addr ) & 0xFF;
		printk("-----------------------------\n");
		k_sleep(1);
		j++;
	}
	printk("WRITE DONE\n");
}






static int shell_enable(int argc, char *argv[])
{
	int num=1;
	int HCSR_selected = 0;
	if(argc != 2)
	{
		printk("Enter one argument");
		return 1;
	}
	HCSR_selected = atoi(argv[num]);
	printk("HCSR_selected : %d",HCSR_selected);

	if(HCSR_selected == 0)
	printk(" No HCSR is enabled\n");

	else if(atoi(argv[num]) == 1)
	{
		printk(" Enabling HCSR[%d]\n", HCSR_selected);
		dev_hcsr04 = device_get_binding(CONFIG_HCSR04_0_NAME);
	}
	else if(atoi(argv[num]) == 2)
	{
		printk(" Enabling HCSR[%d]\n", HCSR_selected);
		dev_hcsr04 = device_get_binding(CONFIG_HCSR04_1_NAME);
	}
	else
	{
		printk(" Enter 0 or 1 or 2\n");
		return 1;
	}
	dev_eeprom = device_get_binding(CONFIG_EEPROM_24FC256_DRV_NAME);
	if(!dev_eeprom)
	{
		printk("Cannot find EEPROM device!\n");
	}
	return 0;
}

static int shell_start(int argc, char *argv[])
{
    printk("SHELL START WORKING\n");
	int num=1;
	if(argc != 2)
	{
		printk("Enter one argument");
		return 1;
	}

	pages_to_write = atoi(argv[num]);

	if(atoi(argv[num]) == 0)
	printk(" No Pages Written\n");
	else if(atoi(argv[num]) <= 512)
	{
			printk(" Pages to Write: %d\n", pages_to_write);
	}
	else
	{
		printk(" Enter pages <= 512\n");
		return 1;
	}
	enable_time = _tsc_read();
	HCSR_enable();
    printk("SHELL START FINISHED\n");
	return 0;
}

static int shell_dump(int argc, char *argv[])
{
	int num=1;
	uint32_t temp_offset;
	uint32_t dist_from_eeprom, time_stamp_eerpom;
	if(argc != 3)
	{
		printk("Enter two argument");
		return 1;
	}
	start_page = atoi(argv[num]);
	end_page = atoi(argv[num+1]);
	printk("Start Page : %d\n",start_page);
	printk("End Page   : %d\n",end_page);

		if(end_page < start_page)
	    printk("No page is erased\n");
		else if((end_page > 512) || (start_page > 512))
		{
			printk(" Enter pages <= 512\n");
			return 1;
		}

	printk("inside the EEPROM thread func\n");

	j=start_page;
	next_addr = 0 + start_page*64;
	temp_offset = 0 + start_page*64;
	printk("NEXT ADDRESS: %d \n", next_addr);
	while(j<=end_page)
	{
		next_page_addr[0] = (next_addr >> 8) & 0xFF;
		next_page_addr[1] = (next_addr) & 0xFF;

		read_addr[0] = next_page_addr[0];
		read_addr[1] = next_page_addr[1];
		//ret =flash_write(dev_eeprom, EEPROM_OFFSET, read_addr,sizeof(read_addr));         //send the address to read
		//printk("EEPROM addr write succesful\n");
		//k_sleep(1);
		ret =flash_read(dev_eeprom, temp_offset, read_buf1,sizeof(read_buf1));
		for(i=0;i<64;i++)
		{
			printk("read byte_%d: %d\n",i,read_buf1[i]);
		}
		
		 int p=1;
		 for(i=0;i<64;i=i+8)
		 {
		 	time_stamp_eerpom = ((uint32_t)read_buf1[i+0])|((uint32_t)read_buf1[i+1]<<8)|((uint32_t)read_buf1[i+2]<<16)|((uint32_t)read_buf1[i+3]<<24);
		 	printk("Time_stamp [page%d, sample%d : %d\n",j,p,time_stamp_eerpom);
			dist_from_eeprom = ((uint32_t)read_buf1[i+4])|((uint32_t)read_buf1[i+5]<<8)|((uint32_t)read_buf1[i+6]<<16)|((uint32_t)read_buf1[i+7]<<24);
			printk("Distance   [page%d, sample%d : %d\n",j,p,dist_from_eeprom);
		 	p++;
		 }


		temp_offset = temp_offset+0x40;
		printk("temp_offset : %d",temp_offset);

		next_addr = next_addr+ 0x40;
		//j++;
		j++;
		printk("-------------------------\n");
	  k_sleep(1);
	}
}



//SHELL DATASTRUCTURE
static struct shell_cmd commands[] = 
{
    { "1",shell_enable, "ENABLE FUNCTION - Choose one (0/1/2) - Type Enable x" },
    { "2",shell_start, "START FUNCTION" },
    { "3",shell_dump, "DUMP FUNCTION" },
    { NULL, NULL, NULL }
};




void main()
{
	   SHELL_REGISTER(MY_SHELL_MODULE, commands);
	   printk("MAIN ENDED\n");
}
/*
void main()
{
	int i;
	struct sensor_value distance;
    printk("HELLO! MAIN STARTED\n");
	dev_hcsr = device_get_binding(CONFIG_HCSR04_0_NAME);
	if(!dev_hcsr)
    {
        printk("Cannot find HCSR04 device! ---Error\n");
        return;
    }
    for(i=0;i<300;i++)
    {
    	sensor_sample_fetch(dev_hcsr);	
    	sensor_channel_get(dev_hcsr, SENSOR_CHAN_ALL, &distance);
    	printk("MAIN CM: %d %d\n", distance.val1, distance.val2);
    }

    printk("BINDING COMPLETE\n");
}
*/