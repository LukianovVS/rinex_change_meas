#include <iostream>
#include <fstream>

#include "input_interface.h"
#include "cls_Alm.h"
#include "rinex.h"

using namespace std;


int main()
{
    //----------------------------------------------------------------------------------------------------------------------// ������������
    /*
    ����� ������
    */
    char fname_alm_gps[SIZE_FNAME];                                                                                         // �������� GPS
    char fname_alm_gln[SIZE_FNAME];                                                                                         // �������� �������
    char fname_alm_rinex_in[SIZE_FNAME];                                                                                    // rinex � ��������� �����������
    char fname_alm_rinex_out[SIZE_FNAME];                                                                                   // �������� ����
    double dxyz[3] = {0. , 0., 0.};                                                                                         // ��������, ������� ����� ������� (���� ���������)
	double xyz0[3];                                                                                                         // ���������� �����������. ���� �������� �� �������� rinex �����

	rinex::TYPES_OF_OBSERV typeObs[rinex::N_TYPES_OF_OBS];                                                                  // ����� ���������� ��� ���� ��������� � ��� �������, � ������� ��� ����� � �����



    if (config_processing(fname_alm_gps, fname_alm_gln, fname_alm_rinex_in, fname_alm_rinex_out, dxyz))                     // ������ ������� ������ (��������� .inf �����)
        return 1;

	int *gps_list;
	int *gln_list;
	gps_list = new int [32];
	gln_list = new int [24];

	read_head_rinex(fname_alm_rinex_in, fname_alm_rinex_out, dxyz, gps_list, gln_list, typeObs, xyz0);                      // ������������ ����� rinex ����� (�� ������� � �����) � ���� ����������� ����������. ��������� ��. � header ����

	delete []gps_list;
	delete []gln_list;


    return 0;
}
