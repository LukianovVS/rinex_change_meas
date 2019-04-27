#include <iostream>
#include <fstream>

#include "input_interface.h"
#include "satellite.h"
#include "rinex.h"

using namespace std;


int main()
{
  //--------------------------------------------------------------------------------------------------------------------// конфигурация
  /*
  Имена файлов
  */
  char fname_alm_gps[SIZE_FNAME];                                                                                       // альманах GPS
  char fname_alm_gln[SIZE_FNAME];                                                                                       // альманах ГЛОНАСС
  char fname_alm_rinex_in[SIZE_FNAME];                                                                                  // rinex с исходными измерениями
  char fname_alm_rinex_out[SIZE_FNAME];                                                                                 // выходной файл

  double dxyz[3];                                                                                                       // смещение, которое нужно сделать (увод координат)
	double xyz0[3];                                                                                                       // координаты потребителя. Пока беруться из входного rinex файла



  if (config_processing(fname_alm_gps, fname_alm_gln, fname_alm_rinex_in, fname_alm_rinex_out, dxyz))                   // читаем входные данные (обработка .inf файла)
      return 1;

  read_almanac(fname_alm_gps, fname_alm_gln);                                                                           // чтение параметров орбит спутников из альманаха

  ifstream fid_in (fname_alm_rinex_in );                                                                                // файл входнной
  ofstream fid_out(fname_alm_rinex_out);                                                                                // файл выходнной

	read_head_rinex(fid_out, fid_in, xyz0, dxyz);                                                                         // Переписываем шапку rinex файла (из старого в новый) и берём необходимую информацию. Подробнее см. в header файл



  return 0;
}
