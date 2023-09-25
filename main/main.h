#include <stdio.h>
#include "components/sdi12.h"
#include "esp_log.h"

// SDI-12 ports 1,2,3,4 on Growlink PIC
#define SDI0 GPIO_NUM_5
#define SDI1 GPIO_NUM_18
#define SDI2 GPIO_NUM_19
#define SDI3 GPIO_NUM_21

// RGB LED pins
#define RGBR GPIO_NUM_4
#define RGBG GPIO_NUM_16
#define RGBB GPIO_NUM_17

// bus array
sdi12_bus_t *_sdi12Bus[4];

// functions
void initSdi12();
void app_main(void);