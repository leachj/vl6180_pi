typedef int vl6180;

#ifdef __cplusplus
extern "C"{
#endif 

int vl6180_initialise(int device);
int get_distance(vl6180 handle);

#ifdef __cplusplus
}
#endif
