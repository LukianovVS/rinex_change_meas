#include "rinex.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>

using namespace std;

RINEX_TYPE rinexType;



const int max_size_string = 120;                                                                                        // вообще, 80 символов (без '\n'), + символ конца строки ('\0'). Но в данном случае можно дать волю суеверию

struct STR_RINEX
{
    char str[max_size_string];
    int len;
};

#define _END_OF_HEADER_ ("END OF HEADER")
#define _APPROX_POSITION_XYZ_ ("APPROX POSITION XYZ")
#define _TYPES_OF_OBSERV_ ("TYPES OF OBSERV")

bool str_rinex_compare(STR_RINEX str1, STR_RINEX str2)
{
  if (!strcmp(&str1.str[str1.len - str2.len], str2.str))
    return true;
  else
    return false;
}

/*
  * Компилятор MinGW осложняет путь решения. Поэтому была написана функция для чтения float чисел
*/
double getFloat(char str[], int N = 14)
{
    double x = 0;
    int j = 0;
    int fp = 0;
    for (int k = 0; k < N; k++)
    {
      if (fp)
        j++;

      if (str[k] == ' ')
        continue;
      else if( str[k] == '.')
        fp = 1;
      else
        x = x*10 + (str[k] - '0');
    }
    return x / pow(10, j);
}



void read_head_rinex(std::ofstream &fid_out, std::ifstream &fid_in, double xyz0[], double dxyz[])
{
  STR_RINEX str_cur           = {""                   , 0                             };
  const STR_RINEX str_eoh     = {_END_OF_HEADER_      , strlen(_END_OF_HEADER_)       };
  const STR_RINEX str_xyz     = {_APPROX_POSITION_XYZ_, strlen(_APPROX_POSITION_XYZ_) };
  const STR_RINEX str_tobs    = {_TYPES_OF_OBSERV_    , strlen(_TYPES_OF_OBSERV_)     };


  do
  {
    fid_in.getline(str_cur.str, max_size_string);

    str_cur.len = strlen(str_cur.str);

    if ( str_rinex_compare(str_cur, str_xyz) )
    {
      double xyz_new[3];

      fid_out.unsetf(ios::floatfield);
      for (int k = 0; k < 3; k++)
      {
        xyz0[k] = getFloat(&str_cur.str[14*k], 14);
        xyz_new[k] = xyz0[k] + dxyz[k];
        fid_out.setf(ios::fixed);
        fid_out.precision(4);
        fid_out.width(14);
        fid_out <<  xyz_new[k];
      }

      fid_out << "                  APPROX POSITION XYZ" << endl;


    }
    else if ( str_rinex_compare(str_cur, str_tobs) )
    {
      rinexType.getListOfTypes(str_cur.str);
      fid_out << str_cur.str << endl;
    }
    else
    {
      fid_out << str_cur.str << endl;
      if ( str_rinex_compare(str_cur, str_eoh) )
        break;
    }

  }while(1);
}


RINEX_TYPE::RINEX_TYPE()
{
  for (int k = 0; k < EndOfList; k++)
	  this->list[k] = EndOfList;
  this->NumUsedType = 0;
}

void RINEX_TYPE::getListOfTypes(char str[])
{
  int N = strlen(str);
  char ch[3] = "X1";
  for (int k = 0; k < N; k++)
  {
    if ( isalpha(str[k]) )
	{
	  ch[0] = str[k  ];
	  ch[1] = str[++k];

	  if ( !strcmp(ch, "C1") )
	    this->list[this->NumUsedType] = C1;
	  else if ( !strcmp(ch, "C2") )
	    this->list[this->NumUsedType] = C2;
	  else if ( !strcmp(ch, "L1") )
	    this->list[this->NumUsedType] = L1;
	  else if ( !strcmp(ch, "L2") )
	    this->list[this->NumUsedType] = L2;
	  else if ( !strcmp(ch, "S1") )
	    this->list[this->NumUsedType] = S1;
	  else if ( !strcmp(ch, "S2") )
	    this->list[this->NumUsedType] = S2;
	  else if ( !strcmp(ch, "D1") )
	    this->list[this->NumUsedType] = D1;
	  else if ( !strcmp(ch, "D2") )
	    this->list[this->NumUsedType] = D2;
	  else if ( !strcmp(ch, "P1") )
	    this->list[this->NumUsedType] = P1;
	  else if ( !strcmp(ch, "P2") )
	    this->list[this->NumUsedType] = P2;
	  else
	  {
	    this->list[this->NumUsedType] = NoObs;
		cout << "error: unknow type " << ch << endl;
	  }
	  this->NumUsedType++;
	}
	else if (str[k] == '#')
		break;
  }
}



