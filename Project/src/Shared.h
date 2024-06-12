#include <zephyr.h>

#define TURN_OFF_T 0x00
//80C
#define GREEN_TEA_T 0x01
//100C
#define BLACK_TEA_T  0x02

static const uint16_t password = 53123;
static const struct device *dev;
bool sender = false;

struct packet{
	uint16_t password;
	uint8_t type;
};

/*start packet*/
struct packet packet  = {
		.password = password,
		.type = TURN_OFF_T
	};

