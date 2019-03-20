
namespace rinex
{
	const int N_TYPES_OF_OBS = 10;
	enum TYPES_OF_OBSERV {C1, C2, L1, L2, S1, S2, D1, D2, P1, P2, NoObs};
}

void read_head_rinex(char fname_alm_rinex_in[], char fname_alm_rinex_out[], double dxyz[], int gps_list[], int gln_list[], rinex::TYPES_OF_OBSERV typeObs[], double xyz0[]);
