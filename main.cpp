#include <iostream>
#include <fstream>

using namespace std;

int config_processing(char *fname_alm_gps, char *fname_alm_gln, char *fname_rinex_in, char *fname_rinex_out, double dx[]);
int calc_length_string(ifstream &fid);
void goToNextString(ifstream &fid);

const char fname_config[] = "config.inf";

int main()
{


    char *fname_alm_gps = 0;
    char *fname_alm_gln = 0;
    char *fname_alm_rinex_in = 0;
    char *fname_alm_rinex_out = 0;
    double dxyz[3] = {0. , 0., 0.};

    if (config_processing(fname_alm_gps, fname_alm_gln, fname_alm_rinex_in, fname_alm_rinex_out, dxyz))
        return 1;




    return 0;
}





int config_processing(char *fname_alm_gps, char *fname_alm_gln, char *fname_rinex_in, char *fname_rinex_out, double dx[])
{
    ifstream fid;
    int N;
    try
    {
        fid.open(fname_config);
        if(!fid.is_open())
            throw 1;

        N = calc_length_string(fid);
        fname_alm_gps = new char[N + 1];
        fid.getline(fname_alm_gps, N + 1);
        goToNextString(fid);

        N = calc_length_string(fid);
        fname_alm_gln = new char[N + 1];
        fid.getline(fname_alm_gln, N + 1);
        goToNextString(fid);

        N = calc_length_string(fid);
        fname_rinex_in = new char[N + 1];
        fid.getline(fname_rinex_in, N + 1);
        goToNextString(fid);

        N = calc_length_string(fid);
        fname_rinex_out = new char[N + 1];
        fid.getline(fname_rinex_out, N + 1);
        goToNextString(fid);

        for (int k = 0; k < 3; k++)
            fid >> dx[k];

    }
    catch (int n_err)
    {
        switch (n_err)
        {
        case 1:
            {
                cout << "Error: The file " << fname_config << " was not open!" << endl;
                break;
            }
        case 2:
        default:
            {
                cout << "Error: Unknown error. Check " << fname_config << " ." << endl;
            }
        }

        fid.close();
        return 1;
    }
    fid.close();

    cout << "GPS alm  : " << fname_alm_gps << endl;
    cout << "GLN alm  : " << fname_alm_gln << endl;
    cout << "rinex in : " << fname_rinex_in << endl;
    cout << "rinex out: " << fname_rinex_out << endl;

    cout << "dX, dY, dZ = ";
    for (int k = 0; k < 3; k++)
        cout << dx[k];
    cout << endl;

    return 0;
}


int calc_length_string(ifstream &fid)
{
    int n = -1;
    char ch;
    do
    {
        fid.get(ch);
        cout << ch;
        n++;
    } while( ch != '\t' && ch != ' ' && ch != '\n' && ch != '#');
    fid.seekg(-n, ios::cur);
    return n;
}

void goToNextString(ifstream &fid)
{
    fid.ignore(521, '\n');
    return;
    char ch;
    int n = 0;
    do
    {
        n++;
        fid.get(ch);
        cout << ch;
    }
    while( ch != '\n' && n < 100);
    cout << "read " << n << endl;
}
