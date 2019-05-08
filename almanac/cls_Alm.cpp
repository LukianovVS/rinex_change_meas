#include "cls_Alm.h"
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;
using namespace ALM_CONST;




const GPS_CONST ALM_CONST::gpsConst = {3.986005e14, 7.2921151467e-5};


void ALM_GPS::calcPosition (int week_calc, double tow_calc)
{
  double tk = (week_calc - this->week) * _SEC_WEEK_ + (tow_calc - this->t_oe);
  double Mk = this->M0 + this->n0 * tk;
  double Ek = Mk;
  double Ek_old;
  do
  {
    Ek_old = Ek;
    Ek = Mk + this->E * sin(Ek);
  } while ( abs(Ek - Ek_old) > 1e-7 );

  double tmp_y = sqrt(1 - pow(this->E, 2)) * sin(Ek);
  double tmp_x = cos(Ek) - this->E;
  double vk = atan2( tmp_y, tmp_x );

  double uk = vk + this->w;
  double rk = this->A * (1 - this->E * cos(Ek));
  double ik = this->i;

  double xk1 = rk * cos(uk);
  double yk1 = rk * sin(uk);

  double Wk = this->Om0 + (this->Om1 /*/ M_PI*/ - gpsConst.OmE) * tk - gpsConst.OmE * this->t_oe;                           // !!! "/ MPI" скорее всего  это ошибка

  this->xyz[0] = xk1 * cos(Wk) - yk1 * cos(ik) * sin(Wk);
  this->xyz[1] = xk1 * sin(Wk) + yk1 * cos(ik) * cos(Wk);
  this->xyz[2] = yk1 * sin(ik);



}

void ALM_GPS::read_alm(char *fname, int Sat)
{
	ifstream fid;
	bool satFound = 0;
	int cur_n = 0;

	fid.open(fname);
	this->prn = Sat;
	while (!fid.eof())
	{
		for (int k_str = 1; k_str <= 3; k_str++)
		{
			switch(k_str)
			{
				case 1:
				{
					fid.ignore(521, '\n');
					break;
				}
				case 2:
				{
					fid >> cur_n;
					if (cur_n != Sat)
						fid.ignore(521, '\n');
					else
					{
						satFound = 1;
						fid >> this->Health;
						fid >> this->week;
						fid >> this->t_oe;
						fid >> this->day;
						fid >> this->month;
						fid >> this->year;
						fid >> this->t_al;
						fid >> this->af0;
						fid >> this->af1;
						fid >> this->Om1;

						this->Om1 *= M_PI;
					}
					break;
				}
				case 3:
				{
					if (!satFound)
						fid.ignore(521, '\n');
					else
					{
						fid >> this->Om0;
						fid >> this->i;
						fid >> this->w;
						fid >> this->E;
						fid >> this->SQRT_A;
						fid >> this->M0;

            this->Om0 *= M_PI;
            this->i   *= M_PI;
            this->w   *= M_PI;
            this->M0  *= M_PI;
						this->A  = this->SQRT_A * this->SQRT_A;
						this->n0 = sqrt( gpsConst.M / pow(this->A, 3) );
					}
					break;
				}
			}
		}
		if(satFound)
			break;
	}

	fid.close();

	if (!satFound)
	{
		this->Health = 1;
		cout << "warning: The Sat " << Sat << " was not found" << endl;
	}

}


void ALM_GPS::dbg()
{
	cout << this->prn << " " << this->Health << " " << this->week << " " << this->t_oe << endl;
	cout << this->Om0 << " " << this->i << endl;
}
