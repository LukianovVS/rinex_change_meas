#include <iostream>
#include <fstream>

#include "input_interface.h"
#include "cls_Alm.h"
#include "rinex.h"

using namespace std;


int main()
{
    //----------------------------------------------------------------------------------------------------------------------// конфигурация
    /*
    Имена файлов
    */
    char fname_alm_gps[SIZE_FNAME];                                                                                         // альманах GPS
    char fname_alm_gln[SIZE_FNAME];                                                                                         // альманах ГЛОНАСС
    char fname_alm_rinex_in[SIZE_FNAME];                                                                                    // rinex с исходными измерениями
    char fname_alm_rinex_out[SIZE_FNAME];                                                                                   // выходной файл
    double dxyz[3] = {0. , 0., 0.};                                                                                         // смещение, которое нужно сделать (увод координат)
	double xyz0[3];                                                                                                         // координаты потребителя. Пока беруться из входного rinex файла

	rinex::TYPES_OF_OBSERV typeObs[rinex::N_TYPES_OF_OBS];                                                                  // здесь пречислены все типы измерений в том порядке, в котором они лежат в файле



    if (config_processing(fname_alm_gps, fname_alm_gln, fname_alm_rinex_in, fname_alm_rinex_out, dxyz))                     // читаем входные данные (обработка .inf файла)
        return 1;

	int *gps_list;
	int *gln_list;
	gps_list = new int [32];
	gln_list = new int [24];

	read_head_rinex(fname_alm_rinex_in, fname_alm_rinex_out, dxyz, gps_list, gln_list, typeObs, xyz0);                      // Переписываем шапку rinex файла (из старого в новый) и берём необходимую информацию. Подробнее см. в header файл

	delete []gps_list;
	delete []gln_list;


    return 0;
}
