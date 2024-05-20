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

#define STACK_SIZE 1024
#define THREAD_PRIORITY 7
/*___________________________________________________________________*/
/*_______________________________Main________________________________*/
/*___________________________________________________________________*/
void toggle_led(){
	if(gpio_pin_get_dt(&led) > 0){
		gpio_pin_set_dt(&led,1);
	}
	else{
		gpio_pin_set_dt(&led,0);
	}
	
}

void led_thread_function(void *arg1, void *arg2, void *arg3)
{
    while (1) {
        k_sleep(K_MSEC(400*packet.nodeID));
		toggle_led();
        
    }
}

struct k_work init_worker;
struct k_timer init_timer;

void init_handler(struct k_work *work) {
    
	printk("Start sending\n");

	packet.type = FLAG_NETWORK_START_SEND;
	ADV_DURATION = 1000;
	advertiser_restart();
	
}

void init_callback(struct k_timer *timer) {
    // Queue the work to the work queue
    k_work_submit(&init_worker);
}

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
bool ALREADY_PRESSED = false;
struct k_thread led_thread;
K_THREAD_STACK_DEFINE(led_thread_stack, STACK_SIZE);
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
	
	k_work_init(&init_worker,init_handler);
	k_timer_init(&init_timer,init_callback,NULL);
	/*led thread*/
	k_tid_t led_thread_tid = k_thread_create(&led_thread, led_thread_stack,
                                            K_THREAD_STACK_SIZEOF(led_thread_stack),
                                            led_thread_function, NULL, NULL, NULL,
                                            THREAD_PRIORITY, 0, K_NO_WAIT);
	
	/*start*/

	
	/*advertising, scanning loop*/
	while(1){

		BUTTON_PRESSED = (gpio_pin_get_dt(&button) > 0);

		if (BUTTON_PRESSED && SINK_NODE==YES_SINK_NODE && !ALREADY_PRESSED){
			ALREADY_PRESSED = true;
			k_timer_start(&init_timer, K_NO_WAIT, K_FOREVER);
			
		}

		k_sleep(K_MSEC(300));
			
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}