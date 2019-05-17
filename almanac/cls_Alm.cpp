#include "cls_Alm.h"
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>


const double _2_PI_ = 2 * M_PI;

using namespace std;
using namespace ALM_CONST;


// glonass ...
inline double pow_1d3 (double x)
{
  return pow(x, 1.0 / 3.0);
}

inline double pow_2 (double x)
{
  return pow(x, 2);
}

inline double pow_2d3 (double x)
{
  return pow(x, 2.0 / 3.0);
}

inline double pow_3d2 (double x)
{
  return pow(x, 3.0 / 2.0);
}

inline double pow_3 (double x)
{
  return pow(x, 3);
}
//

const GPS_CONST ALM_CONST::gpsConst = {3.986005e14, 7.2921151467e-5};
const GLN_CONST ALM_CONST::glnConst = {.i0 = 63.0 * M_PI / 180.0, .C_20 = 1082.62575e-6, .a_earth = 6378.136e3, .w_earth = 0.7292115e-4, .nu_earth = 398600.4418e9, .T_mean = 43200};

void ALM_GPS::calcPosition (const int week_calc, const double tow_calc)
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

	if (this->Health)
  {
    cout << "warning: The Sat " << Sat << " is Health" << endl;
  }
	if (!satFound)
	{
		this->Health = 1;
		cout << "warning: The Sat " << Sat << " was not found" << endl;
	}

}




void utc2gln(int &N4, int &N0, double &ti, const int day, const int month, const int year, const double sec, int dh)
{
  const int DayLen = 86400;

  int J = ( (year - 1996) % 4 );
	int TabBeginYear[] = { 0, 366, 731, 1096 };
	int C = TabBeginYear[J];

	int feb = (!J) ? 29 : 28;		                                                                                // проверка на високостный год

	int mond[] = { 31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	N0 = day + C;
	for (int k = 0; k < month - 1; k++) { N0 += mond[k]; }

  ti = sec;
	N4 = (year - 1996 - J) / 4 + 1;

	ti += 3600 * dh;	      			                                                                                //mdv = utc + 3 hr

	if (ti >= DayLen) {
		ti = ti - DayLen;
		N0 = N0 + 1;
	}
	if (N0 > 1461) {
		N0 = 1;
		N4 = N4 + 1;
	}
}


void ALM_GLN::calcPosition (const int N4, const int N0, const double ti)
{
  //                                  этот кусок надо обдумать
  double tmp = (N4 != 27) ? 1461.0 : 1460.0;
	int dNa = N0 - this->Na - (int) round((N0 - this->Na) / tmp) * tmp;

	double dt = dNa * 86400 + (ti - this->ta);
	int W = (dt / (glnConst.T_mean + this->dT0));
	double Tdr = glnConst.T_mean + this->dT0 + (2 * W + 1) * this->dT1;
	double n = 2.0 * M_PI / Tdr;
  //

	double a = pow_1d3( pow_2(Tdr / _2_PI_) * glnConst.nu_earth );
	double a_old;
	do
  {
    a_old = a;
    double p1 = a * (1 - pow_2(this->E));
    tmp = 1.5 * glnConst.C_20 * pow_2( glnConst.a_earth / p1 ) *
            ( (2. - 2.5 * pow_2(sin(this->i)) ) * pow_3d2(1. - pow_2(this->E)) / pow_2(1. + this->E * cos(this->w)) +
              pow_3(1. + this->E * cos(this->w)) / (1. - pow_2(this->E)) );
    double T_osk = Tdr / (1. - tmp);
    a = pow_1d3( pow_2(T_osk / _2_PI_) * glnConst.nu_earth);
  } while ( abs(a - a_old) > 0.01 );

  double p = a * (1. - pow_2(this->E));

  tmp = 1.5 * glnConst.C_20*n*pow(glnConst.a_earth / p, 2)*cos(i);
	double lk = this->Lam - (glnConst.w_earth + tmp)*dt;
	double wk = this->w - 3. / 4. * glnConst.C_20*n*pow(glnConst.a_earth/p, 2)*(1 - 5 * pow(cos(i), 2))*dt;

	double E0 = -2. * atan(sqrt((1 - this->E) / (1 + this->E))*tan(wk / 2));
	double la1 = wk + E0 - this->E * sin(E0);
	double la2 = la1 + n * (dt - (glnConst.T_mean + this->dT0) * W - this->dT1 * pow(W, 2) );

	double h = this->E * sin(wk);
	double l = this->E * cos(wk);
	double B = 1.5 * glnConst.C_20 * pow(glnConst.a_earth / a, 2);

	double da1 = (2. * B*(1. - 1.5*pow(sin(i),2))*(l*cos(la1) + h * sin(la1)) +
		B * pow(sin(i), 2)*(0.5*h*sin(la1) -
		0.5*l*cos(la1) + cos(2. * la1) +
		(7. / 2.)*l*cos(3. * la1) + (7. / 2.)*h*sin(3. * la1)))*a;
	double dh1 = ( B * (1. - (3. / 2.)* pow(sin(i), 2))*
		(sin(la1) + (3. / 2.)*l*sin(2. * la1) - (3. / 2.)*h*cos(2. * la1)) -
		(1. / 4.)*B*pow(sin(i),2)*(sin(la1) - (7. / 3.)*sin(3. * la1) + 5. * l*sin(2. * la1) -
		(17. / 2.)*l*sin(4. * la1) + (17. / 2.)*h*cos(4. * la1) + h * cos(2. * la1)) +
		B * pow(cos(i), 2)*(-(1. / 2.))*l*sin(2. * la1));
	double dl1 = (B*(1. - (3. / 2.)*pow(sin(i), 2))*
		(cos(la1) + (3. / 2.)*l*cos(2. * la1) + (3. / 2.)*h*sin(2. * la1)) -
		(1. / 4.)*B*pow(sin(i), 2)*((-cos(la1)) - (7. / 3.)*cos(3. * la1) -
			5. * h*sin(2. * la1) - (17. / 2.)*l*cos(4. * la1) - (17. / 2.)*h*sin(4. * la1) +
			l * cos(2. * la1)) + (1. / 2.) * B*pow(cos(i), 2)*h*sin(2. * la1));
	double dlk1 = -B * cos(i)*((7. / 2.)*l*sin(la1) -
		(5. / 2.)*h*cos(la1) - (1. / 2.)*sin(2. * la1) -
		(7. / 6.)*l*sin(3. * la1) + (7. / 6.)*h*cos(3. * la1));
	double di1 = (1. / 2.) * B*cos(i)*sin(i)*
		((-l)*cos(la1) + h * sin(la1) + cos(2. * la1) +
		(7. / 3.)*l*cos(3. * la1) + (7. / 3.)*h*sin(3. * la1));
	double dla1 = 2. * B*(1. - (3. / 2.)*pow(sin(i), 2))*((7. / 4.)*l*sin(la1) -
		(7. / 4.)*h*cos(la1)) + 3. * B*pow(sin(i), 2)*((-7. / 24.)*h*cos(la1) -
		(7. / 24.)*l*sin(la1) - (49. / 72.)*h*cos(3. * la1) + (49. / 72.)*l*sin(3. * la1) +
			(1. / 4.)*sin(2. * la1)) + B * pow(cos(i), 2)*((7. / 2.)*l*sin(la1) -
			(5. / 2.)*h*cos(la1) - (1. / 2.)*sin(2. * la1) - (7. / 6.)*l*sin(3. * la1) +
				(7. / 6.)*h*cos(3. * la1));
	double da2 = (2. * B*(1. - (3. / 2.)*pow(sin(i), 2))*
		(l*cos(la2) + h * sin(la2)) + B * pow(sin(i), 2)*((1. / 2.)*h*sin(la2) -
		(1. / 2.)*l*cos(la2) + cos(2. * la2) + (7. / 2.)*l*cos(3. * la2) + (7. / 2.)*h*sin(3. * la2)))*a;
	double dh2 = (B*(1. - (3. / 2.)*pow(sin(i), 2))*(sin(la2) + (3. / 2.)*l*sin(2. * la2) -
		(3. / 2.)*h*cos(2. * la2)) - (1. / 4.)*B*pow(sin(i),2)*(sin(la2) -
		(7. / 3.)*sin(3. * la2) + 5. * l*sin(2. * la2) - (17. / 2.)*l*sin(4. * la2) +
			(17. / 2.)*h*cos(4. * la2) + h * cos(2. * la2)) + B * pow(cos(i), 2)*(-(1. / 2.))*l*sin(2. * la2));
	double dl2 = (B*(1. - (3. / 2.)*pow(sin(i), 2))*(cos(la2) + (3. / 2.)*l*cos(2. * la2) +
		(3. / 2.)*h*sin(2. * la2)) - (1. / 4.)*B*pow(sin(i), 2)*((-cos(la2)) - (7. / 3.)*cos(3. * la2) -
			5. * h*sin(2. * la2) - (17. / 2.)*l*cos(4. * la2) - (17. / 2.)*h*sin(4. * la2) + l * cos(2. * la2))
		+ (1. / 2.) * B*pow(cos(i), 2)*h*sin(2. * la2));
	double dlk2 = -B * cos(i)*((7. / 2.)*l*sin(la2) - (5. / 2.)*h*cos(la2) - (1. / 2.)*sin(2. * la2)
		- (7. / 6.)*l*sin(3. * la2) + (7. / 6.)*h*cos(3. * la2));
	double di2 = 0.5 * B*cos(i)*sin(i)*((-l)*cos(la2) + h * sin(la2) + cos(2. * la2) +
		(7. / 3.)*l*cos(3. * la2) + (7. / 3.)*h*sin(3. * la2));
	double dla2 = 2. * B*(1. - (3. / 2.)*pow(sin(i), 2))*((7. / 4.)*l*sin(la2) -
		(7. / 4.)*h*cos(la2)) + 3. * B*pow(sin(i), 2)*((-7. / 24.)*h*cos(la2) -
		(7. / 24.)*l*sin(la2) - (49. / 72.)*h*cos(3. * la2) + (49. / 72.)*l*sin(3. * la2) +
			(1. / 4.)*sin(2. * la2)) + B * pow(cos(i), 2)*((7. / 2.)*l*sin(la2) -
			(5. / 2.)*h*cos(la2) - (1. / 2.)*sin(2. * la2) - (7. / 6.)*l*sin(3. * la2) +
				(7. / 6.)*h*cos(3. * la2));

	a += da2 - da1;
	h += dh2 - dh1;
	l += dl2 - dl1;
	la2 += dla2 - dla1;
	i += di2 - di1;
	lk += dlk2 - dlk1;
	wk = atan2(h, l);
	this->E = sqrt( h*h + l*l );

	double E = la2 - wk;

	double E1;
	for (int k = 0; k < 5; k++) {
		E1 = la2 - wk + this->E * sin(E);
		if (abs(E1 - E) < 1e-9) {
			break;
		}
		E = E1;
	}
	double Ei = E1;

	double vk = 2. * atan(sqrt((1 + this->E) / (1. - this->E))*tan(Ei / 2.));
	double uk = vk + wk;
	p = a * (1. - this->E*this->E);
	double rk = p / (1. + this->E * cos(vk));
	double Vr = (sqrt(glnConst.nu_earth / p))*(this->E*sin(vk));
	double Vu = (sqrt(glnConst.nu_earth / p))*(1. + this->E * cos(vk));

	double X0i = rk * (cos(uk)*cos(lk) - sin(uk)*sin(lk)*cos(i));
	double Y0i = rk * (cos(uk)*sin(lk) + sin(uk)*cos(lk)*cos(i));
	double Z0i = rk * sin(uk)*sin(i);

	double Vx0i = (Vr*(cos(uk)*cos(lk) - sin(uk)*sin(lk)*cos(i)) - Vu * (sin(uk)*cos(lk) + cos(uk)*sin(lk)*cos(i))) + glnConst.w_earth * Y0i;
	double Vy0i = Vr * (cos(uk)*sin(lk) + sin(uk)*cos(lk)*cos(i)) - Vu * (sin(uk)*sin(lk) - cos(uk)*cos(lk)*cos(i)) - glnConst.w_earth * X0i;
	double Vz0i = Vr * sin(uk)*sin(i) + Vu * cos(uk)*sin(i);

  this->xyz[0] = X0i;
  this->xyz[1] = Y0i;
  this->xyz[2] = Z0i;

  return;
}

void ALM_GLN::read_alm(char *fname, int Sat)
{
  ifstream fid;
	bool satFound = 0;
	int cur_n = 0;

	fid.open(fname);
	this->N_sat = Sat;
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
						fid >> this->ch_freq;
						fid >> this->Health;
						fid >> this->day;
						fid >> this->month;
						fid >> this->year;
						fid >> this->sec;
						fid >> this->dt_gln2utc;
						fid >> this->dt_gps2gln;
						fid >> this->dt_sat;

						this->Health = 1 - this->Health;                                                                            // делаем признак здоровья как у GPS
            utc2gln(this->Na4, this->Na, this->ta, this->day, this->month, this->year, this->sec, 0);
					}
					break;
				}
				case 3:
				{
					if (!satFound)
						fid.ignore(521, '\n');
					else
					{
						fid >> this->Lam;               // долгота узла
						fid >> this->dI;                // коррекция наклонения орбиты
						fid >> this->w;                 // аргумент перигея
						fid >> this->E;                 // эксцентриситет
						fid >> this->dT0;               // поправка к драконическому периоду
						fid >> this->dT1;               // поправка к драконическому периоду

            this->Lam *= M_PI;
            this->i   = glnConst.i0 + this->dI * M_PI;
            this->w   *= M_PI;
					}
					break;
				}
			}
		}
		if(satFound)
			break;
	}

	fid.close();

  if (this->Health)
  {
    cout << "warning: The Sat " << Sat << " is Health" << endl;
  }
	if (!satFound)
	{
		this->Health = 1;
		cout << "warning: The Sat " << Sat << " was not found" << endl;
	}
}
