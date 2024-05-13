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
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	err = observer_start();
	if(err){
		return;
	}

	

}

struct k_timer network_form_timer;
struct k_work network_form_worker;

void network_form_handler(){
	if(nodeCount == 3){
		//k_timer_start(&network_form_timer,K_FOREVER,K_FOREVER);
		k_timer_stop(&network_form_timer);
		packet.type = FLAG_NETWORK_START_SEND;
		advertiser_restart();
		return;
	}
	advertiser_restart();

}

void network_form_callback(){
	k_work_submit(&network_form_worker);
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
	
	k_work_init(&network_form_worker,network_form_handler);
	k_timer_init(&network_form_timer,network_form_callback,NULL);
	/*start*/
	
	/*advertising, scanning loop*/
	while(1){
		BUTTON_PRESSED = (gpio_pin_get_dt(&button) > 0);

		if (BUTTON_PRESSED && (SINK_NODE==SINK_NODE_NOT_KNOWN)){
			printk("Sink node\n");
			SINK_NODE = YES_SINK_NODE;

			k_timer_start(&network_form_timer, K_NO_WAIT, K_MSEC(700));

			k_sleep(K_FOREVER);
			
		}
		k_sleep(K_SECONDS(1));
			
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}