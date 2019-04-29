

namespace ALM_CONST
{
  const int _SEC_DAY_  = 24 * 60 * 60;
  const int _SEC_WEEK_ = 7 * _SEC_DAY_;

  typedef struct
  {
    double M;
    double OmE;
  } GPS_CONST;

  extern const GPS_CONST gpsConst;
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
	double n0
public:
	void calcPosition(int week, double tow);
	void read_alm(char *fname, int N);

	void dbg();

};

/*
void read_alm(char *fname, ALM_GLN *p, int N = 1);
*/
