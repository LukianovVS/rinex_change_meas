
#include "satellite.h"
#include <fstream>
#include <cmath>

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


void utc2gps(TIME_GPS *tgps, TIME tutc, const int LS)
{
  const int dayBeginYear[] = {0, 366, 731, 1096};
  int J = ((tutc.year - 1980) % 4) + 1;
  int C = dayBeginYear[J];

  int feb = (J == 1)? 29 : 28;
  int mond[12] = {31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int Dayin4 = tutc.day + C - 1;                                                                              // прошло дней с начала последнего 4-хлетия
  for (int k = 0; k < (tutc.month - 1); k++)
    Dayin4 += mond[k];

  double ToD = tutc.h * 3600 + tutc.m * 60 + tutc.sec;

  int N4 = (tutc.year - 1980 - (J - 1)) / 4;
  int Days = N4 * 1461 - 5 + Dayin4

  tgps->week = Days / 7;
  tgps->tow  = (Days % 7) * 86400 + ToD - LS;

  if (tgps->tow > 604800)
  {
    tgps->TOW -= 604800;
    tgps->week ++;
  }
  else if (tgps->tow < 0)
  {
    tgps->TOW += 604800;
    tgps->week --;
  }

}


double calc_dr(SAT_ID sat, TIME_GPS time, double xyz_receiver[], double dxyz[])
{
  const double propagationTime = 73.0e-3;
  double xyz_sat[3];
  double R_rs[3];
  double norm_R_rs;
  double dr = 0;

  ALM_GPS *alm = &almGps[sat.num - 1];


  time.tow -= propagationTime;

  if (time.tow < 0)
  {
    time.tow += 604800;
    time.week--;
  }

  alm->calc_XV();
  alm->get_x(xyz_sat);


  norm_R_rs = 0;
  for (int k = 0; k < 3; k++)
  {
    R_rs[k] = xyz_sat[k] - xyz_receiver[k];
    norm_R_rs += R_rs[k] * R_rs[k];
  }
  norm_R_rs = sqrt(norm_R_rs);

  double tmp = 0;

  for (int k = 0; k < 3; k++)
    tmp += dxyz[k] * R_rs[k];

  dr = - tmp / norm_R_rs;

  return dr;
}
