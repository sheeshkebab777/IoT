#include <zephyr.h>

#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>


#ifndef SHARED
#define SHARED
#include "Shared.h"
#endif
/*______________________________________________________________________*/
/*_______________________________Observer_______________________________*/
/*______________________________________________________________________*/
#define SINK_NODE_NOT_KNOWN -1
#define NO_SINK_NODE 0
#define YES_SINK_NODE 1

int8_t SINK_NODE = SINK_NODE_NOT_KNOWN;
bool sending = false;

struct k_work start_send_worker;
struct k_timer start_send_timer;

void wait_for_advertiser(){
	//wait for advertising to stop
	while(advertising){
		k_sleep(K_MSEC(1));
	}
}

/*Start advertising temp values*/
void start_sending_handler(struct k_work *work) {
    // Call stop_ble() here
	wait_for_advertiser();
    start_sending();
}

void callback_start_sending(struct k_timer *timer) {
    k_work_submit(&start_send_worker);
}





/*_____________________________*/

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
/*Advertisment found*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	/*dont process packet if currently advertising*/
			
	wait_for_advertiser();
	
	struct packet pack;
	bt_data_parse(ad,data_cb,&pack);

	if(pack.password != password) return;
	
	if (SINK_NODE == SINK_NODE_NOT_KNOWN) {
		SINK_NODE = NO_SINK_NODE;
	}

	if(SINK_NODE == YES_SINK_NODE){
		//measurements received
		if(pack.type == FLAG_NETWORK_SEND){
			//<nodeiID>;<measurement-counter>;<temp>;<humidity>;<timestamp>;<tx-time>
			printk("%d;%d;%.1f;%.1f;%d;%d\n",
			pack.nodeID,
			pack.counter,
			pack.temp,
			pack.humidity,
			pack.timestamp,
			((((sys_clock_cycle_get_32())*1000)/32768) - pack.timestamp));
			return;
		}
		//received ack for network formation
		else if(pack.type == FLAG_NETWORK_ACK_FORM){
			if(nodeCount == 3) return;

			for (size_t i = 0; i < 3; i++)
			{
				if(pack.nodeID == nodes[i]) return;
			}
			
			nodes[nodeCount] = pack.nodeID;
			nodeCount++;
			printk("New node added, ID:%d\n",pack.nodeID);
			if(nodeCount == 3){
				printk("Network formed...\n");
			}
		}
	}

	else if(SINK_NODE == NO_SINK_NODE){

		if(pack.type == FLAG_NETWORK_FORM){
			if(packet.recvNodeID == 0){
				packet.recvNodeID = pack.nodeID;
				printk("Node formed with root node ID: %d\n",packet.recvNodeID);
			}

			//return if not received from root node
			if(pack.recvNodeID != packet.nodeID) return;

			packet.type = FLAG_NETWORK_FORM;
			//advertise to neighbour the network formation
			printk("Forwarding formation\n");
			advertiser_restart();

			//wait for advertising to stop
			wait_for_advertiser();

			//ack the network formation
			packet.type = FLAG_NETWORK_ACK_FORM;
			advertiser_restart();
			
			return;
		}
		else if(pack.type == FLAG_NETWORK_START_SEND && packet.recvNodeID == pack.nodeID){
			
			packet.type = FLAG_NETWORK_START_SEND;
			packet.nodeCount = pack.nodeCount - 1;

			if(packet.nodeCount > 0){
				//advertise for longer
				uint16_t last_dur = ADV_DURATION;
				ADV_DURATION = 1000;

				advertiser_restart();
				//wait for advertising to stop
				wait_for_advertiser();

				ADV_DURATION = last_dur;
			}
			
			packet.type = FLAG_NETWORK_SEND;
			if (!sending){
				sending = true;
				printk("Start sending...\n");
				k_work_init(&start_send_worker, start_sending_handler);

				uint32_t rand = 500 + sys_rand32_get()%(500);
				k_timer_init(&start_send_timer, callback_start_sending, NULL);
				k_timer_start(&start_send_timer, K_MSEC(rand), K_MSEC(500));
			}
			
		}
		//forwarding neighbours measurments
		else if((pack.type == FLAG_NETWORK_SEND  ||
				 pack.type == FLAG_NETWORK_ACK_FORM) && pack.recvNodeID == packet.nodeID){

			struct packet own;
			memcpy((void*)(&own),(void*)(&packet),sizeof(struct packet));
			memcpy((void*)(&packet),(void*)(&pack),sizeof(struct packet));
			packet.recvNodeID = own.recvNodeID;

			advertiser_restart();
			if(pack.type == FLAG_NETWORK_ACK_FORM){
				printk("Forwarding ACK ID: %d...\n",pack.nodeID);
			}
			else{
				printk("Forwarding ID: %d...\n",pack.nodeID);
			}
			
			//wait for advertising to stop
			wait_for_advertiser();
			
			memcpy((void*)(&packet),(void*)(&own),sizeof(struct packet));
			return;
		}
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

