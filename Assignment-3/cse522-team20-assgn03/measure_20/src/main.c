// #SUSHANT TRIVEDI, ASUID: 1213366971, TEAM20
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
#define comment 1
#define LIMIT 150
#define MY_SHELL_MODULE "RESULTS"

//INTERRUPTS RELATED GLOBAL VARIABLES
#define PINMUX CONFIG_PINMUX_NAME

//INPUT PIN - INTERRUPT TRIGGERED
#define GPIO_PORT PINMUX_GALILEO_GPIO_INTEL_CW_NAME
#define PIN 0
#define PINMUX_PIN 5
#define PINFUNC PINMUX_FUNC_B

//OUTPUT PIN - PWM SETTINGS
#define PWM_PORT PINMUX_GALILEO_PWM0_NAME
#define PWMPIN 5
#define PINMUX_PWMPIN 6
#define PWMFUNC PINMUX_FUNC_C

//OUTPUT PIN - GPIO CONFIGURATION
#define GPIO_PORT2 PINMUX_GALILEO_GPIO_INTEL_CW_NAME
#define PIN2 1
#define PINMUX_PIN2 6
#define PINFUNC2 PINMUX_FUNC_A
#define STACKSIZE 1000
#define SLEEP_TIME 500
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH)



char thread_stacks[4][STACKSIZE];
static struct k_thread threads[4];
static struct gpio_callback gpio_cb;
int count=0;    //Counts the samples taken till no, Runs till Limit
int pwm_interrupt = 0; //Monitors Flag everytime PWM Interrupt is called 

//TIME MONITORING VARIABLES - INTERRUPTS
static u64_t start=0;
static u64_t end=0;
static u64_t time[3][500];
int j=0; //TRACKS WHERE TO STORE MEASUREMENTS IN ARRAY


//CONTEX SWITCH RELATED GLOBAL VARIABLES
K_MUTEX_DEFINE(mutex1);
int i_contextswitch = 0;
int low_priority_thread_executing = 0;


//MESURING CONTEXT SWITCHING LATENCY
u64_t start_contextswitch;
u64_t stop_contextswitch;
u64_t cycles_contextswitch;

//MEASURING PERIOD OF MUTEX SYSTEM CALLS
u64_t start_mutex;
u64_t stop_mutex;
u64_t cycles_mutex;


//MESSAGE QUEUE DATATYPE
struct data_item_type {
    u32_t p1;
    u32_t p2;
    u32_t p3;
}A;

K_MSGQ_DEFINE(queue, sizeof(A), 10, 4);

// MESSAGE QUEUES - COOPERATIVE THREADS
void threadsend(void)
{
    while (1)
    {
        struct data_item_type *send;
        send->p1 = 21;
        send->p2 = 22;
        send->p3 = 23;
        if (comment==1)
            {printk("Sending\n");}
        while (k_msgq_put(&queue, &send, K_NO_WAIT) != 0) 
            {k_msgq_purge(&queue);}  
        k_sleep(100);
    }
}

void threadreceive(void)
{
    struct data_item_type *receive;
    while (1) 
    {
        k_msgq_get(&queue, &receive, K_FOREVER);
        if(comment==1)
            {printk("Received\n");}
        k_sleep(100);
    }
}


//INTERRUPT CALL FUNCTION ->Same Callback for Both interrupt cases, Uses PWM_interrupt flag to decide which case.
void gpio_callfunc(struct device *gpiob, struct gpio_callback *cb,
            u32_t pins)
{
    //u64_t temp;
    //start = _tsc_read();
    end = _tsc_read();
    time[j][count] = SYS_CLOCK_HW_CYCLES_TO_NS(end - start);
    if(pwm_interrupt) 
    {
        if(comment==1){printk("PWM Interrupt triggered %d Time(ns): %llu Start: %llu End: %llu\n", count, time[j][count], start, end);}       
        count = count+1;
    }
    else 
    {
        if(comment==1){printk("GPIO Interrupt triggered %d Time(ns): %llu Start: %llu End: %llu\n", count, time[j][count], start, end);}
        count = count+1;
    }

}

//CALCULATE INTERRUPT LATENCY OF BOTH CASES HERE ONE AFTER ANOTHER
void calculate_interupt_latency() 
{
    //Device pointer declarations
    struct device *dev_gpio;
    struct device *dev_pinmux;
    struct device *dev_pwm;
    struct device *dev_gpio2;
    struct device *dev_pinmux2;
    printk("Hello Board Started\n");
    printk("INTERRUPT LATENCY - WITHOUT BACKGROUND COMPUTING STARTED\n");
    j=0;   //TRACKS WHERE TO STORE MEASUREMENTS IN ARRAY
    

    // INPUT PIN - BINDING AS PER AN INPUT (IO5)
    //FIRST -> PINMUX GET BINDING, SECOND -> PINMUX PIN SET, THIRD -> GPIO DRIVER GET BINDING
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
    if (!dev_gpio) 
    {
        printk("Error: Get Binding\n");
        return;
    }

    //OUTPUT PIN - CONFIGURED AS GPIO
    //FIRST -> PINMUX GET BINDING, SECOND -> PINMUX PIN SET, THIRD -> GPIO DRIVER GET BINDING
    dev_pinmux2 = device_get_binding(PINMUX);
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
    if (!dev_gpio2) {
        printk("Error: Get Binding\n");
        return;
    }
    
    //INPUT PIN CONFIGURATION - CONFIGURED AS INTERRUPT ENABLED PIN
    gpio_pin_configure(dev_gpio, PIN, GPIO_DIR_IN | GPIO_INT | EDGE);
    gpio_init_callback(&gpio_cb, gpio_callfunc, BIT(PIN));
    gpio_add_callback(dev_gpio, &gpio_cb);
    gpio_pin_enable_callback(dev_gpio, PIN);


    //OUTPUT PIN CONFIGURATION - CONFIGURED AS GPIO HERE
    gpio_pin_configure(dev_gpio2, PIN2, GPIO_DIR_OUT);
    if(gpio_pin_write(dev_gpio2, PIN2, 0))
        {printk("Output Write Failed");}
    
    
    while(count<LIMIT)
    {
        //ALTERNATIVELY WRITING LOW HIGH TO TRIGGER THE INTERRUPT
        start = _tsc_read();
        if(gpio_pin_write(dev_gpio2, PIN2, 0))
        {printk("Output Write Failed");}
        if(gpio_pin_write(dev_gpio2, PIN2, 1))
        {printk("Output Write Failed");}
    }
printk("INTERRUPT LATENCY - WITH BACKGROUND COMPUTING STARTED\n");

    j=1;   //TRACKS WHERE TO STORE MEASUREMENTS IN ARRAY
    

    //RECONFIGURE OUTPUT PIN AS PWM
    //OUTPUT PIN - CONFIGURED AS PWM
    //FIRST -> PINMUX PIN SET, THIRD -> PWM DRIVER GET BINDING
    if(pinmux_pin_set(dev_pinmux2, PINMUX_PWMPIN, PWMFUNC))
    {
        printk("Failed to Set Pinmux2 Pin");
        return;
    }

    dev_pwm = device_get_binding(PWM_PORT);
    if (!dev_pwm) {
        printk("Error: Get Binding\n");
        return;
    }


    //THREAD CREATE - THESE ARE MESSAGE QUEUE THREADS 
    k_thread_create(&threads[0], &thread_stacks[0][0], STACKSIZE, threadsend, 0, 0, 0, K_PRIO_COOP(4), 0, 0);
    k_thread_create(&threads[1], &thread_stacks[1][0], STACKSIZE, threadreceive, 0, 0, 0, K_PRIO_COOP(4), 0, 0); 

    //RUNNING FOR AS MANY TIMES AS REQUIRED
    count=0; 
    int disable = 0;
    while(!disable) 
    {
        start = _tsc_read();
        pwm_pin_set_cycles(dev_pwm, PWMPIN,4095,2000);
        printk("Loop Entered\n");
        pwm_interrupt = 1;
        if(count>=LIMIT)
        {
            if(comment==1){printk("DISABLING INTERRUPTS\n");}
            disable = 1;
            gpio_pin_disable_callback(dev_gpio, PIN);
        }
    }


    // BACKGROUND MESSAGE QUEUE THREADS DESTROYED
    k_thread_abort(&threads[0]);
    k_thread_abort(&threads[1]);

}

//CONTEX SWITCHING THREAD FUNCTION DEFINITION HERE
void cs_thread1(void) //HIGH PRIORITY
{
    while (i_contextswitch < LIMIT) 
    {
        k_mutex_lock(&mutex1, K_FOREVER);
        if(low_priority_thread_executing) 
        {
            stop_contextswitch = _tsc_read();
            start_mutex = _tsc_read();
            
            k_mutex_lock(&mutex1, K_FOREVER);
            k_mutex_unlock(&mutex1);
            
            stop_mutex = _tsc_read();
            cycles_mutex = stop_mutex - start_mutex;
            cycles_contextswitch = stop_contextswitch - start_contextswitch - cycles_mutex;
            time[2][i_contextswitch] = SYS_CLOCK_HW_CYCLES_TO_NS(cycles_contextswitch);
            
            if(comment==1){printk("Context Switched %d\n",time[2][i_contextswitch]);}
            i_contextswitch++;
        }
        k_mutex_unlock(&mutex1);
        k_sleep(800);
    }
    
}

void cs_thread2(void)  //LOW PRIORITY THREAD
{
    while (i_contextswitch < LIMIT) 
    {
        low_priority_thread_executing = 1;
        k_mutex_lock(&mutex1, K_FOREVER);
        k_sleep(600);
        start_contextswitch = _tsc_read();
        k_mutex_unlock(&mutex1);
        low_priority_thread_executing = 1;
    }
    
}

//CONTEXT SWITCH OVERHEAD MEASUREMENT CODE
void measure_context_switching_overhead() 
{
    j=2; //TRACKS WHERE TO STORE MEASUREMENTS IN ARRAY
    k_thread_create(&threads[2], &thread_stacks[2][0], STACKSIZE, cs_thread1, NULL, NULL, NULL, 2, 0, 0);
    k_thread_create(&threads[3], &thread_stacks[3][0], STACKSIZE, cs_thread2, NULL, NULL, NULL, 4, 0, 0);
    k_sleep(1000000);
    k_thread_abort(&threads[2]);
    k_thread_abort(&threads[3]);

}

//SHELL FUNCTIONS 
static int shell_interuptlatency(int argc, char *argv[])
{
    
    for(int i=0;i<LIMIT;i++)
    {
        printk("%llu\t",time[0][i]);
        if((i%15) == 0) 
        {
        printk("\n");
        }
    }
    printk("\n");
    return 0;
}

static int shell_interupt_bg(int argc, char *argv[])
{
    for(int i=0;i<LIMIT;i++)    
    {
      printk("%llu\t",time[1][i]);
      if((i%15) == 0) 
      {
            printk("\n");
      }
    }
    printk("\n");
    return 0;
}

static int shell_contextswitch(int argc, char *argv[])
{
   for(int i=0;i<LIMIT;i++)
   {
        printk("%llu\t",time[2][i]);
        if((i%15) == 0) 
        {
            printk("\n");
        }
    }
    printk("\n");
    return 0;
}

//SHELL DATASTRUCTURE
static struct shell_cmd commands[] = 
{
    { "1",shell_interuptlatency, "Interrupt Latency Without Background Task" },
    { "2",shell_interupt_bg, "Interrupt Latency With Background Task" },
    { "3",shell_contextswitch, "Context Switching Overhead" },
    { NULL, NULL, NULL }
};


void main(void)
{
   //CALLING INTERRUPT LATENCY ANALYSIS FUNCTION
   calculate_interupt_latency();

   //CALLING CONTEXT SWITCHING ANALYSIS FUNCTION
   printk("\nCONTEXT SWITCHING ANALYSIS STARTED\n");
   measure_context_switching_overhead();

   //SHELL MODULE
   SHELL_REGISTER(MY_SHELL_MODULE, commands);
   k_sleep(60000);
   printk("CODE ENDED\n");
}