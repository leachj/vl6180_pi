#ifdef __cplusplus
extern "C"{
#endif 

typedef int vl6180;

#define VL6180_DEFAULT_ADDR 0x29

///Initialize a vl6180 sensor on the i2c bus configured at the specified address
/// \param device The I2C bus to open. e.g. "1" for using /dev/i2c-1
/// \param i2c_addr addres of the device. If you don't know, just call the "vl6180_initialise" variant of this function.
/// \return handle to the sensor. Keep this variable to talk to the sensor via the library
vl6180 vl6180_initialise_address(int device, int i2c_addr);

///Initialise a vl6180 sensor at it's default address
/// \param device The I2C bus to open. e.g. "1" for using /dev/i2c-1
/// \return handle to the sensor. Keep this variable to talk to the sensor via the library
vl6180 vl6180_initialise(int device);
  
///Change the address of the device. Needed if you have an address conflict (for example: using two of theses sensors on the same design). The handle will also be updated
/// \param handle The handle to the sensor given by vl6180_initialise
/// \param new_addr The new address to use on the i2c bus for this device
void vl6180_change_addr(vl6180 handle, int new_addr);

//TODO some of theses functions doesn't have the vl6180 prefix. udpate them to avoid name clashing
  
///Return the current distance as readed by the sensor
/// \param handle The handle to the sensor given by vl6180_initialise
/// \return distance in milimeter as an integer
int get_distance(vl6180 handle);

///Set the scalinb mode (read datasheet to seee about the max range vs. precision deal on this sensor)
/// \param handle The handle to the sensor given by vl6180_initialise
/// \param scaling Index of the scaling mode to use
void set_scaling(vl6180 handle, int scaling);

#define VL6180X_SYSTEM_INTERRUPT_CLEAR 0x0015

#define VL6180X_SYSALS_START 0x0038
#define VL6180X_SYSALS_ANALOGUE_GAIN 0x003F
#define VL6180X_SYSALS_INTEGRATION_PERIOD 0x0040

#define VL6180X_RESULT_ALS_VAL 0x0050
#define VL6180X_RESULT_RANGE_VAL 0x0062

#define VL6180X_I2C_SLAVE_DEVICE_ADDRESS 0x0212

typedef enum {
    GAIN_20 = 0, // Actual ALS Gain of 20
    GAIN_10,     // Actual ALS Gain of 10.32
    GAIN_5,      // Actual ALS Gain of 5.21
    GAIN_2_5,    // Actual ALS Gain of 2.60
    GAIN_1_67,   // Actual ALS Gain of 1.72
    GAIN_1_25,   // Actual ALS Gain of 1.28
    GAIN_1,      // Actual ALS Gain of 1.01
    GAIN_40,     // Actual ALS Gain of 40
} vl6180x_als_gain_t;

///Returns ambient light read by sensor in lx
/// \param handle the handle to the sensor given by vl6180_initialise
/// \param VL6180X_ALS_GAIN selected gain according to vl6180x_als_gain_t type. For factory calibrated sensor GAIN_1 is OK
/// \return ambient light read by sensor
float get_ambient_light(vl6180 handle, vl6180x_als_gain_t VL6180X_ALS_GAIN);

#ifdef __cplusplus
}
#endif
