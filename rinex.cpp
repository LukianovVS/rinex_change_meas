
#include <fstream>
#include <iostream>
#include <cstring>
#include <cctype>


#include "rinex.h"
#include "string/charstring.h"
#include "satellite.h"

using namespace std;

RINEX_TYPE rinexType;


//----------------------------------------------------------------------------------------------------------------------// Шапка

const int max_size_string = 120;                                                                                        // вообще, 80 символов (без '\n'), + символ конца строки ('\0'). Но в данном случае можно дать волю суеверию

struct STR_RINEX
{
    char str[max_size_string];
    int len;
};

#define _END_OF_HEADER_ ("END OF HEADER")
#define _APPROX_POSITION_XYZ_ ("APPROX POSITION XYZ")
#define _TYPES_OF_OBSERV_ ("TYPES OF OBSERV")
#define _LEAP_SECONDS_ ("LEAP SECONDS")

bool str_rinex_compare(STR_RINEX str1, STR_RINEX str2)
{
  if (!strcmp(&str1.str[str1.len - str2.len], str2.str))
    return true;
  else
    return false;
}



void read_head_rinex(std::ofstream &fid_out, std::ifstream &fid_in, double xyz0[], double dxyz[])
{
  STR_RINEX str_cur           = {""                   , 0                             };
  const STR_RINEX str_eoh     = {_END_OF_HEADER_      , strlen(_END_OF_HEADER_)       };
  const STR_RINEX str_xyz     = {_APPROX_POSITION_XYZ_, strlen(_APPROX_POSITION_XYZ_) };
  const STR_RINEX str_tobs    = {_TYPES_OF_OBSERV_    , strlen(_TYPES_OF_OBSERV_)     };
  const STR_RINEX str_ls      = {_LEAP_SECONDS_       , strlen(_LEAP_SECONDS_)        };


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
        xyz0[k] = str2float(&str_cur.str[14*k], 14);
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
    else if ( str_rinex_compare(str_cur, str_ls) )
    {
      LS = str2int( str_cur.str, 12);
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


//----------------------------------------------------------------------------------------------------------------------// Основная часть файла

void read_body_rinex(std::ofstream &fid_out, std::ifstream &fid_in, double xyz0[], double dxyz[])
{

  int partOfBody = 0;
  char str[max_size_string];
  int Nsat = 0;
  SAT_ID sat_list[_MAX_SAT_];
  TIME time;

  fid_out.unsetf(ios::floatfield);
  fid_out.setf(ios::fixed);

  while (!fid_in.eof())
  {
    fid_in.getline(str, max_size_string);
    if (partOfBody == 0)                                                                                                // читаем заголовок. Переписываем его без изменений и узнаём в каком порядке идут спутники и их количество
    {
      fid_out << str << endl;

      if (str[30] != '0')
        continue;

      time.year   = 2000 + str2int(str +  1,  2);
      time.month  =        str2int(str +  4,  2);
      time.day    =        str2int(str +  7,  2);
      time.h      =        str2int(str + 10,  2);
      time.m      =        str2int(str + 13,  2);
      time.sec    =      str2float(str + 16, 10);

      Nsat = str2int(&str[30], 2);

      int n = 0;
      for (int k_sat = 0; k_sat < Nsat; k_sat++)
      {
        if (k_sat >= 12)
        {
          if (k_sat == 12)
          {
            fid_in.getline(str, max_size_string);
            fid_out << str << endl;
          }
          n = (k_sat - 12) * 3 + 32;
        }
        else
          n = k_sat * 3 + 32;

        sat_list[k_sat].sys = str[n];
        sat_list[k_sat].num = str2int(&str[n+1], 2);
      }

      partOfBody = 1;
    }
    else                                                                                                                // меняем измерения
    {
      int k_in_string = 0;
      double meas;
      char LLIandPower[3] = "01";

      for (int k_sat = 0; k_sat < Nsat; k_sat++)
      {
        double dR = calc_dr(sat_list[k_sat], time);

        for (int k_type = 0; k_type < rinexType.getNumUsedTypes(); k_type++)
        {
          if (k_in_string == 5)
          {
            fid_in.getline(str, max_size_string);
            fid_out << endl;
            k_in_string = 0;
          }

          int flag_empty = 0;
          meas            = str2float(&str[k_in_string * 16], 14, &flag_empty);
          LLIandPower[0]  = str[k_in_string * 16 + 14];
          LLIandPower[1]  = str[k_in_string * 16 + 15];

          k_in_string ++;

          switch (rinexType.getType(k_type))
          {
            case C1:
            case C2:
            case P1:
            case P2:
            {

              break;
            }
            case L1:
            {

              break;
            }
            case L2:
            {

              break;
            }

          }

          fid_out.width(14);
          fid_out.precision(3);
          if (flag_empty)
            fid_out << ' ';
          else
            fid_out << meas;
          fid_out.flush();
          fid_out.width(2);
          fid_out << LLIandPower;
          fid_out.flush();

        }

      }
      fid_out << '\n';
      partOfBody = 0;
    }


  }



}
