#include <random/rand32.h>
#include <kernel.h>
#include "Advertiser.h"

//MSECS
#define ADV_INTERVAL_MIN    500    
#define ADV_INTERVAL_MAX    700 
/*___________________________________________________________________*/
/*_______________________________Other_______________________________*/
/*___________________________________________________________________*/
struct k_work stop_ble_work;
char addr_s[BT_ADDR_LE_STR_LEN];

void stop_ble_handler(struct k_work *work) {
    // Call stop_ble() here
    advertiser_restart(addr_s);
}

void timer_callback(struct k_timer *timer) {
    // Queue the work to the work queue
    k_work_submit(&stop_ble_work);
}


struct k_timer timer;

int main(void)
{	
	
	int err;
	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem*/
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}
	printk("Bluetooth initialized\n");

	/*advertising random timer through a workqueue thread*/
	k_work_init(&stop_ble_work, stop_ble_handler);

	uint32_t rand = ADV_INTERVAL_MIN + sys_rand32_get()%(ADV_INTERVAL_MAX - ADV_INTERVAL_MIN);
	k_timer_init(&timer, timer_callback, NULL);
	k_timer_start(&timer, K_MSEC(rand), K_MSEC(500));
	printk("Started timer with %d MSECS\n",rand);

	while(1){
		k_sleep(K_FOREVER);
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}