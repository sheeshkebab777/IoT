/** @file
 *  @brief HoG Service sample
 */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <zephyr/types.h>
#include <drivers/gpio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <sys/printk.h>
#include <sys/byteorder.h>
#include <kernel.h>
#include <zephyr/types.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>


#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

enum {
	HIDS_REMOTE_WAKE = BIT(0),
	HIDS_NORMALLY_CONNECTABLE = BIT(1),
};

struct hids_info {
	uint16_t version; /* version number of base USB HID Specification */
	uint8_t code; /* country HID Device hardware is localized for. */
	uint8_t flags;
} __packed;

struct hids_report {
	uint8_t id; /* report id */
	uint8_t type; /* report type */
} __packed;

static struct hids_info info = {
	.version = 0x0000,
	.code = 0x00,
	.flags = HIDS_NORMALLY_CONNECTABLE,
};

enum {
	HIDS_INPUT = 0x01,
	HIDS_OUTPUT = 0x02,
	HIDS_FEATURE = 0x03,
};

static struct hids_report input = {
	.id = 0x01,
	.type = HIDS_INPUT,
};

static uint8_t simulate_input;
static uint8_t ctrl_point;
static uint8_t report_map[] = {
	0x05, 0x01, /* Usage Page (Generic Desktop Ctrls) */
	0x09, 0x02, /* Usage (Mouse) */
	0xA1, 0x01, /* Collection (Application) */
	0x85, 0x01, /*	 Report Id (1) */
	0x09, 0x01, /*   Usage (Pointer) */
	0xA1, 0x00, /*   Collection (Physical) */
	0x05, 0x09, /*     Usage Page (Button) */
	0x19, 0x01, /*     Usage Minimum (0x01) */
	0x29, 0x03, /*     Usage Maximum (0x03) */
	0x15, 0x00, /*     Logical Minimum (0) */
	0x25, 0x01, /*     Logical Maximum (1) */
	0x95, 0x03, /*     Report Count (3) */
	0x75, 0x01, /*     Report Size (1) */
	0x81, 0x02, /*     Input (Data,Var,Abs,No Wrap,Linear,...) */
	0x95, 0x01, /*     Report Count (1) */
	0x75, 0x05, /*     Report Size (5) */
	0x81, 0x03, /*     Input (Const,Var,Abs,No Wrap,Linear,...) */
	0x05, 0x01, /*     Usage Page (Generic Desktop Ctrls) */
	0x09, 0x30, /*     Usage (X) */
	0x09, 0x31, /*     Usage (Y) */
	0x15, 0x81, /*     Logical Minimum (129) */
	0x25, 0x7F, /*     Logical Maximum (127) */
	0x75, 0x08, /*     Report Size (8) */
	0x95, 0x02, /*     Report Count (2) */
	0x81, 0x06, /*     Input (Data,Var,Rel,No Wrap,Linear,...) */
	0xC0,       /*   End Collection */
	0xC0,       /* End Collection */
};


static ssize_t read_info(struct bt_conn *conn,
			  const struct bt_gatt_attr *attr, void *buf,
			  uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data,
				 sizeof(struct hids_info));
}

static ssize_t read_report_map(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, report_map,
				 sizeof(report_map));
}

static ssize_t read_report(struct bt_conn *conn,
			   const struct bt_gatt_attr *attr, void *buf,
			   uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data,
				 sizeof(struct hids_report));
}

static void input_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	simulate_input = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

static ssize_t read_input_report(struct bt_conn *conn,
				 const struct bt_gatt_attr *attr, void *buf,
				 uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, NULL, 0);
}

static ssize_t write_ctrl_point(struct bt_conn *conn,
				const struct bt_gatt_attr *attr,
				const void *buf, uint16_t len, uint16_t offset,
				uint8_t flags)
{
	uint8_t *value = attr->user_data;

	if (offset + len > sizeof(ctrl_point)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);

	return len;
}

#if CONFIG_SAMPLE_BT_USE_AUTHENTICATION
/* Require encryption using authenticated link-key. */
#define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ_AUTHEN
#define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE_AUTHEN
#else
/* Require encryption. */
#define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ_ENCRYPT
#define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE_ENCRYPT
#endif

/* HID Service Declaration */
BT_GATT_SERVICE_DEFINE(hog_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_HIDS),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_INFO, BT_GATT_CHRC_READ,
			       BT_GATT_PERM_READ, read_info, NULL, &info),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_REPORT_MAP, BT_GATT_CHRC_READ,
			       BT_GATT_PERM_READ, read_report_map, NULL, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_REPORT,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       SAMPLE_BT_PERM_READ,
			       read_input_report, NULL, NULL),
	BT_GATT_CCC(input_ccc_changed,
		    SAMPLE_BT_PERM_READ | SAMPLE_BT_PERM_WRITE),
	BT_GATT_DESCRIPTOR(BT_UUID_HIDS_REPORT_REF, BT_GATT_PERM_READ,
			   read_report, NULL, &input),
	BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_CTRL_POINT,
			       BT_GATT_CHRC_WRITE_WITHOUT_RESP,
			       BT_GATT_PERM_WRITE,
			       NULL, write_ctrl_point, &ctrl_point),
);



volatile bool changed = false;

struct k_thread X_thread;
K_THREAD_STACK_DEFINE(X_thread_stack, STACK_SIZE);
struct k_thread Y_thread;
K_THREAD_STACK_DEFINE(Y_thread_stack, STACK_SIZE);
struct k_thread left_thread;
K_THREAD_STACK_DEFINE(left_thread_stack, STACK_SIZE);
struct k_thread right_thread;
K_THREAD_STACK_DEFINE(right_thread_stack, STACK_SIZE);

#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_X = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});

#define SW1_NODE	DT_ALIAS(sw1)
#if !DT_NODE_HAS_STATUS(SW1_NODE, okay)
#error "Unsupported board: sw1 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_Y = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,
							      {0});

#define SW2_NODE	DT_ALIAS(sw2)
#if !DT_NODE_HAS_STATUS(SW2_NODE, okay)
#error "Unsupported board: sw2 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_left = GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios,
							      {0});							  

#define SW3_NODE	DT_ALIAS(sw3)
#if !DT_NODE_HAS_STATUS(SW3_NODE, okay)
#error "Unsupported board: sw3 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button_right = GPIO_DT_SPEC_GET_OR(SW3_NODE, gpios,
							      {0});

uint32_t get_ms(){
	return (((sys_clock_cycle_get_32())*1000)/32768);
}

static int8_t report[3] = {0, 0, 0};
uint8_t presses_x = 0;
uint8_t presses_y = 0;
void button_manage_X(void * arg1,void *  arg2,void * arg3)
{
	while (true){
		if(!gpio_pin_get_dt(&button_X)){
			presses_x = 0;
		}
		if(gpio_pin_get_dt(&button_X) && presses_x == 0){
			presses_x = 1;
			uint32_t start_ms = get_ms();
			bool pressed = true;
			while(get_ms() - start_ms < 500){
				if (!gpio_pin_get_dt(&button_X)){
					pressed = false;
				}
				if (!pressed && gpio_pin_get_dt(&button_X)){
					presses_x = 2;
					break;
				}

			}
		}

		switch (presses_x)
		{
		case 0:
			report[1] = 0;
			break;
		
		case 1:
			report[1] = 5;
			break;
		
		case 2:
			report[1] = -5;
			break;
		}

		bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));
		memset(report,0,3*sizeof(int8_t));
		k_sleep(K_MSEC(25));
	}
	

}

void button_manage_Y(void * arg1,void *  arg2,void * arg3)
{	
		while (true){
		if(!gpio_pin_get_dt(&button_Y)){
			presses_y = 0;
		}
		if(gpio_pin_get_dt(&button_Y) && presses_y == 0){
			presses_y = 1;
			uint32_t start_ms = get_ms();
			bool pressed = true;
			while(get_ms() - start_ms < 500){
				if (!gpio_pin_get_dt(&button_Y)){
					pressed = false;
				}
				if (!pressed && gpio_pin_get_dt(&button_Y)){
					presses_y = 2;
					break;
				}

			}
		}
		

		switch (presses_y)
		{
		case 0:
			report[2] = 0;
			break;
		
		case 1:
			report[2] = -5;
			break;
		
		case 2:
			report[2] = 5;
			break;
		}

		bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));
		memset(report,0,3*sizeof(int8_t));
		k_sleep(K_MSEC(10));
	}
}

void left_button_manage(void * arg1, void * arg2, void * arg3){
	while (true){

		if(gpio_pin_get_dt(&button_left)){
			report[0] |= 1;

			bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));

			uint32_t start_ms = get_ms();
			bool pressed = true;
			while(get_ms() - start_ms < 500){
				if (!gpio_pin_get_dt(&button_left)){
					pressed = false;
				}
				if (!pressed && gpio_pin_get_dt(&button_left)){
					bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));
					break;
				}

			}
		}
		memset(report,0,3*sizeof(int8_t));
		bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));
		
		k_sleep(K_MSEC(25));
	}
}

void right_button_manage(void * arg1, void * arg2, void * arg3){
	while (true){

		if(gpio_pin_get_dt(&button_right)){
			report[0] |= 2;

			bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));

			uint32_t start_ms = get_ms();
			bool pressed = true;
			while(get_ms() - start_ms < 500){
				if (!gpio_pin_get_dt(&button_right)){
					pressed = false;
				}
				if (!pressed && gpio_pin_get_dt(&button_right)){
					bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));
					break;
				}

			}
		}
		memset(report,0,3*sizeof(int8_t));
		bt_gatt_notify(NULL, &hog_svc.attrs[5],
						report, sizeof(report));
		k_sleep(K_MSEC(25));
	}
}


void hog_init(void)
{
	
	gpio_pin_configure_dt(&button_X, GPIO_INPUT);
	printk("Set up button at %s pin %d\n", button_X.port->name, button_X.pin);

	gpio_pin_configure_dt(&button_Y, GPIO_INPUT);
	printk("Set up button at %s pin %d\n", button_Y.port->name, button_Y.pin);

	gpio_pin_configure_dt(&button_left, GPIO_INPUT);
	printk("Set up button at %s pin %d\n", button_left.port->name, button_left.pin);

	gpio_pin_configure_dt(&button_right, GPIO_INPUT);
	printk("Set up button at %s pin %d\n", button_right.port->name, button_right.pin);
	
	k_thread_create(&X_thread, X_thread_stack,
					K_THREAD_STACK_SIZEOF(X_thread_stack),
					button_manage_X, NULL, NULL, NULL,
					THREAD_PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&Y_thread, Y_thread_stack,
					K_THREAD_STACK_SIZEOF(Y_thread_stack),
					button_manage_Y, NULL, NULL, NULL,
					THREAD_PRIORITY, 0, K_NO_WAIT);
	
	k_thread_create(&left_thread, left_thread_stack,
					K_THREAD_STACK_SIZEOF(left_thread_stack),
					left_button_manage, NULL, NULL, NULL,
					THREAD_PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&right_thread, right_thread_stack,
					K_THREAD_STACK_SIZEOF(right_thread_stack),
					right_button_manage, NULL, NULL, NULL,
					THREAD_PRIORITY, 0, K_NO_WAIT);
}

void hog_button_loop(void)
{

	for (;;) {
	/* HID Report:
		* Byte 0: buttons (lower 3 bits)
		* Byte 1: X axis (int8)
		* Byte 2: Y axis (int8)
		*/
	k_sleep(K_MSEC(100));
	}
}
