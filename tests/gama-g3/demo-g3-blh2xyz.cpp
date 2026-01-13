#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <gnu_gama/ellipsoid.h>
#include <gnu_gama/latlong.h>
#include <gnu_gama/gon2deg.h>

using std::string;
using std::cout;
using std::endl;

int error = 0;

double s2d(string str)
{
  std::stringstream ss(str);
  double result;
  if ((ss >> result) && ss.eof() == true)
    {
      return result;
    }
  else
    {
      std::cerr << "Conversion from '" << str << "' to double failed\n";
      error++;

      return 0;
    }
}

int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "\nUsage: demo-g3-blh2xyz  B L H \n\n";
      return 1;
    }

  string Bstr = argv[1];
  string Lstr = argv[2];
  string Hstr = argv[3];

  double B, L;
  bool Bok = GNU_gama::deg2gon(Bstr, B);
  if (!Bok) {
    std::cerr << "Bad format B '" << Bstr <<"': must be 'bb-mm-ss.sss'\n";
    error++;
  }
  bool Lok = GNU_gama::deg2gon(Lstr, L);
  if (!Lok) {
    std::cerr << "Bad format L '" << Lstr <<"': must be 'll-mm-ss.sss'\n";
    error++;
  }
  double H = s2d(Hstr);

  if (error) return error;

  cout.precision(3);
  cout << "\nBLH  " << Bstr << "\n"
       << "     " << Lstr << "\n"
       << "     " << Hstr  << "\n\n";

  GNU_gama::Ellipsoid ellipsoid;  // WGS84

  B = B/200*M_PI;
  L = L/200*M_PI;

  double X, Y, Z;
  ellipsoid.blh2xyz(B, L, H, X, Y, Z);

  cout.precision(16);
  cout << "XYZ  " << X << "  " << Y << "  " << Z << endl;

  return 0;
}
