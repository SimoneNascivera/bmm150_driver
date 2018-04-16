#ifndef _BMM150_ARM_H_
#define _BMM150_ARM_H_

#include <stdint.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "bmm150.h"
#include "bmm150_defs.h"

void delay_ms(uint32_t period);
int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

class BMM150{
	public:
		BMM150();
		bmm150_dev initialize(int8_t &rslt);
		void set_sensor_settings(struct bmm150_dev *dev, int mode, int8_t &rslt);
		void read_sensor_data(struct bmm150_dev *dev, int8_t &rslt);
};

#endif

