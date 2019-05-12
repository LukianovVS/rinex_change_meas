#include "rinex.h"
#include <iostream>


const char fname_config[] = "config.inf";

using namespace std;


void read_inf(char fname[], ifstream &fid)
{
    char ch;
    int fpos = 0;
    while(1)
    {
        fid.get(ch);
        if(ch == '\n' || ch == ' ' || ch == '\t' || ch == '#')
            break;
        fname[fpos++] = ch;
    }

    fname[fpos] = '\0';

    if (ch != '\n')
        fid.ignore(521, '\n');
}


inline void error_fopen( const char *fname)
{
	cout << "Error: Can't open a file \"" << fname <<"\"" << endl;
}





int config_processing(char fname_alm_gps[], char fname_alm_gln[], char fname_rinex_in[], char fname_rinex_out[], double dx[])
{
  ifstream fid;
	ifstream fid_test;

  try
  {
    fid.open(fname_config);
    if(!fid.is_open())
        throw 1;
    //													// alm gps
    read_inf(fname_alm_gps, fid);

    fid_test.open(fname_alm_gps);
    if( fid_test.is_open() )
      fid_test.close();
    else
      throw 2;

    //													// alm gln
    read_inf(fname_alm_gln, fid);

		fid_test.open(fname_alm_gln);
		if( fid_test.is_open() )
      fid_test.close();
		else
			throw 3;

		//													// rinex input
    read_inf(fname_rinex_in, fid);

		fid_test.open(fname_rinex_in);
		if( fid_test.is_open() )
			fid_test.close();
		else
			throw 4;


		//													// rinex output
    read_inf(fname_rinex_out, fid);

    for (int k = 0; k < 3; k++)                         // dx, dy, dz
      fid >> dx[k];

  }
  catch (int n_err)
  {
      switch (n_err)
      {
        case 1:
        {
          error_fopen(fname_config);
          break;
        }
        case 2:
        {
          error_fopen(fname_alm_gps);
          break;
        }
        case 3:
        {
          error_fopen(fname_alm_gln);
          break;
        }
        case 4:
        {
          error_fopen(fname_rinex_in);
          break;
        }
        case 5:
        {
          error_fopen(fname_rinex_out);
          break;
        }
        default:
        {
          cout << "Error: Unknown error. Check " << fname_config << " ." << endl;
        }
      }

    fid.close();
    return n_err;
  }
  fid.close();

  cout << "GPS alm  : " << fname_alm_gps << endl;
  cout << "GLN alm  : " << fname_alm_gln << endl;
  cout << "rinex in : " << fname_rinex_in << endl;
  cout << "rinex out: " << fname_rinex_out << endl;

  cout << "dX, dY, dZ = ";
  for (int k = 0; k < 3; k++)
      cout << dx[k] << " ";
  cout << endl;

  return 0;
}



