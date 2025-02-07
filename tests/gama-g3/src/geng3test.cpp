#include <iostream>
#include <sstream>

// ......................................................  .h
#include <string>
#include <vector>
//#include <algorithm>
#include <gnu_gama/ellipsoids.h>

class GenG3 {
public:
  GenG3(GNU_gama::Ellipsoid e=GNU_gama::ellipsoid_wgs84);

  std::string ellipsoid_caption() const;
  std::string ellipsoid_id() const;
  std::string xml_header() const;
  std::string xml_end() const;

  std::string xml_points() const;
  std::string xml_observations() const;

  std::istream& read(std::istream&);

private:
  GNU_gama::Ellipsoid ell_id;

  std::string text() { return ""; }

  enum point_status_BLH {
    BLH_undefined = 0,
    BLH_fixed_fixed, BLH_fixed_free,   BLH_fixed_constr,
    BLH_free_free,   BLH_free_fixed,   BLH_free_constr,
    BLH_constr_free, BLH_constr_fixed, BLH_constr_constr
  };

  struct g3point {
    point_status_BLH status;
    std::string id;
  };

  std::vector<g3point> points;
};


// ......................................................

using std::cout;
using std::endl;

int main()
{
  GenG3 geng3;

  //char c;
  //while (input >> c) std::cout << c  << "...\n";

  if (0)cout << "# geng3test :   ellipsoid  "
       << "id = " << geng3.ellipsoid_id() << "   "
       << geng3.ellipsoid_caption() << "\n";

  cout << geng3.xml_header();
  cout << geng3.xml_points();
  cout << geng3.xml_observations();

  cout << geng3.xml_end();

  std::istringstream sin(
      "\tX  \n"
      "  \t \n"
      "*\t abcd        \n"
      "\tX\tY  \t  Z\n"
      );
  geng3.read(sin);
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

std::string GenG3::xml_points() const
{
  point_status_BLH blh_status{BLH_undefined}, prev_status{BLH_undefined};

  std::ostringstream s("\n<!-- Points -->\n\n");


  return s.str();
}

std::string GenG3::xml_observations() const
{
  std::ostringstream s("\n<!-- Observations -->\n\n");


  return s.str();
}

std::istream& GenG3::read(std::istream& inp)
{
  std::string str;
  while (std::getline(inp, str)) {
    std::cout << "read -->" << str << "<--\n";

    // Normalize whitespace to space using std::transform
    //std::transform(str.begin(), str.end(), str.begin(),
    //     [](char c) { return std::isspace(static_cast<unsigned char>(c)) ? ' ' : c; });

    std::istringstream tokens(str);
    std::string token;
    while (tokens >> token) { // Extracts tokens, automatically skipping spaces
      std::cout << "TOKEN ~~>" << token << "<~~\n";
    }
    std::cout << "\n";
  }
  return inp;
}
