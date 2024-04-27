#include <zephyr.h>

#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
/*________________________________________________________________________*/
/*_______________________________Advertiser______________________________*/
/*________________________________________________________________________*/

//#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define DURATION 3
int counter = 0;
/*Adv Data*/
/* Send status of the LED Data */
static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_MANUFACTURER_DATA,&counter,sizeof(counter))
};

void advertiser_restart(uint32_t start_time,char * addr_s){
	int err;
	
	err  = bt_le_adv_stop();
	if (err){
		printk("Advertising failed to stop (err %d)\n", err);
		return;
	}

	counter++;
	
	err  = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY, ad, ARRAY_SIZE(ad), NULL, 0);

	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}
	uint32_t time_ms = (((sys_clock_cycle_get_32() - start_time)*1000)/32768);
	printk("%d: %s Sending Counter: %d\n",time_ms,addr_s,counter);
	
}



