#include "bmm150_arm.h"

int file;
int adapter_nr = 2;
char filename[40];

BMM150::BMM150(){
    sprintf(filename,"/dev/i2c-1");
    file = open(filename, O_RDWR);//, O_NONBLOCK,O_NDELAY);
    if (file < 0)
    {
        printf("Failed to open the bus.");
        /* ERROR HANDLING; you can check error */
        exit(1);
    }

    /* The I2C address */

    if (ioctl(file, I2C_SLAVE, BMM150_DEFAULT_I2C_ADDRESS) < 0)
    {
        printf("Failed to acquire bus access and/or talk to slave.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    else{
        printf("BMM150 found at 0x%02X\n",  BMM150_DEFAULT_I2C_ADDRESS);
    }
}

int8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len){
    uint8_t buffer_write[2];
    memset(buffer_write,'\0',2);
	int n_writ;
    n_writ = 0;
    // Request data
    buffer_write[0] = reg_addr;

    n_writ = write(file,buffer_write,1);
    if (n_writ != 1)
    {
        /* ERROR HANDLING: i2c transaction failed */
      	printf("BMM150 Reading Error (cannot request data): Failed to write");
		return -1;
   	}
	int n_read;
   	// Read data
   	n_read = read(file,data,len);
    if (n_read != len)
    {
        /* ERROR HANDLING: i2c transaction failed */
       	printf("BMM150 Reading Error (not enough data readed) :Failed to read");
		return -1;
    }
	return BMM150_OK;
}

int8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len){
	uint8_t buffer_write[len+1]; // cast needed???!?
    memset(buffer_write,'\0',len+1);
    int n_writ;
    buffer_write[0] = reg_addr;
	for(int i = 0; i<len; i++){
	        buffer_write[i+1] = data[i];
	}
    n_writ = write(file,buffer_write, len+1);
    if( n_writ < len+1)
    {
    	/* ERROR HANDLING: i2c transaction failed */
        printf("BMM150 Writing Error :Failed to write");
        return -1;
    }
    return BMM150_OK;
}

void delay_ms(uint32_t period){
	usleep(period*1000);
}

bmm150_dev BMM150::initialize(int8_t &rslt){
	struct bmm150_dev dev;
	
	dev.dev_id = BMM150_DEFAULT_I2C_ADDRESS;
	dev.intf = BMM150_I2C_INTF;
	dev.read = i2c_read;
	dev.write = i2c_write;
	dev.delay_ms = delay_ms;
	
	rslt = bmm150_init(&dev);
	
	return dev;
}

void BMM150::set_sensor_settings(struct bmm150_dev *dev, int mode, int8_t &rslt)
{
	if(mode == 0){
		/* Setting the power mode as normal */
		dev->settings.pwr_mode = BMM150_NORMAL_MODE;
		rslt = bmm150_set_op_mode(dev);
	}
	else{
		/* Setting the preset mode as Low power mode 
		i.e. data rate = 10Hz XY-rep = 1 Z-rep = 2*/
		dev->settings.preset_mode = BMM150_PRESETMODE_LOWPOWER;
		rslt = bmm150_set_presetmode(dev);
	}
}

void BMM150::read_sensor_data(struct bmm150_dev *dev, int8_t &rslt)
{

	/* Mag data for X,Y,Z axis are stored inside the
	bmm150_dev structure in int16_t format */
	rslt = bmm150_read_mag_data(dev);

	/* Print the Mag data */
	printf("\n Magnetometer data \n");
	printf("MAG X : %d \t MAG Y : %d \t MAG Z : %d \n"
		,dev->data.x, dev->data.y, dev->data.z);
}
