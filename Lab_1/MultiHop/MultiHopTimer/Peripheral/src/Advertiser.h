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
struct k_timer adv_timer;
#define ADV_DURATION 300
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
int counter = 0;
bool advertising = false;
/*Adv Data*/
/* Send status of the LED Data */
static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_MANUFACTURER_DATA,&counter,sizeof(counter))
};
int advertiser_stop(){
	int err;
	err  = bt_le_adv_stop();
	if (err){
		printk("Advertising failed to stop (err %d)\n", err);
		return err;
	}
	advertising = false;
	return 0;
}

void advertiser_restart(){
	int err;
	
	err = advertiser_stop();
	if(err){ return;}

	err  = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY, ad, ARRAY_SIZE(ad), NULL, 0);

	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}
	advertising = true;
	//advertise for ADV_DURATION mseconds and then stop
	k_timer_start(&adv_timer, K_MSEC(ADV_DURATION), K_FOREVER);
	
}





