#include <kernel.h>
#include <device.h>
#include <inttypes.h>
#include <random/rand32.h>

#include "Advertiser.h"
#include "Observer.h"

/*___________________________________________________________________*/
/*_______________________________Main________________________________*/
/*___________________________________________________________________*/
struct k_work init_worker;
struct k_timer init_timer;

void init_handler(struct k_work *work) {
    printk("Sink node\n");
	
	SINK_NODE = YES_SINK_NODE;
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

int main(void)
{
	int err;
	

	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem*/
	err = bt_enable(bt_ready);
	if (err) { return 0;}
	
	/*init packet*/
	init_packet();

	k_work_init(&work,stop_ble_handler);
	k_timer_init(&timer,timer_callback,NULL);
	
	/*start*/
	k_work_init(&init_worker,init_handler);
	k_timer_init(&init_timer,init_callback,NULL);
	k_timer_start(&init_timer,K_SECONDS(1),K_FOREVER);
	/*advertising, scanning loop*/
	while(1){
		k_sleep(K_FOREVER);
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}