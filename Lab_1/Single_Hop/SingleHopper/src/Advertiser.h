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
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    BT_DATA_BYTES(BT_DATA_NAME_COMPLETE,
                  'L','E','D')
};

/* Send status of the LED, Scan Data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA,&BUTTON_PRESSED,sizeof(BUTTON_PRESSED)),
};
int advertiser_start(){
	/*Update data*/
	bt_le_adv_update_data(NULL, 0, sd, ARRAY_SIZE(sd));
	/* Start advertising */
	int err = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		
	}
	return err;
}

int advertiser_stop(){
	int err = bt_le_adv_stop();
	if (err)
	{
		printk("Error with stopping advertiser (err %d)\n",err);
	}
	return err;
}
void handler(struct k_timer *timer_id) {
    /* Turn off advertising */
    advertiser_stop();
	SHUTTING_DOWN=false;
    printk("Advertising turned off after %d seconds\n",DURATION);
}