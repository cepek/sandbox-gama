#include <iostream>
#include <sstream>

// ......................................................  .h
#include <string>
#include <gnu_gama/ellipsoids.h>

class GenG3 {
public:
  GenG3(GNU_gama::Ellipsoid e=GNU_gama::ellipsoid_wgs84);

  std::string ellipsoid_caption() const;
  std::string ellipsoid_id() const;
  std::string xml_header() const;
  std::string xml_end() const;

private:
  GNU_gama::Ellipsoid ell_id;

  std::string text() { return ""; }
};


// ......................................................
using std::cout;
using std::endl;

int main()
{
  GenG3 geng3;
  if (0)cout << "# geng3test :   ellipsoid  "
       << "id = " << geng3.ellipsoid_id() << "   "
       << geng3.ellipsoid_caption() << "\n";

  cout << geng3.xml_header();

  cout << geng3.xml_end();
  return 0;
}


// ......................................................  .cpp
GenG3::GenG3(GNU_gama::Ellipsoid e) : ell_id(e)
{
}

std::string GenG3::ellipsoid_caption() const
{
  return GNU_gama::gama_ellipsoid_caption[ell_id.id];
}

std::string GenG3::ellipsoid_id() const
{
  return GNU_gama::gama_ellipsoid_id[ell_id.id];
}

std::string GenG3::xml_header() const
{
  std::ostringstream s;
  s << "<?xml version=\"1.0\" ?>\n"
    << "<gnu-gama-data xmlns="
    << "\"http://www.gnu.org/software/gama/gnu-gama-data\">\n\n";

  s << "<text>" << "geng3test" << "</text>\n\n";

  s << "<g3-model>\n"
    << "   <constants>\n"
    << "      <apriori-standard-deviation>" << 10
    <<        "</apriori-standard-deviation>\n"
    << "      <confidence-level>" << 0.95 << "</confidence-level>\n"
    << "      <angular-units-gons/>\n"
    << "      <ellipsoid>" << ellipsoid_id() << "</ellipsoid>\n"
    << "   </constants>\n";



  return s.str();
}

std::string GenG3::xml_end() const
{
  return "\n</g3-model>\n</gnu-gama-data>\n";
}
