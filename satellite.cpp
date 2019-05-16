
#include "satellite.h"
#include <fstream>
#include <cmath>
#include "srns_const.h"


ALM_GPS almGps[_MAX_GPS_SAT_];
ALM_GLN almGln[_MAX_GLN_SAT_];

int LS = 0;

using namespace std;

void read_almanac (char fname_alm_gps[], char fname_alm_gln[])
{
  for (int k = 0; k < _MAX_GPS_SAT_; k++)
  {
    almGps[k].read_alm(fname_alm_gps, k + 1);
  }

  for (int k = 0; k < _MAX_GLN_SAT_; k++)
  {
    almGln[k].read_alm(fname_alm_gln, k + 1);
  }

  return;
}




void utc2gps(TIME_GPS *tgps, TIME tutc, const int LS)
{
  const int dayBeginYear[] = {0, 366, 731, 1096};
  int J = ((tutc.year - 1980) % 4);
  int C = dayBeginYear[J];

  int feb = (J == 0)? 29 : 28;
  int mond[12] = {31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int Dayin4 = C + (tutc.day - 1);                                                                              // прошло дней с начала последнего 4-хлетия
  for (int k = 0; k < (tutc.month - 1); k++)
    Dayin4 += mond[k];

  double ToD = tutc.h * 3600 + tutc.m * 60 + tutc.sec;

  int N4 = (tutc.year - 1980 - J) / 4;
  int Days = N4 * 1461 - 5 + Dayin4;

  tgps->week = Days / 7;
  tgps->tow  = (Days % 7) * 86400 + ToD - LS * 0;

  if (tgps->tow > 604800)
  {
    tgps->tow -= 604800;
    tgps->week ++;
  }
  else if (tgps->tow < 0)
  {
    tgps->tow += 604800;
    tgps->week --;
  }
}




void calc_dr(double &dR, double &dL1, double &dL2, const SAT_ID sat, TIME_GPS t_gps, TIME_GLN t_gln, const double xyz_receiver[], const double dxyz[])
{

  const double propagationTime = 73.0e-3;
  double xyz_sat[3];
  double R_rs[3];
  double norm_R_rs;

  double RtoL1, RtoL2;


  if (sat.sys == 'G')
  {
    ALM_GPS *alm = &almGps[sat.num - 1];
    t_gps.tow -= propagationTime;
    if (t_gps.tow < 0)
    {
      t_gps.tow += 604800;
      t_gps.week--;
    }
    alm->calcPosition(t_gps.week, t_gps.tow);
    alm->get_x(xyz_sat);
    RtoL1 =_GPS_RANGE_TO_CYCLES_L1_;
    RtoL2 =_GPS_RANGE_TO_CYCLES_L2_;
  }
  else if (sat.sys == 'R')
  {
    ALM_GLN *alm = &almGln[sat.num - 1];
    t_gln.sec -= propagationTime;
    if (t_gln.sec < 0)
    {
      t_gln.sec += 24 * 3600;
      t_gln.N0--;

      if (t_gln.N0 == 0)
      {
        t_gln.N0 = 366 + 365 * 3;
        t_gln.N4--;
      }
    }
    alm->calcPosition(t_gln.N4, t_gln.N0, t_gln.sec);
    alm->get_x(xyz_sat);

    int ch_freq = alm->get_ch_freq();

    RtoL1 = (_GLN_FREQ0_L1_ + ch_freq * _GLN_DFREQ_L1_) / _C_;
    RtoL2 = (_GLN_FREQ0_L2_ + ch_freq * _GLN_DFREQ_L2_) / _C_;

  }



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

  dR = - tmp / norm_R_rs;
  dL1 = dR * RtoL1;
  dL2 = dR * RtoL2;

}
