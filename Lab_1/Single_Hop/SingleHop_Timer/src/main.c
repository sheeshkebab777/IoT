#include <random/rand32.h>
#include <kernel.h>
#include "Advertiser.h"
#include "Observer.h"

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
    advertiser_restart(start_time,addr_s);
}

void timer_callback(struct k_timer *timer) {
    // Queue the work to the work queue
    k_work_submit(&stop_ble_work);
}

static void bt_ready(int err)
{
	
	bt_addr_le_t addr = {0};
	size_t count = 1;
	bt_id_get(&addr,&count);

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
struct k_timer timer;

int main(void)
{	
	start_time = sys_clock_cycle_get_32();
	int err;
	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem*/
	err = bt_enable(bt_ready);
	if (err) { return 0;}

	/*advertising random timer through a workqueue thread*/
	k_work_init(&stop_ble_work, stop_ble_handler);

	uint32_t rand = ADV_INTERVAL_MIN + sys_rand32_get()%(ADV_INTERVAL_MAX - ADV_INTERVAL_MIN);
	k_timer_init(&timer, timer_callback, NULL);
	k_timer_start(&timer, K_MSEC(rand), K_MSEC(rand));
	printk("Started timer with %d MSECS\n",rand);

	while(1){
		k_sleep(K_FOREVER);
	}
	

	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}