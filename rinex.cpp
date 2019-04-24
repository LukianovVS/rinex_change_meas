#include "rinex.h"
#include <fstream>
#include <iostream>
#include <cstring>


using namespace rinex;
using namespace std;

bool strfind(const char str1[], const char str2[]);

void read_head_rinex(char fname_alm_rinex_in[],
						char fname_alm_rinex_out[],
						double dxyz[], int gps_list[], int gln_list[],
								rinex::TYPES_OF_OBSERV typeObs[], double xyz0[])
{
	ifstream fid_in;
	ofstream fid_out;
	char str[123];

	fid_in.open(fname_alm_rinex_in);
	fid_out.open(fname_alm_rinex_out);
	int n = 0;
	do
	{
		fid_in.getline(str, 123);
		fid_out << str << endl;
	}while ( !strfind(str, "END OF HEADER") && (++n < 19) );
	fid_in.close();
	fid_out.close();
}




bool strfind(const char str1[], const char str2[])
{
	int N1 = strlen(str1);
	int N2 = strlen(str2);

	if (N2 > N1) return 0;
	int dN = N1 - N2 + 1;

	char *str_copy = new char[N2 + 1];
	str_copy[N2 + 1] = '\0';

	cout << "N = " << N1 << " " << N2 << endl;

	for( int k = 0; k < dN; k++)
	{
		strncpy(str_copy, &str1[k], sizeof(char) * N2);
		str_copy[N2 + 1] = '\0';
		cout << &str1[k] << " vs " << str2 << endl;
		cout << str_copy << endl;
		if (!strcmp(&str1[k], str2))
			return 1;
	}
	delete [] str_copy;
	return 0;
}


