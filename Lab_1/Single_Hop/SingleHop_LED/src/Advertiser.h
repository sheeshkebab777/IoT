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

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define DURATION 3

bool SHUTTING_DOWN = false;
bool BUTTON_PRESSED = false;

/*Adv Data*/
/* Send status of the LED Data */
static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_MANUFACTURER_DATA,&BUTTON_PRESSED,sizeof(BUTTON_PRESSED))
};


int advertiser_restart(){
	int err = bt_le_adv_stop();
	if(err){
		printk("Stop Advertising failed (err %d)\n", err);
		return err;	
	}
	/* Start advertising */
	/*Advertise in interval (30,150) ms*/
	err  = bt_le_adv_start(BT_LE_ADV_PARAM(0, BT_GAP_ADV_FAST_INT_MIN_1,
					BT_GAP_ADV_FAST_INT_MAX_2, NULL),
					ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return err;
	}


	return 0;
}

/*
void update_ad(){
	memcpy((void*)ad[1].data,&BUTTON_PRESSED,sizeof(BUTTON_PRESSED));
	bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL,0);
}*/