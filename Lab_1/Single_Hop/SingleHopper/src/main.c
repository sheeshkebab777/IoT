#include "Advertiser.h"
#include "Observer.h"
/*___________________________________________________________________*/
/*_______________________________Other_______________________________*/
/*___________________________________________________________________*/
void print_led_status(){
	if(LED_ON){
		printk("LED ON as\n");
	}
	else{
		printk("LED OFF as\n");
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

bool BUTTON_ON = false; //value, that checks if button was already pressed
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
			/*advertise the button not pressed information for DURATION seconds, before shutting down*/
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
		//fix: dont print that often
		print_led_status();


	}
	
	printk("Exiting %s thread.\n", __func__);
	return 0;
}