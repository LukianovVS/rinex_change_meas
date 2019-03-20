

class BASE_ALM
{
protected:
	double xyz[3];
	double V[3];
public:
	
	virtual void calc_XV() = 0;
	virtual void read_alm(char *fname, int N) = 0;
// 															// interface
	void get_x(double *x);
	void get_v(double *v);
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
public:
	void calc_XV();
	void read_alm(char *fname, int N);
	
	void dbg();
	
};

/*
void read_alm(char *fname, ALM_GLN *p, int N = 1);
*/