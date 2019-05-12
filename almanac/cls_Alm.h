

namespace ALM_CONST
{
  const int _SEC_DAY_  = 24 * 60 * 60;
  const int _SEC_WEEK_ = 7 * _SEC_DAY_;

  typedef struct
  {
    double M;
    double OmE;
  } GPS_CONST;

  typedef struct
  {
    double i0;
    double C_20;
    double a_earth;
    double w_earth;
    double nu_earth;
    double T_mean;
  } GLN_CONST;

  extern const GPS_CONST gpsConst;
  extern const GLN_CONST glnConst;
}

class BASE_ALM
{
protected:
	double xyz[3];
	double V[3];
public:
  virtual void calcPosition(int week, double tow) = 0;
	virtual void read_alm(char *fname, int N) = 0;
// 															// interface
	void inline get_x(double *x) {x[0] = xyz[0]; x[1] = xyz[1]; x[2] = xyz[2];}
	void inline get_v(double *v) {v[0] =   V[0]; v[1] =   V[1]; v[2] =   V[2];}
};

class ALM_GPS : public BASE_ALM
{
private:
	short int prn;
	short int Health;
	int week;
	double t_oe;
	int day;
	int month;
	int year;
	double t_al;
	double af0;
	double af1;
	double Om1;
	double Om0;
	double i;
	double w;
	double E;
	double SQRT_A;
	double M0;
	//  next parameters does not readed of the almanac
	double A;
	double n0;
public:
	void calcPosition(int week, double tow);
	void read_alm(char *fname, int N);

};


class ALM_GLN : public BASE_ALM
{
private:
  short int N_sat;
  short int ch_freq;                                                                                                    // Number of frequency channel
  short int Health;                                                                                                     // инвертируется при чтении альманаха (1 - болен, 0 - здоров)
  int day;
	int month;
	int year;
	double sec;
	double dt_gln2utc;
	double dt_gps2gln;
	double dt_sat;

	double Lam;
	double dI;
	double w;
	double E;
	double dT0;
	double dT1;
  // +++
  double i;

  double week;
  double tow;

public:
  void calcPosition(int week, double tow);
  void read_alm(char *fname, int N);

};


inline void dmyhms2WnTow (int *wn, double *t, int c_day, int c_month, int c_year, double c_h, double c_min, double c_sec)
{
  dmys2WnTow(wn, t, c_day, c_month, c_year, c_h * 3600 + c_min * 60 + c_sec);
}
