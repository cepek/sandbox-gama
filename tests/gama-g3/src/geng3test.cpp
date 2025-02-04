#include <iostream>
#include <gnu_gama/ellipsoids.h>


using namespace std;

int main()
{
  GNU_gama::Ellipsoid e(GNU_gama::ellipsoid_wgs84);

  cout << "Hello Geng3test!    " <<
      GNU_gama::gama_ellipsoid_caption[e.id] << endl;
  return 0;
}
