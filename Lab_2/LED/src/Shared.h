#include <zephyr.h>

#define FLAG_NETWORK_FORM 0x00
#define FLAG_NETWORK_SEND 0x01
#define FLAG_NETWORK_ACK_FORM 0x02
#define FLAG_NETWORK_START_SEND 0x03

static const uint16_t password = 63123;

struct packet{
	uint16_t password;
	uint8_t type;
	uint16_t nodeID;
	uint16_t recvNodeID;
	uint32_t counter;
	float temp;
	float humidity;
	uint32_t timestamp;
	uint8_t nodeCount;

};

/*start packet*/
struct packet packet  = {
		.password = password,
		.type = FLAG_NETWORK_FORM,
		.nodeID = 0,
		.recvNodeID = 0,
		.counter = 0,
		.temp = 0,
		.humidity = 0,
		.timestamp = 0,
		.nodeCount = 3
	};

//since we only have 4 nodes including sink node
static uint16_t nodes[3];
static uint8_t nodeCount = 0; 