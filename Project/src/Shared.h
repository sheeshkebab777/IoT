#include <zephyr/drivers/gpio.h>


#define TURN_OFF_T 0x00
//80C
#define GREEN_TEA_T 0x01
//100C
#define BLACK_TEA_T  0x02

#define POWER_SUPPLY_PIN_NUMBER 27
#define PORT "GPIO_0"


const struct device * power_supply;


static const uint16_t password = 53123;


struct k_work advertiser_start_work;
struct k_work run_cooker_work;
bool sender = true;
bool already_set = false;
struct packet{
	uint16_t password;
	uint8_t type;
};

/*start packet*/
struct packet packet  = {
		.password = password,
		.type = TURN_OFF_T
	};

static const struct device *temp_dev;
