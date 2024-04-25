#include <zephyr.h>
#include <zephyr/types.h>

#include <sys/printk.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

#include <stddef.h>

bool BUTTON_PRESSED = false;
bool LED_ON = false;
bool SHUTTING_DOWN = false;
#define DURATION 3
/*________________________________________________________________________*/
/*_______________________________Advertiser______________________________*/
/*________________________________________________________________________*/
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

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
/*______________________________________________________________________*/
/*_______________________________Observer_______________________________*/
/*______________________________________________________________________*/
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
	return;
}
/*Scan receive found*/
static void scan_recv(const struct bt_le_scan_recv_info *info,
		      struct net_buf_simple *buf)
{
	bool BUTTON_STATUS;
	bt_data_parse(buf, data_cb, &BUTTON_STATUS);
	if(BUTTON_STATUS){
		LED_ON = true;
	}
	return;
}

static struct bt_le_scan_cb scan_callbacks = {
	.recv = scan_recv,
};

int observer_start()
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_PASSIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;

	bt_le_scan_cb_register(&scan_callbacks);
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
/*___________________________________________________________________*/
/*_______________________________Other_______________________________*/
/*___________________________________________________________________*/
void print_led_status(){
	if(LED_ON){
		printk("LED ON");
	}
	else{
		printk("LED OFF");
	}
}

/*At the beginning only scan for incoming adverts*/
static void bt_ready(int err)
{
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};

	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	err = observer_start();
	if(err){
		return;
	}

	printk("Observing as %s\n", addr_s);

}

bool BUTTON_ON = false;
bool final_adv_active = false;
struct k_timer advertising_timer;

int main(void)
{
	int err;

	printk("Starting ...\n");

	/* Initialize the Bluetooth Subsystem and only start scanning*/
	err = bt_enable(bt_ready);
	if (err) {
		return 0;
	}

	/*advertising, scanning loop*/
	
	while(1){
		/*First buttonpress*/
		if(!BUTTON_ON && BUTTON_PRESSED && !LED_ON){
			printk("BUTTON PRESSED");
			err = observer_stop();
			if (err) { return err;}
			
			err = advertiser_start();
			if(err) { return err; }
		}

		/*Button released*/
		if(BUTTON_ON && !BUTTON_PRESSED){
			printk("BUTTON RELEASED");
			BUTTON_ON = false;
			//update, button is not pressed anymore
			bt_le_adv_update_data(NULL, 0, sd, ARRAY_SIZE(sd));
			/*advertise the button not pressed information for DURATION seconds*/
			SHUTTING_DOWN = true;
			k_timer_init(&advertising_timer, handler, NULL);
			k_timer_start(&advertising_timer, K_SECONDS(DURATION), K_NO_WAIT);

			while(SHUTTING_DOWN){

			}

			/*Turn back to observer mode*/
			err = observer_start();
			if (err) { return err;}
			
			err = advertiser_stop();
			if(err) { return err; }
		}
		print_led_status();


	}
	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}