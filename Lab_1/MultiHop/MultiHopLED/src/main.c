#include <kernel.h>
#include <device.h>
#include <inttypes.h>
#include <random/rand32.h>

#include "Advertiser.h"
#include "Observer.h"
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

static void bt_ready(int err)
{
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};
	size_t count = 1;
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");
	bt_id_get(&addr,&count);
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	err = observer_start();
	if(err){
		return;
	}


	printk("Observing as %s\n", addr_s);
	

}

bool BUTTON_ALRDY_PRESSED= false; //value, that checks if button was already pressed
bool first_button_press = true;
int main(void)
{
	int err;
	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem*/
	err = bt_enable(bt_ready);
	if (err) { return 0;}
	err = button_ready();
	if(err) { return 0;}
	
	k_work_init(&work,stop_ble_handler);
	k_timer_init(&timer,timer_callback,NULL);
	gpio_pin_set_dt(&led,0);
	
	/*advertising, scanning loop*/
	if(led.port){
		
		while(1){
			BUTTON_PRESSED = (gpio_pin_get_dt(&button) > 0);

			if (first_button_press && sink_node && BUTTON_PRESSED){
				printk("Root node\n");
				err = observer_stop();
				if (err){return 0;}
				first_button_press = false;
			}
			/*only advertise the led status if you are the sink node*/
			if(sink_node && !first_button_press){
					
				/*First buttonpress*/
				if(!BUTTON_ALRDY_PRESSED && BUTTON_PRESSED){
					printk("BUTTON PRESSED\n");
					BUTTON_ALRDY_PRESSED = true;
					//update button is pressed and advertise
					advertiser_restart();
				}

				/*Button released*/
				else if(BUTTON_ALRDY_PRESSED && !BUTTON_PRESSED){
					printk("BUTTON RELEASED\n");
					BUTTON_ALRDY_PRESSED = false;
					//update, button is not pressed anymore and advertise
					advertiser_restart();

				}
				
			}
			k_sleep(K_SECONDS(1));
			
	}
	}

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}