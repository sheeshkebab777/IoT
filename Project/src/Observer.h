#include <zephyr.h>

#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <drivers/sensor.h>
#include <drivers/gpio.h>


#ifndef SHARED
#define SHARED
#include "Shared.h"
#endif
/*______________________________________________________________________*/
/*_______________________________Observer_______________________________*/
/*______________________________________________________________________*/

static bool data_cb(struct bt_data *data, void *user_data)
{
	struct packet * pack = user_data;

	switch (data->type) {
	case BT_DATA_MANUFACTURER_DATA:
		memcpy(pack,data->data,sizeof(struct packet));
		return false;
	default:
		return true;
	}
}

volatile static bool cook = false;

static void cook_till(uint8_t celsius){
	cook = true;
	int ret;
	while(cook){
		ret = sensor_sample_fetch(dev);
		if (ret) {
			printk("Failed to fetch sample: %d\n", ret);
		}
		struct sensor_value temp;
		ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		if (ret) {
			printk("Failed to get channel: %d\n", ret);
		}

		if(temp.val1 >= celsius){
			cook = false;
			//turn off gpio pin
		}
		printk("Temp: %d.%06d\n", temp.val1, temp.val2);
		k_sleep(K_SECONDS(2));

	}

	printk("Turned off cooking...\n");
}

/*Advertisment found*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	struct packet pack;
	bt_data_parse(ad,data_cb,&pack);

	if(pack.password != password) return;

	//receiver
	if(!sender){
		switch(pack.type){
			case TURN_OFF_T: {
				printk("Turning kettle off...\n");
				cook = false;
				break;
			}
			case BLACK_TEA_T:{
				printk("Cooking until black tea temperature is reached...\n");
				cook_till(100);
				break;
			}
			case GREEN_TEA_T:{
				printk("Cooking until green tea temperature is reached...\n");
				cook_till(80);
				break;
			}
			default:{
				printk("Default, turning off...\n");
				cook = false;
				break;
			}
		}
	}
	//sender
	else{
		printk("Received stuff from cooker...\n");
	}
	
	
}

int observer_start()
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_PASSIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Start observing failed (err %d)\n", err);
		return err;
	}
	printk("Started observing...\n");

	return 0;
}

int observer_stop(){
	int err = bt_le_scan_stop();
	
	if (err){
		printk("Stop observing failed (err %d)\n", err);
		return err;
	}
	return 0;
}

