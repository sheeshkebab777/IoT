#include <zephyr.h>

#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <random/rand32.h>

#ifndef SHARED
#define SHARED
#include "Shared.h"
#endif
/*________________________________________________________________________*/
/*_______________________________Advertiser______________________________*/
/*________________________________________________________________________*/

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
uint16_t ADV_DURATION = 200;
struct k_timer advertiser_timer;
struct k_work advertiser_worker;
bool advertising = false;


/*Adv Data*/
/* Send status of the LED Data */
static struct bt_data ad[] = {
	//BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_MANUFACTURER_DATA,&packet,sizeof(packet))
};

uint32_t get_ms(){
	return (((sys_clock_cycle_get_32())*1000)/32768);
}

int advertiser_stop(){
	int err = bt_le_adv_stop();
	advertising = false;
	if (err){
		printk("Stop advertising failed (err %d)\n", err);
		return err;
	}
	return 0;
	
}


int advertiser_start(){
	/* Start advertising */
	/*Advertise in interval (30,60) ms*/
	int err = advertiser_stop();
	if (err){
		printk("Stop advertising failed (err %d)\n", err);
		return err;
	}

	err  = bt_le_adv_start(BT_LE_ADV_PARAM(0, BT_GAP_ADV_FAST_INT_MIN_1,
					BT_GAP_ADV_FAST_INT_MAX_2, NULL),
					ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return err;
	}

	advertising = true;
	k_timer_start(&advertiser_timer,K_MSEC(ADV_DURATION),K_FOREVER);


	return 0;
}



void stop_ble_handler(struct k_work *work) {
    // Call stop_ble() here
    advertiser_stop();
}

void timer_callback(struct k_timer *timer) {
    // Queue the work to the work queue
    k_work_submit(&advertiser_worker);
}

