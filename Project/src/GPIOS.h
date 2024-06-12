#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#ifndef SHARED
#define SHARED
#include "Shared.h"
#endif

#ifndef ADVERTISER
#define ADVERTISER
#include "Advertiser.h"
#endif
/*Source: https://github.com/zephyrproject-rtos/zephyr/blob/main/samples/basic/button/src/main.c*/
/*BUTTON*/
/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
/*__________________________________________________________________*/
/*_______________________________BUTTON_____________________________*/
/*__________________________________________________________________*/
#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_1 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});

static struct gpio_callback button_1_cb_data;

#define SW1_NODE	DT_ALIAS(sw1)
#if !DT_NODE_HAS_STATUS(SW1_NODE, okay)
#error "Unsupported board: sw1 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_2 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,
							      {0});

static struct gpio_callback button_2_cb_data;

#define SW2_NODE	DT_ALIAS(sw2)
#if !DT_NODE_HAS_STATUS(SW2_NODE, okay)
#error "Unsupported board: sw2 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_3 = GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios,
							      {0});

static struct gpio_callback button_3_cb_data;

#define SW3_NODE	DT_ALIAS(sw3)
#if !DT_NODE_HAS_STATUS(SW3_NODE, okay)
#error "Unsupported board: sw3 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec button_4 = GPIO_DT_SPEC_GET_OR(SW3_NODE, gpios,
							      {0});

static struct gpio_callback button_4_cb_data;


static struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});

static struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios,
						     {0});

static struct gpio_dt_spec led_3 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios,
						     {0});

static struct gpio_dt_spec led_4 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led3), gpios,
						     {0});


void button_1_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	if(sender){
		sender = false;
	}
	else{
		sender = true;
	}
}

void button_2_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	packet.type = TURN_OFF_T;
	advertiser_start();
}

void button_3_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	packet.type = BLACK_TEA_T;
	advertiser_start();
}

void button_4_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	packet.type = GREEN_TEA_T;
	advertiser_start();
}
int button_ready(){
	int ret;
	typedef void (*FuncPtr)(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

	FuncPtr functions[] = {button_1_pressed,button_2_pressed,button_3_pressed,button_4_pressed};

	struct gpio_dt_spec leds [] = {led_1,led_2,led_3,led_4};

	struct gpio_dt_spec buttons[] = {button_1,button_2,button_3,button_4};

	struct gpio_callback button_callbacks []= {button_1_cb_data,button_2_cb_data,button_3_cb_data,button_4_cb_data};


	//init buttons
	for (size_t i = 0; i < 4; i++)
	{
		struct gpio_dt_spec button = buttons[i];
		struct gpio_callback button_callback = button_callbacks[i];
		FuncPtr func = functions[i];

		ret = gpio_pin_configure_dt(&button,GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n",
				ret, button.port->name, button.pin);
			return 1;
		}

		ret = gpio_pin_interrupt_configure_dt(&button,
							GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n",
				ret, button.port->name, button.pin);
			return 1;
		}
		
		gpio_init_callback(&button_callback, button_1_pressed, BIT(button_1.pin));
		gpio_add_callback(button.port, &func);
		printk("Set up button at %s pin %d\n", button.port->name, button.pin);
	}
	
	for (size_t i = 0; i < 4; i++)
	{
		struct gpio_dt_spec led = leds[i];
		if (led.port) {
			ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
			if (ret != 0) {
				printk("Error %d: failed to configure LED device %s pin %d\n",
					ret, led.port->name, led.pin);
				return 1;
			} else {
				printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
				
			}
		}
	}

	dev = device_get_binding("OW0");
    if (!dev) {
        printk("DS18B20 device not found\n");
        return 1;
    }

	return 0;
}