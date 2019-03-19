#include <iostream>
#include <fstream>

#include "input_interface.h"

using namespace std;


int main()
{


    char *fname_alm_gps = 0;
    char *fname_alm_gln = 0;
    char *fname_alm_rinex_in = 0;
    char *fname_alm_rinex_out = 0;
    double dxyz[3] = {0. , 0., 0.};

    if (config_processing(fname_alm_gps, fname_alm_gln, fname_alm_rinex_in, fname_alm_rinex_out, dxyz))
        return 1;

	
	



    return 0;
}
