#include "cls_Alm.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace ALM_CONST;

const GPS_CONST gpsConst = {3.986005e14, 7.2921151467e-5};


void ALM_GPS::calc_XV()
{
	this->E = this->E + 1.;
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
