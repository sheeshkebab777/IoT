#include <kernel.h>
#include <device.h>
#include <inttypes.h>

#include "Advertiser.h"
#include "Observer.h"
/*Source: https://github.com/zephyrproject-rtos/zephyr/blob/main/samples/basic/button/src/main.c*/
/*BUTTON*/
/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button_cb_data;


void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

}

int button_ready(){
	int ret;
	/*
	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return 1;
	}
	*/
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
	
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);
	/*
	if (led.port && !gpio_is_ready_dt(&led)) {
		printk("Error %d: LED device %s is not ready; abort \n",
		       ret, led.port->name);
		return 1;
	}*/
	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			return 1;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
			return 0;
		}
	}
	return 1;
}
/*___________________________________________________________________*/
/*_______________________________Other_______________________________*/
/*___________________________________________________________________*/
void print_led_status(){
	if(LED_ON){
		printk("LED ON as\n");
	}
	else{
		printk("LED OFF as\n");
	}
}

/*At the beginning only scan for incoming adverts*/
static void bt_ready(int err)
{
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};

	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	err = observer_start();
	if(err){
		return;
	}

	printk("Observing as %s\n", addr_s);

}

int BUTTON_VAL = 2; //value, that checks if button was already pressed
bool final_adv_active = false;
struct k_timer advertising_timer;
int button_val;
int led_val;
int main(void)
{
	int err;

	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem and only start scanning*/
	err = bt_enable(bt_ready);
	if (err) {
		return 0;
	}
	err = button_ready();
	if(err) {return 0 ;
	};
	/*advertising, scanning loop*/
	if(led.port){
		
		while(1){
		
		button_val = gpio_pin_get_dt(&button);
		BUTTON_PRESSED = (button_val > 0);
		led_val = gpio_pin_get_dt(&led);
		LED_ON = (led_val>0);	
		
		int err;

		/*First buttonpress*/
		if((BUTTON_VAL!=button_val) && BUTTON_PRESSED && !LED_ON){
			printk("BUTTON PRESSED\n");
			err = observer_stop();
			if (err) { return err;}
			
			err = advertiser_start();
			if(err) { return err; }	

			BUTTON_VAL=button_val;
		}

		/*Button released*/
		if((BUTTON_VAL==button_val) && !BUTTON_PRESSED){
			printk("BUTTON RELEASED\n");
			BUTTON_VAL = 0;
			//update, button is not pressed anymore
			bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL,0);
			/*advertise the button not pressed information for DURATION seconds, before shutting down*/
			SHUTTING_DOWN = true;
			k_timer_init(&advertising_timer, handler, NULL);
			k_timer_start(&advertising_timer, K_SECONDS(DURATION), K_NO_WAIT);

			while(SHUTTING_DOWN){

			}

			/*Turn back to observer mode*/
			err = observer_start();
			if (err) { return err;}
			
			err = advertiser_stop();
			if(err) { return err; }
		}
		//fix: dont print that often
		//print_led_status();


	}
	}

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}