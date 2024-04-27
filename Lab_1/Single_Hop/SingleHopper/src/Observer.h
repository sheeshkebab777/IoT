#include <zephyr.h>

#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <drivers/gpio.h>
/*______________________________________________________________________*/
/*_______________________________Observer_______________________________*/
/*______________________________________________________________________*/

bool LED_ON = false;
static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});
static bool data_cb(struct bt_data *data, void *user_data)
{
	bool *BUTTON_STATUS = user_data;

	switch (data->type) {
	case BT_DATA_MANUFACTURER_DATA:
		*BUTTON_STATUS = data->data;
		return true;
	default:
		return false;
	}
}
#define NAME_LEN 30
/*Advertisment found*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	char addr_str[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	printk("Received advertisment from: %s",addr_str);

	//LED logic
	bool BUTTON_STATUS;
	bt_data_parse(ad, data_cb, &BUTTON_STATUS);
	if(BUTTON_STATUS){
		gpio_pin_set_dt(&led,(int)BUTTON_STATUS);
		LED_ON = true;
	}
	else{
		gpio_pin_set_dt(&led,(int)BUTTON_STATUS);
		LED_ON = false;
	}
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

	
	printk("Registered scan callbacks\n");

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
	if (err)
	{
		printk("Error with stopping observer (err %d)\n",err);
	}
	return err;
}