#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/devicetree.h>


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


#define GPIO_PIN_0 DT_ALIAS(gpio_pin_0)
#define GPIO_PIN_1 DT_ALIAS(gpio_pin_1)


void button_1_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	if (already_set){return;}
	if(sender){
		sender = false;
		printk("Cooker\n");
		gpio_pin_set_dt(&led_1,1);

	}
	else{
		sender = true;
		printk("Sender\n");
		gpio_pin_set_dt(&led_1,0);
	}
	//gpio_pin_toggle(power_supply,PIN_NUMBER);
}

void button_2_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	if(!sender){ return;}
	packet.type = TURN_OFF_T;
	printk("Send: Turn off\n");
	k_work_submit(&advertiser_start_work);
}

void button_3_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	if(!sender){ return;}
	packet.type = BLACK_TEA_T;
	printk("Send: Black Tea\n");
	k_work_submit(&advertiser_start_work);
}

void button_4_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	if(!sender){ return;}
	packet.type = GREEN_TEA_T;
	printk("Send: Green Tea\n");
	k_work_submit(&advertiser_start_work);
	
}
static const struct device *get_ds18b20_device(void)
{
	const struct device *const dev = DEVICE_DT_GET_ANY(maxim_ds18b20);

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}

	printk("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

int button_ready(){
	int ret;

	struct gpio_dt_spec leds [] = {led_1,led_2,led_3,led_4};
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
			gpio_pin_set_dt(&led,0);
		}
	}
		
	// ! button 1
		ret = gpio_pin_configure_dt(&button_1,GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n",
				ret, button_1.port->name, button_1.pin);
			return 1;
		}

		ret = gpio_pin_interrupt_configure_dt(&button_1,
							GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n",
				ret, button_1.port->name, button_1.pin);
			return 1;
		}

		gpio_init_callback(&button_1_cb_data, button_1_pressed, BIT(button_1.pin));
		gpio_add_callback(button_1.port, &button_1_cb_data);
		printk("Set up button at %s pin %d\n", button_1.port->name, button_1.pin);
		
		// ! button 2
		ret = gpio_pin_configure_dt(&button_2,GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n",
				ret, button_2.port->name, button_2.pin);
			return 1;
		}

		ret = gpio_pin_interrupt_configure_dt(&button_2,
							GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n",
				ret, button_2.port->name, button_2.pin);
			return 1;
		}
		
		gpio_init_callback(&button_2_cb_data, button_2_pressed, BIT(button_2.pin));
		gpio_add_callback(button_2.port, &button_2_cb_data);
		printk("Set up button at %s pin %d\n", button_2.port->name, button_2.pin);

		// ! Button 3

		ret = gpio_pin_configure_dt(&button_3, GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n", ret, button_3.port->name, button_3.pin);
			return 1;
		}

		ret = gpio_pin_interrupt_configure_dt(&button_3, GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, button_3.port->name, button_3.pin);
			return 1;
		}
		gpio_init_callback(&button_3_cb_data, button_3_pressed, BIT(button_3.pin));
		gpio_add_callback(button_3.port, &button_3_cb_data);
		printk("Set up button at %s pin %d\n", button_3.port->name, button_3.pin);
		// ! Button 4
		

		ret = gpio_pin_configure_dt(&button_4, GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure %s pin %d\n", ret, button_4.port->name, button_4.pin);
			return 1;
		}

		ret = gpio_pin_interrupt_configure_dt(&button_4, GPIO_INT_EDGE_TO_ACTIVE);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, button_4.port->name, button_4.pin);
			return 1;
		}

		gpio_init_callback(&button_4_cb_data, button_4_pressed, BIT(button_4.pin));
		gpio_add_callback(button_4.port, &button_4_cb_data);
		printk("Set up button at %s pin %d\n", button_4.port->name, button_4.pin);
	
    power_supply  = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    if (!power_supply) {
        printk("Error: didn't find %s device\n", PORT);
        return;
    }

	ret = gpio_pin_configure(power_supply, POWER_SUPPLY_PIN_NUMBER, GPIO_OUTPUT);
    if (ret < 0) {
        printk("Error %d: failed to configure pin %d\n", ret, POWER_SUPPLY_PIN_NUMBER);
        return;
    }

	const struct device *dev = get_ds18b20_device();

	if (dev == NULL) {
		return 0;
	}

	while (1) {
		struct sensor_value temp;

		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);

		printk("Temp: %d.%06d\n", temp.val1, temp.val2);
		k_sleep(K_MSEC(2000));
	}

	return 0;
}