#ifdef __cplusplus
extern "C"{
#endif 

typedef int vl6180;

#define VL1680_DEFALUT_ADDR 0x29

int vl6180_initialise(int device, int i2cAddr);
int get_distance(vl6180 handle);
void set_scaling(vl6180 handle, int scaling);

//hack:add access to lower_level functions

int read_byte(vl6180 handle, int reg);
void write_byte(vl6180 handle, int reg, char data);
void write_two_bytes(vl6180 handle, int reg, int data);


#ifdef __cplusplus
}
#endif
