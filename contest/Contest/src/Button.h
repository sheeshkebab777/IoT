#include <device.h>
#include <drivers/gpio.h>
#ifndef SHARED
#define SHARED
#include "Shared.h"
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

#define SW1_NODE	DT_ALIAS(sw1)
#if !DT_NODE_HAS_STATUS(SW1_NODE, okay)
#error "Unsupported board: sw1 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});

static struct gpio_callback button_cb_data;

static const struct gpio_dt_spec button_change = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,
							      {0});

static struct gpio_callback button_change_cb_data;

static struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});

static struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios,
						     {0});

static struct gpio_dt_spec led_3 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios,
						     {0});

static struct gpio_dt_spec led_4 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led3), gpios,
						     {0});

static struct gpio_dt_spec leds [4];

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	if(packet.type != FLAG_NETWORK_SEND){
		packet.nodeID++;
		packet.recvNodeID = packet.nodeID - 1;
		if (packet.nodeID > 4){
			packet.nodeID = 1;
			packet.recvNodeID = 0;
		}

		if(packet.nodeID == 1){
			SINK_NODE = YES_SINK_NODE;
		}
		else{
			SINK_NODE = NO_SINK_NODE;
		}

		own_node_id = packet.nodeID;

	}
	
}

void button_pressed_nothing(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	
	
}
int button_ready(){
	int ret;

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
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
	
	gpio_init_callback(&button_cb_data, button_pressed_nothing, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	ret = gpio_pin_configure_dt(&button_change, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button_change.port->name, button_change.pin);
		return 1;
	}

	ret = gpio_pin_interrupt_configure_dt(&button_change,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button_change.port->name, button_change.pin);
		return 1;
	}
	
	gpio_init_callback(&button_change_cb_data, button_pressed, BIT(button_change.pin));
	gpio_add_callback(button_change.port, &button_change_cb_data);
	printk("Set up button at %s pin %d\n", button_change.port->name, button_change.pin);
	
		if (led_1.port) {
		ret = gpio_pin_configure_dt(&led_1, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led_1.port->name, led_1.pin);
			return 1;
		} else {
			printk("Set up LED at %s pin %d\n", led_1.port->name, led_1.pin);
			
		}
		}

		if (led_2.port) {
		ret = gpio_pin_configure_dt(&led_2, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led_2.port->name, led_2.pin);
			return 1;
		} else {
			printk("Set up LED at %s pin %d\n", led_2.port->name, led_2.pin);
			
		}
		}

		if (led_3.port) {
		ret = gpio_pin_configure_dt(&led_3, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led_3.port->name, led_3.pin);
			return 1;
		} else {
			printk("Set up LED at %s pin %d\n", led_3.port->name, led_3.pin);
			
		}
		}

		if (led_4.port) {
		ret = gpio_pin_configure_dt(&led_4, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led_4.port->name, led_4.pin);
			return 1;
		} else {
			printk("Set up LED at %s pin %d\n", led_4.port->name, led_4.pin);
			return 0;
		}
		}
	return 1;
}