#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>


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
volatile static double celsius = 0.0;

void cook_till(struct k_work *work){
	cook = true;
	int ret;
	// ! turn on gpio pin
	gpio_pin_set(power_supply,POWER_SUPPLY_PIN_NUMBER,1);
	
	while(cook){
		k_sleep(K_MSEC(100));
		
		ret = sensor_sample_fetch(temp_dev);
		if (ret) {
			printk("Failed to fetch sample: %d\n", ret);
		}
		struct sensor_value temp;
		ret = sensor_channel_get(temp_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		if (ret) {
			printk("Failed to get channel: %d\n", ret);
		}
		double cel = sensor_value_to_double(&temp);

		if(cel >= celsius){
			cook = false;
		}
		printk("Temp: %d.%06d\n", temp.val1, temp.val2);
		
	}
	// ! turn off gpio pin
	gpio_pin_set(power_supply,POWER_SUPPLY_PIN_NUMBER,0);
	printk("Turned off cooking...\n");
}

/*Advertisment found*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	struct packet pack;
	bt_data_parse(ad,data_cb,&pack);
	
	if(pack.password != password) return;
	already_set = true;
	printk("hallo\n");
	//receiver
	if(!sender){
		

		switch(pack.type){
			case TURN_OFF_T: {
				printk("Turning kettle off...\n");
				cook = false;
				break;
			}
			case BLACK_TEA_T:{
				celsius = 100.0;
				if(cook){
					printk("Already cooking...\n");
					break;
				}
				printk("Cooking until black tea temperature is reached...\n");
				k_work_submit(&run_cooker_work);
				printk("moinn\n");
				break;
			}
			case GREEN_TEA_T:{
				celsius = 80.0;
				if(cook){
					printk("Already cooking...\n");
					break;
				}
				printk("Cooking until green tea temperature is reached...\n");
				k_work_submit(&run_cooker_work);
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

