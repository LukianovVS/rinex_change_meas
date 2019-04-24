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



    if (config_processing(fname_alm_gps, fname_alm_gln, fname_alm_rinex_in, fname_alm_rinex_out, dxyz))                     // ������ ������� ������ (��������� .inf �����)
        return 1;

    ifstream fid_in (fname_alm_rinex_in );                                                                                  // ���� ��������
    ofstream fid_out(fname_alm_rinex_out);                                                                                  // ���� ���������

	read_head_rinex(fid_out, fid_in, xyz0, dxyz);                                                                           // ������������ ����� rinex ����� (�� ������� � �����) � ���� ����������� ����������. ��������� ��. � header ����




    return 0;
}
