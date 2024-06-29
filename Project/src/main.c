#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <inttypes.h>
#include <zephyr/random/rand32.h>

#ifndef ADVERTISER
#define ADVERTISER
#include "Advertiser.h"
#endif
#include "Observer.h"

#ifndef LED
#define LED
#include "GPIOS.h"
#endif
/*___________________________________________________________________*/
/*_______________________________Main________________________________*/
/*___________________________________________________________________*/


static void bt_ready(int err)
{
	//char addr_s[BT_ADDR_LE_STR_LEN];
	//bt_addr_le_t addr = {0};
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");
	//bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	err = observer_start();
	if(err){
		return;
	}


	printk("Observing\n");
	

}

int main(void)
{
	int err;

	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem*/
	err = bt_enable(bt_ready);
	if (err) { return 0;}
	err = button_ready();
	if(err) { return 0;}
	
	k_work_init(&advertiser_worker,stop_ble_handler);
	k_timer_init(&advertiser_timer,timer_callback,NULL);

	k_work_init(&advertiser_start_work,advertiser_start_worker);
	k_work_init(&run_cooker_work,cook_till);
	
	/*advertising, scanning loop*/
	while(true){
		k_sleep(K_MSEC(300));
			
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}