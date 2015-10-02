#include "vl6180_pi.h"
#include <stdio.h>

int main(){

	vl6180 handle = vl6180_initialise(1);

	if(handle<=0){
		return 1;
	}
	
	int distance = get_distance(handle);

	printf("distance is %d\n", distance);

	return 0;
}
