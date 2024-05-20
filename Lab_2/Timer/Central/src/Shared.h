#include <zephyr.h>

#define FLAG_NETWORK_SEND 0x01
#define FLAG_NETWORK_START_SEND 0x03

static const uint16_t password = 53123;

struct packet{
	uint16_t password;
	uint8_t type;
	uint16_t nodeID;
	uint16_t recvNodeID;
	uint32_t counter;
	int16_t temp;
	uint8_t humidity;
	uint32_t timestamp;
};

/*start packet*/
struct packet packet  = {
		.password = password,
		.type = FLAG_NETWORK_START_SEND,
		.nodeID = 3,
		.recvNodeID = 0,
		.counter = 0,
		.temp = 0,
		.humidity = 0,
		.timestamp = 0,
	};

