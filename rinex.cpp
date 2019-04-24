#include "rinex.h"
#include <fstream>
#include <iostream>
#include <cstring>


using namespace rinex;
using namespace std;

const int rinex::MAX_TYPES_OF_OBS = 11;

rinex::TYPES_OF_OBSERV typeObs[rinex::MAX_TYPES_OF_OBS];



const int max_size_string = 120;                                                                        // вообще, 80 символов (без '\n'), + символ конца строки ('\0'). Но в данном случае можно дать волю суеверию

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
            // пересчёт координат
        }
        else if ( str_rinex_compare(str_cur, str_tobs) )
        {
            // копируем измерения в нужном порядке
        }
        else
        {
            fid_out << str_cur.str << endl;
            if ( str_rinex_compare(str_cur, str_eoh) )
                break;
        }

    }while(1);
}







