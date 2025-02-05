#include <iostream>

#include <string>
#include <gnu_gama/ellipsoids.h>

class GenG3 {
public:
  GenG3(GNU_gama::Ellipsoid e=GNU_gama::ellipsoid_wgs84);

  std::string ellipsoid_caption() const;
  int         ellipsoid_id() const;

private:
  GNU_gama::Ellipsoid ell;
};


// ......................................................
using std::cout;
using std::endl;

int main()
{
  cout << "Hello Geng3test!" << endl;

  GenG3 geng3;
  cout << "id = " << geng3.ellipsoid_id() << "  " << geng3.ellipsoid_caption() << "\n";

  return 0;
}


// ......................................................
GenG3::GenG3(GNU_gama::Ellipsoid e) : ell(e)//=GNU_gama::ellipsoid_wgs84)
{
}

std::string GenG3::ellipsoid_caption() const
{
  return GNU_gama::gama_ellipsoid_caption[ell.id];
}

int GenG3::ellipsoid_id() const
{
  return ell.id;
}
