#include <kernel.h>
#include <device.h>
#include <inttypes.h>
#include <random/rand32.h>

#include "Advertiser.h"
#include "Observer.h"

#ifndef LED
#define LED
#include "Button.h"
#endif
/*___________________________________________________________________*/
/*_______________________________Main________________________________*/
/*___________________________________________________________________*/

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

int main(void)
{
	int err;
	bool BUTTON_PRESSED = false;

	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem*/
	err = bt_enable(bt_ready);
	if (err) { return 0;}
	err = button_ready();
	if(err) { return 0;}
	
	/*init packet*/
	init_packet();

	k_work_init(&work,stop_ble_handler);
	k_timer_init(&timer,timer_callback,NULL);
	
	/*start*/
	
	/*advertising, scanning loop*/
	while(1){
		BUTTON_PRESSED = (gpio_pin_get_dt(&button) > 0);

		if (BUTTON_PRESSED && (SINK_NODE==SINK_NODE_NOT_KNOWN)){
			printk("Sink node\n");
			SINK_NODE = YES_SINK_NODE;
			advertiser_restart();
		}
		k_sleep(K_SECONDS(1));
			
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}