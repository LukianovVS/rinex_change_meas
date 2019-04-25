#ifndef RENEX_H
#define RENEX_H

#include <fstream>


extern const int MAX_TYPES_OF_OBS;
enum LIST_TYPES_OF_OBSERV {C1 = 0, C2, L1, L2, S1, S2, D1, D2, P1, P2, NoObs, EndOfList};


class RINEX_TYPE
{
private:
  LIST_TYPES_OF_OBSERV list[LIST_TYPES_OF_OBSERV::EndOfList]; 
  NumUsedType;
public:
  RINEX_TYPE();
  
  void getListOfTypes(char str[]);
  
  
}


    /*
    * ������� ��� ������ ����� ��������� (��������) rinex �����.
    * ������:
    * 1. ���������� ����� (����� ����������� ��  ��������, �������� ������ ������ ���������, ���, �� ���� ������������, � �� �����).
    * 2. ����� �� ����� �� ����������, ����������� ��� ���������� ������.
    *************************************************************************************************************************************************************************/
void read_head_rinex(std::ofstream &fid_out, std::ifstream &fid_in, double xyz0[], double dxyz[]);

#endif
