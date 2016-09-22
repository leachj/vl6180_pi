#ifdef __cplusplus
extern "C"{
#endif 

typedef int vl6180;

int vl6180_initialise(int device);
int get_distance(vl6180 handle);

#ifdef __cplusplus
}
#endif
