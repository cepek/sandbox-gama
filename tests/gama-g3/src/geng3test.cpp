#include <iostream>
#include <sstream>

// ......................................................  .h
#include <string>
#include <vector>
#include <gnu_gama/ellipsoids.h>

using Tokens = std::vector<std::vector<std::string>>;
using std::cout;

struct gend3point {
  std::string id;               // point ID
  double B{0}, L{0}, H{0};      // ellipsoidal coordinates
  double X{0}, Y{0}, Z{0};      // ... corresponding XZY
  double dB{0}, dL{0}, dH{0};   // simulated coordinate errors
  enum Status {
    fixed, free, constr         // "constrained"
  } BL_status{fixed}, H_status{fixed};
};

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
  void write()
  {
    for (auto t1=tokens.begin(); t1!=tokens.end(); t1++)
    {
      std::vector<std::string> record = *t1;

      for (auto i=0; i<record.size(); i++)
      {
        //if (i == 0) std::cout << "record  ";
        std::cout << record[i] << " ";
      }
      std::cout << "\n";
    }
  }
private:
  Tokens tokens;

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
      "# ghilani-gnss-v1.xml \n"
      "# \n"

      "* A  fixed fixed    402.35087 -4652995.30109  4349760.77753      0 0 0\n"
      "* B  fixed fixed   8086.03178 -4642712.84739  4360439.08326      0 0 0\n"
      "* C  free  free   12046.58080 -4649394.08240  4353160.06450       0 0 0\n"
      "* D  free  free  43-23-16.3401747 -90-02-16.8958323 894.01416    0 0 0\n"
      "* E  free  free   -4919.33880 -4649361.21990  4352934.45480      0 0 0\n"
      "* F  free  free    1518.80120 -4648399.14540  4354116.69140      0 0 0\n"
      );

  geng3.read(sin);
  geng3.write();
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
  int line = 0;
  std::string str;
  while (std::getline(inp, str)) {
    std::string str_copy = str;  // used in a possible error message
    line++;

    std::istringstream istr_tokens(str);
    std::vector<std::string> vec_tokens;

    while (istr_tokens >> str) { // Extracts tokens, automatically skipping spaces
      vec_tokens.push_back(str);
    }

    if (vec_tokens.empty()) continue;      // skip empty records
    if (vec_tokens[0][0] == '#') continue; // skip comments

    if (vec_tokens[0] == "*")    tokens.push_back(vec_tokens);
    else
      std::cout << "ERROR LINE " <<  line
                << " bad data format: " << str_copy << std::endl;

    //std::cout << "\n";
  }
  return inp;
}
