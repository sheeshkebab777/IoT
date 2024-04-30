#include <zephyr.h>

#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <drivers/gpio.h>
/*______________________________________________________________________*/
/*_______________________________Observer_______________________________*/
/*______________________________________________________________________*/
static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});

bool sink_node = true;


static bool data_cb(struct bt_data *data, void *user_data)
{
	bool *BUTTON_STATUS = user_data;

	switch (data->type) {
	case BT_DATA_MANUFACTURER_DATA:
		memcpy(BUTTON_STATUS,data->data,sizeof(bool));
		return false;
	default:
		return true;
	}
}
#define NAME_LEN 30
/*Advertisment found*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	/*dont process packet if currently advertising*/
	if(advertising){return;}
	char addr_str[BT_ADDR_LE_STR_LEN];
	
	sink_node = false;

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	//printk("Received advertisment from: %s\n",addr_str);

	//LED logic
	bool BUTTON_STATUS;
	bt_data_parse(ad, data_cb, &BUTTON_STATUS);
	
	gpio_pin_set_dt(&led,(int)BUTTON_STATUS);
	printk("LED status: %d\n",BUTTON_STATUS);
	BUTTON_PRESSED = BUTTON_STATUS;
	/*advertise update to nearby nodes*/
	advertiser_restart();
	return;
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
