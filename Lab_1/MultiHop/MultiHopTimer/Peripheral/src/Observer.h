#include <zephyr.h>

#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
/*______________________________________________________________________*/
/*_______________________________Observer_______________________________*/
/*______________________________________________________________________*/
struct bt_le_scan_param scan_param = {
	.type       = BT_LE_SCAN_TYPE_PASSIVE,
	.options    = BT_LE_SCAN_OPT_NONE,
	.interval   = BT_GAP_SCAN_FAST_INTERVAL,
	.window     = BT_GAP_SCAN_FAST_WINDOW,
};

static bool data_cb(struct bt_data *data, void *user_data)
{
	int *counter = user_data;

	switch (data->type) {
	case BT_DATA_MANUFACTURER_DATA:
		memcpy(counter, data->data, sizeof(int));
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
	//dont accept packets, when advertising 
	if(advertising){return;}
	
	int c;
	bt_data_parse(ad, data_cb, &c);
	
	//c < counter -> probably packet from peripheral node
	if(c > counter){
		counter = c;
		uint32_t time_ms = (((sys_clock_cycle_get_32())*1000)/32768);
		printk("%d: Received Counter: %d\n",time_ms,c);
		//start advertising this value
		advertiser_restart();
	}
	
	
	return;
}

int observer_start()
{

	int err;

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Start observing failed (err %d)\n", err);
		return err;
	}
	printk("Started observing...\n");

	return 0;
}
