/* cout.precision(n+PREC) sets maximum output precision for double
 * in attempt to minimize rounding errors
 */
const int PREC = 16;

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include <vector>

using namespace std;

random_device r;
mt19937 rgen(r());
uniform_real_distribution<> rdis(0,1);

std::ofstream ofstr1, ofstr2;
std::vector<std::ostream*> output;

const  int N_max = 100;
int        N_test {};

struct XYZ { double x{}, y{}, z{}; };
vector<XYZ>    Point(N_max);

vector<string> point_type = {
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",

  " fix='xyz' ",
  " fix='xy' ",
  " fix='xy' adj='z' ",
  " fix='xy' adj='z' ",
  " fix='xy' adj='Z' ",
  " fix='xy' adj='Z' ",
  " fix='z' ",
  " fix='z' adj='xy' ",
  " fix='z' adj='xy' ",
  " fix='z' adj='XY' ",
  " fix='z' adj='XY' ",
};

struct Dh
{
  double from, to;

  Dh()
  {
    from = to = 0;
    double rf = rdis(rgen);
    if (rf < 0.5) from = 0.75 +  rf*2;
    double rt = rdis(rgen);
    if (rt < 0.5) to = 0.75 +  rt*2;
  }

  double diff() { return to - from; }
};

ostream& operator<<(ostream& out, Dh dh)
{
  auto p = out.precision();
  out.precision(3+PREC);
  if (dh.from != 0) out << " from_dh='" << dh.from << "'";
  if( dh.to   != 0) out << " to_dh='"   << dh.to   << "'";
  out.precision(p);
  return out;
}

using Function = void (*)(int);

void observations(int);
void heights(int);
void coordinates(int);
void vectors(int);

vector<Function> cluster = {observations, observations, observations,
                            heights,      coordinates,  vectors};


double xyz()
{
  return rdis(rgen)*1000.0;
}

int random_point_type_index()
{
  return int( rdis(rgen) * point_type.size());
}

int random_index(int M)
{
  return int( rdis(rgen) * M );
}

int target(int from)
{
  int to;
  do {
    to = random_index(N_test);
  } while (from == to);
  return to;
}

void dh(int from)
{
  int to = target(from);
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout.precision(3+PREC);
      cout << "<dh from='" << from << "' to='" << to << "'"
           << " val='" << Point[to].z - Point[from].z << "'"
           << " stdev='2.0' />\n";
    }
}

void dist(int from)
{
  int to = target(from);
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout.precision(4+PREC);
      double dx = Point[to].x - Point[from].x;
      double dy = Point[to].y - Point[from].y;
      double d  = sqrt(dx*dx + dy*dy);
      cout << "<distance from='" << from << "' to='" << to << "'"
           << " val='" << d << "' stdev='4.0' />\n";
    }
}

void sdist(int from)
{
  Dh dh;

  int to = target(from);
  double dx = Point[to].x - Point[from].x;
  double dy = Point[to].y - Point[from].y;
  double dz = Point[to].z - Point[from].z;
  double D  = sqrt(dx*dx + dy*dy + dz*dz);
  dz += dh.diff();
  double d  = sqrt(dx*dx + dy*dy + dz*dz);
  bool first=true;
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout.precision(4+PREC);
      if (first) {
          cout << "<s-distance from='" << from << "' to='" << to << "'"
               << " val='" << d << "'" << dh << " stdev='5.0' />\n";
          first=false;
        }
      else {
          cout << "<s-distance from='" << from << "' to='" << to << "'"
               << " val='" << D << "' stdev='5.0' />\n";
        }
    }
}

void dir(int from)
{

  for (int i=0; i<=1+random_index(4); i++)
    {
      int to = target(from);
      double dx = Point[to].x - Point[from].x;
      double dy = Point[to].y - Point[from].y;
      double s  = atan2(dy, dx)/M_PI*200;
      if (s < 0) s += 400;
      for (auto ostr : output)
        {
          auto& cout = *ostr;
          cout.precision(4+PREC);
          cout << "<direction to='" << to << "'"
               << " val='" << s << "' stdev='6.0' />\n";
        }
    }
}

void zen(int from)
{
  Dh dh;

  int   to;
  double dx, dy, dz, s, dZ, S;
  do
    {
      to = target(from);
      cout.precision(4+PREC);
      dx = Point[to].x - Point[from].x;
      dy = Point[to].y - Point[from].y;
      dz = Point[to].z - Point[from].z + dh.diff();
      s  = sqrt(dx*dx + dy*dy + dz*dz);

      dZ = Point[to].z - Point[from].z;
      S  = sqrt(dx*dx + dy*dy + dZ*dZ);
    }
  while(s < 1);  // less than 1m
  double z = acos(dz/s)/M_PI*200;
  double Z = acos(dZ/S)/M_PI*200;
  bool first=true;
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout.precision(4+PREC);

      if (first) {
          cout << "<z-angle from='" << from << "' to='" << to << "'"
               << " val='" << z << "'" << dh << " stdev='7.0' />\n";
          first=false;
        }
      else {
          cout << "<z-angle from='" << from << "' to='" << to << "'"
               << " val='" << Z << "' stdev='7.0' />\n";
        }
    }
}

vector<Function> obs = {  dir, dist, sdist, zen };

void observations(int from)
{
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<obs from='" << from << "'>\n";
    }

  for (int i=0; i<=random_index(5); i++) obs[random_index(obs.size())](from);

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "</obs>\n\n";
    }
}

void heights(int from)
{
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<height-differences>\n";
    }

  for (int i=0; i<=random_index(5); i++) dh(from);

  for (auto ostr : output)
    {
      auto& cout = *ostr;cout << "</height-differences>\n\n";
    }
}

void coordinates(int from)
{
  cout.precision(3+PREC);
  int p[] = {0, 0, 0};

  int test=0;
  do
    {
      p[0] = target(from);
      p[1] = target(from);
      p[2] = target(from);
      if (test++ > N_max) return;
    }
  while (p[0] == p[1] || p[0] == p[2] || p[1] == p[2]);

  int dim=0;

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<coordinates>\n";
    }

  for (int i=0; i<=random_index(3); i++)
    {
      int k = random_index(3);
      bool first = true;
      for (auto ostr : output)
        {
          auto& cout = *ostr;
          cout.precision(3+PREC);
          cout << "<point id='" << p[i] << "'";
          switch (k)
            {
            case 0:
              if (first) dim += 3;
              cout << " x='" << Point[p[i]].x << "'";
              cout << " y='" << Point[p[i]].y << "'";
              cout << " z='" << Point[p[i]].z << "'";
              break;
            case 1:
              if (first) dim += 2;
              cout << " x='" << Point[p[i]].x << "'";
              cout << " y='" << Point[p[i]].y << "'";
              break;
            case 2:
              if (first) dim += 1;
              cout << " z='" << Point[p[i]].z << "'";
              break;
            };
          cout << " />\n";
          first = false;
        }
    }

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<cov-mat dim='" << dim << "' band='" << dim-1 << "'>\n";
      for (int s=1, i=1; i<=dim; i++)
        {
          cout << "25";
          for (int k=1; k<=dim-1 && i+k<=dim; k++, s=-s) cout << " " << s;
          cout << endl;
        }
      cout << "</cov-mat>\n";

      cout << "</coordinates>\n\n";
    }
}

void vectors(int from)
{
  cout.precision(3+PREC);
  int p[] = {0, 0, 0};

  int test=0;
  do
    {
      p[0] = target(from);
      p[1] = target(from);
      p[2] = target(from);
      if (test++ > N_max) return;
    }
  while (p[0] == p[1] || p[0] == p[2] || p[1] == p[2]);

  int dim=0;

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<vectors>\n";
    }

  for (int i=0; i<=random_index(3); i++)
    {
      dim += 3;
      int to = target(p[i]);
      for (auto ostr : output)
        {
          auto& cout = *ostr;
          cout.precision(3+PREC);
          cout << "<vec from='" << p[i] << "' to='" << to << "'"
               << " dx='" << Point[to].x - Point[p[i]].x << "'"
               << " dy='" << Point[to].y - Point[p[i]].y << "'"
               << " dz='" << Point[to].z - Point[p[i]].z << "'"
               << " />\n";
        }
    }

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<cov-mat dim='" << dim << "' band='" << dim-1 << "'>\n";
      for (int s=1, i=1; i<=dim; i++)
        {
          cout << "25";
          for (int k=1; k<=dim-1 && i+k<=dim; k++, s=-s) cout << " " << s;
          cout << endl;
        }
      cout << "</cov-mat>\n";

      cout << "</vectors>\n\n";
    }
}

bool open(int argc, char* argv[])
{
  for (int i=1; i<argc; i++)
    {
      std::string h(argv[i]);
      if (h == "-h"    || h == "--h"    ||
          h == "-help" || h == "--help" || argc > 3)
        {
          std::cerr << "\ngen-test [ file1 | - [ file2 | - ]]\n\n";
          return false;
        }
    }

  if (argc == 2 || argc == 3) {
      std::string a(argv[1]);
      if (a == "-") {
          output.push_back(&std::cout);
        }
      else {
          ofstr1.open(a);
          output.push_back(&ofstr1);
        }
    }

  if (argc == 3) {
      std::string b(argv[2]);
      if (b == "-") {
          output.push_back(&std::cout);
        }
      else {
          ofstr2.open(b);
          output.push_back(&ofstr2);
        }
    }

  if (output.empty()) output.push_back(&std::cout);

  return true;
}

// #######################################################################

int main(int argc, char* argv[])
{
  if (!open(argc, argv)) return 1;

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout.setf(ios::fixed, ios::floatfield);
    }

  do
    N_test = random_index(N_max);
  while (N_test < 2);

  for (int i=0; i<N_test; i++)
    {
      Point[i].x = xyz();
      Point[i].y = xyz();
      Point[i].z = xyz();
    }

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "<?xml version='1.0' ?>\n"
           << "<gama-local xmlns='http://www.gnu.org/software/gama/gama-local'>\n"

           << "<network>\n\n"

           << "<description>\n"
           << "gen-test 3.00\n"
           << "</description>\n\n"

           << "<points-observations>\n\n";
    }

  for (int i=0; i<N_test; i++)
    {
      auto pt = point_type[random_point_type_index()];
      for (auto ostr : output)
        {
          auto& cout = *ostr;
          cout.precision(2+PREC);
          cout << "<point id='" << i << "'"
               << " x='" << Point[i].x << "'"
               << " y='" << Point[i].y << "'"
               << " z='" << Point[i].z << "'"
               << pt
               << "/>\n";
        }
    }
  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << endl;
    }

  for (int i=0; i<N_test; i++)
    for (int j=0; j<random_index(3); j++)
      {
        cluster[random_index(cluster.size())](i);
      }

  for (auto ostr : output)
    {
      auto& cout = *ostr;
      cout << "</points-observations>\n"
           << "</network>\n"
           << "</gama-local>\n\n";
    }
}

