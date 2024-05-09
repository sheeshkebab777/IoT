#include <zephyr.h>

#define FLAG_NETWORK_FORM 0x00
#define FLAG_NETWORK_SEND 0x01

static const uint32_t password = 1610612736;

struct packet{
	uint32_t password;
	uint8_t type;
	uint32_t nodeID;
	uint32_t rootNodeID;
	uint32_t counter;
	float temp;
	float humidity;
	uint32_t timestamp;

};

/*start packet*/
struct packet packet  = {
		.password = password,
		.type = FLAG_NETWORK_FORM,
		.nodeID = 0,
		.rootNodeID = 0,
		.counter = 0,
		.temp = 0,
		.humidity = 0,
		.timestamp = 0
	};