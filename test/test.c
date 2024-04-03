#include "vl6180_pi.h"
#include <stdio.h>

int main(){

	vl6180 handle = vl6180_initialise(1);

	if(handle<=0){
		return 1;
	}
	
	int distance = get_distance(handle);

	printf("distance at scaling 1 is %d\n", distance);

	set_scaling(handle,2);
	
	distance = get_distance(handle);

	printf("distance at scaling 2 is %d\n", distance);
	
	set_scaling(handle,3);
	
	distance = get_distance(handle);

	printf("distance at scaling 3 is %d\n", distance);

	// for factory calibration
	float ambient_light = get_ambient_light(handle, GAIN_1);

	printf("ambient_light at gain 1 is %f\n", ambient_light);

	return 0;
}
