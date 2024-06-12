#include <zephyr.h>

#define FLAG_NETWORK_SEND 0x01
#define FLAG_NETWORK_START_SEND 0x03

static const uint16_t password = 53123;

#define SINK_NODE_NOT_KNOWN -1
#define NO_SINK_NODE 0
#define YES_SINK_NODE 1

int8_t SINK_NODE = YES_SINK_NODE;
bool ALREADY_PRESSED = false;
uint16_t own_node_id = 1;
struct packet{
	uint16_t password;
	uint8_t type;
	uint16_t nodeID;
	uint16_t recvNodeID;
	uint32_t counter;
	int16_t temp;
	uint8_t temp_dec;
	uint16_t humidity;
	uint8_t humidity_dec;
	uint32_t timestamp;
};

/*start packet*/
struct packet packet  = {
		.password = password,
		.type = FLAG_NETWORK_START_SEND,
		.nodeID = 1,
		.recvNodeID = 0,
		.counter = 0,
		.temp = 0,
		.temp_dec = 0,
		.humidity = 0,
		.humidity_dec = 0,
		.timestamp = 0,
	};

