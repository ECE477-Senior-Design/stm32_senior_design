#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc.h"


#ifndef USB_H
#define USB_H

int load_map(void);
int check_usb_connection(void);


#endif
