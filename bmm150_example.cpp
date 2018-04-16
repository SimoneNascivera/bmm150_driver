#include "bmm150_arm.h"
#include <iostream>

using namespace std;

BMM150 bmm = BMM150();

int main(){
    int8_t rslt = BMM150_OK;
	bmm150_dev sensore;

	sensore = bmm.initialize(rslt);
    cout << (rslt!=BMM150_OK) << endl;

	bmm150_dev* p_sensore = &sensore;
	bmm.set_sensor_settings(p_sensore, 0, rslt);
    cout << (rslt!=BMM150_OK) << endl;
	
	for(int i = 0; i< 500; i++)
    {
		bmm.read_sensor_data(p_sensore, rslt);
        cout << (rslt!=BMM150_OK) << endl;
		usleep(100000);
	}
	return 0;
}
