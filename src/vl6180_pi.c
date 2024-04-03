#include "vl6180_pi.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>



int read_byte(vl6180 handle, int reg){
    char data_write[2];
    char data_read[1];
    data_write[0] = (reg >> 8) & 0xFF; // MSB of register address
    data_write[1] = reg & 0xFF; // LSB of register address

    write(handle, data_write, 2);
    read(handle, data_read, 1);
    return data_read[0];
}

int read_two_bytes(vl6180 handle, int reg){
    char data_write[2];
    char data_read[2];
    data_write[0] = (reg >> 8) & 0xFF; // MSB of register address
    data_write[1] = reg & 0xFF; // LSB of register address

    write(handle, data_write, 2);
    read(handle, data_read, 2);     // 16-bit value
    return (data_read[0] << 8) | data_read[1];
}

void write_byte(vl6180 handle, int reg,char data) {
    char data_write[3];
    data_write[0] = (reg >> 8) & 0xFF;; // MSB of register address
    data_write[1] = reg & 0xFF; // LSB of register address
    data_write[2] = data & 0xFF;
    write(handle, data_write, 3);
}

void write_two_bytes(vl6180 handle, int reg,int data) {
    char data_write[4];
    data_write[0] = (reg >> 8) & 0xFF;; // MSB of register address
    data_write[1] = reg & 0xFF; // LSB of register address
    data_write[2] = (data >> 8) & 0xFF;; // MSB of data
    data_write[3] = data & 0xFF; // LSB of data
    write(handle, data_write, 4);
}

void start_range(vl6180 handle) {
    write_byte(handle, 0x018,0x01);
}

void poll_range(vl6180 handle) {
    char status;
    char range_status;

    // check the status
    status = read_byte(handle,0x04f);
    range_status = status & 0x07;

    // wait for new measurement ready status
    while (range_status != 0x04) {
        status = read_byte(handle,0x04f);
        range_status = status & 0x07;
    }
}

void clear_interrupts(vl6180 handle) {
    write_byte(handle,0x015,0x07);
}

void set_scaling(vl6180 handle, int new_scaling){

    int scalerValues[] = {0, 253, 127, 84};
    int defaultCrosstalkValidHeight = 20;
    if (new_scaling < 1 || new_scaling > 3) { return; }

    int ptp_offset = read_byte(handle,0x24);

    write_two_bytes(handle,0x96,scalerValues[new_scaling]);
    write_byte(handle,0x24,ptp_offset / new_scaling);
    write_byte(handle,0x21, defaultCrosstalkValidHeight / new_scaling);
    int rce = read_byte(handle,0x2d);
    write_byte(handle,0x2d, (rce & 0xFE) | (new_scaling == 1));
}

vl6180 vl6180_initialise(int device)
{
	return vl6180_initialise_address(device, VL6180_DEFAULT_ADDR);
}

vl6180 vl6180_initialise_address(int device, int addr){
    vl6180 handle=-1;
    char buf[15];
    snprintf(buf,15,"/dev/i2c-%d",device);
    if ((handle = open(buf, O_RDWR)) < 0) {
        return -2;
    }

    if (ioctl(handle, I2C_SLAVE, addr) < 0) {
        return -3;
    }

    int setup = read_byte(handle,0x016);

    if(setup == 1){
        write_byte(handle,0x0207, 0x01);
        write_byte(handle,0x0208, 0x01);
        write_byte(handle,0x0096, 0x00);
        write_byte(handle,0x0097, 0xfd);
        write_byte(handle,0x00e3, 0x00);
        write_byte(handle,0x00e4, 0x04);
        write_byte(handle,0x00e5, 0x02);
        write_byte(handle,0x00e6, 0x01);
        write_byte(handle,0x00e7, 0x03);
        write_byte(handle,0x00f5, 0x02);
        write_byte(handle,0x00d9, 0x05);
        write_byte(handle,0x00db, 0xce);
        write_byte(handle,0x00dc, 0x03);
        write_byte(handle,0x00dd, 0xf8);
        write_byte(handle,0x009f, 0x00);
        write_byte(handle,0x00a3, 0x3c);
        write_byte(handle,0x00b7, 0x00);
        write_byte(handle,0x00bb, 0x3c);
        write_byte(handle,0x00b2, 0x09);
        write_byte(handle,0x00ca, 0x09);
        write_byte(handle,0x0198, 0x01);
        write_byte(handle,0x01b0, 0x17);
        write_byte(handle,0x01ad, 0x00);
        write_byte(handle,0x00ff, 0x05);
        write_byte(handle,0x0100, 0x05);
        write_byte(handle,0x0199, 0x05);
        write_byte(handle,0x01a6, 0x1b);
        write_byte(handle,0x01ac, 0x3e);
        write_byte(handle,0x01a7, 0x1f);
        write_byte(handle,0x0030, 0x00);

        // Recommended : Public registers - See data sheet for more detail

        write_byte(handle,0x0011, 0x10); // Enables polling for ‘New Sample ready’ when measurement completes
        write_byte(handle,0x010a, 0x30); // Set the averaging sample period (compromise between lower noise and increased execution time)
        write_byte(handle,0x003f, 0x46); // Sets the light and dark gain (upper nibble). Dark gain should not be changed.
        write_byte(handle,0x0031, 0xFF); // sets the # of range measurements after which auto calibration of system is performed
        write_byte(handle,0x0040, 0x63); // Set ALS integration time to 100ms
        write_byte(handle,0x002e, 0x01); // perform a single temperature calibratio of the ranging sensor
        write_byte(handle,0x001b, 0x09); // Set default ranging inter-measurement period to 100ms
        write_byte(handle,0x003e, 0x31); // Set default ALS inter-measurement period to 500ms
        write_byte(handle,0x0014, 0x24); // Configures interrupt on ‘New Sample Ready threshold event’ 

        write_byte(handle,0x016, 0x00);
    }

    set_scaling(handle,1);

    return handle;
}

void vl6180_change_addr(vl6180 handle, int newAddr)
{
    write_byte(handle, VL6180X_I2C_SLAVE_DEVICE_ADDRESS, newAddr);
    ioctl(handle, I2C_SLAVE, newAddr);
}

int get_distance(vl6180 handle){

    int range;
    start_range(handle);
    poll_range(handle);

    range=read_byte(handle, VL6180X_RESULT_RANGE_VAL);
    clear_interrupts(handle);
    return range;
}

float get_ambient_light(vl6180 handle, vl6180x_als_gain_t vl6180x_als_gain) {
    // Ported from C++: https://github.com/sparkfun/SparkFun_ToF_Range_Finder-VL6180_Arduino_Library
    // First load in Gain we are using, do it everytime incase someone changes it on us.
    // Note: Upper nibble should be set to 0x4 i.e. for ALS gain of 1.0 write 0x46
    write_byte(handle, VL6180X_SYSALS_ANALOGUE_GAIN, (0x40 | vl6180x_als_gain));    // Set the ALS gain

    // Start ALS Measurement
    write_byte(handle, VL6180X_SYSALS_START, 0x01);
    write_byte(handle, VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);

    // Retrieve the Raw ALS value from the sensor
    unsigned int alsRaw = read_two_bytes(handle, VL6180X_RESULT_ALS_VAL);

    // Get Integration Period for calculation, we do this everytime incase someone changes it on us.
    unsigned int alsIntegrationPeriodRaw = read_two_bytes(handle, VL6180X_SYSALS_INTEGRATION_PERIOD);

    float alsIntegrationPeriod = 100.0 / alsIntegrationPeriodRaw;

    // Calculate actual LUX from Appnotes

    float alsGain = 0.0;

    switch (vl6180x_als_gain)
    {
    case GAIN_20:
        alsGain = 20.0;
        break;
    case GAIN_10:
        alsGain = 10.32;
        break;
    case GAIN_5:
        alsGain = 5.21;
        break;
    case GAIN_2_5:
        alsGain = 2.60;
        break;
    case GAIN_1_67:
        alsGain = 1.72;
        break;
    case GAIN_1_25:
        alsGain = 1.28;
        break;
    case GAIN_1:
        alsGain = 1.01;
        break;
    case GAIN_40:
        alsGain = 40.0;
        break;
    }

    // Calculate LUX from formula in AppNotes

    float alsCalculated = (float)0.32 * ((float)alsRaw / alsGain) * alsIntegrationPeriod;

    return alsCalculated;
}