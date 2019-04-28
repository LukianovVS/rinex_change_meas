
#include "satellite.h"
#include <fstream>

ALM_GPS almGps[_MAX_GPS_SAT_];
int LS = 0;

using namespace std;

void read_almanac (char fname_alm_gps[], char fname_alm_gln[])
{
  for (int k = 0; k < _MAX_GPS_SAT_; k++)
  {
    almGps[k].read_alm(fname_alm_gps, k + 1);
  }

  return;
}

double calc_dr(SAT_ID sat, TIME time)
{
  double dr = 0;



  return dr;
}
