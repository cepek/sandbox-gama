#include "geng3test.h"
#include "geng3test-md.h"

#include <iostream>
#include <sstream>
#include <set>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[])
{
  GenG3 geng3;

  const std::set<string> example {"-e", "-example", "--example"};
  const std::set<string> help    {"-h", "-help",    "--help"};

  for (int p=1; p<argc; p++) {
    if (example.find(string(argv[p])) != example.end()) {
      cout << geng3.example();
      return 0;
    } else if (help.find(string(argv[p])) != help.end()) {
      cout << geng3.help();
      return 0;
    }
  }

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

std::string GenG3::example() const
{
  std::string header =
      "# geng3test :  file = ghilani-gnss-v1.xml\n"
      "# ellipsoid id = " + ellipsoid_id() +
      "   " + ellipsoid_caption() + "\n\n";

  std::string data =
R"GHILANI_V1(# Example from Section 17.8
#
# Ghilani Charles D. (2010): Adjustment Computations. Spatial Data
# Analysis. Fifth Edition, John Wiley &amp; Sons, Inc.,
# ISBN 16 978-0-470-46491-5, Ch. 17.6, p 337-352

* A  fixed fixed    402.35087 -4652995.30109  4349760.77753      0 0 0
* B  fixed fixed   8086.03178 -4642712.84739  4360439.08326      0 0 0
* C  free  free   12046.58080 -4649394.08240  4353160.06450      0 0 0
* D  free  free  43-23-16.3401747 -90-02-16.8958323 894.01416    0 0 0
* E  free  free   -4919.33880 -4649361.21990  4352934.45480      0 0 0
* F  free  free    1518.80120 -4648399.14540  4354116.69140      0 0 0

)GHILANI_V1";

  return header + data;
}

std::string GenG3::help() const
{  
  return geng3test_help_md;
}

