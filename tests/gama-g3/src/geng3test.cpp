#include "geng3test.h"
#include "geng3test-md.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <charconv>

#include <gnu_gama/ellipsoid.h>
#include <gnu_gama/gon2deg.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int GenG3::error_count = 0;

const char* const main_help =
  "Usage: geng3test [input_file] | [options]\n"
  "    input_file  Process the specified input file (use -h for format details)\n"
  "Options:\n"
  "        -h  Display full program help in markdown format\n"
  "        -e  Show example input data\n"
  "        -r  Show results for the example input data\n"
  "\nUse -h for complete documentation.\n";


bool parse_double(const std::string& input, double& result) {
  const char* begin = input.data();
  const char* end = begin + input.size();

  auto [ptr, ec] = std::from_chars(begin, end, result);

  // Success only if conversion succeeded and no extra characters remain
  return ec == std::errc() && ptr == end;
}

int main(int argc, char* argv[])
{
  if (argc == 1) {
    cerr << main_help;
    return 1;
  }

  GenG3 geng3;

  const std::set<string> example {"-e", "-example", "--example"};
  const std::set<string> help    {"-h", "-help",    "--help"};
  const std::set<string> result  {"-r", "-result",  "--result"};

  for (int p=1; p<argc; p++) {
    if (example.find(string(argv[p])) != example.end()) {
      cout << geng3.example();
      return 0;
    }
    else if (result.find(string(argv[p])) != result.end()) {
      string example = geng3.example();
      std::istringstream istr(example);
      geng3.read(istr);
      geng3.write(cout);
      return 0;
    }
    else if (help.find(string(argv[p])) != help.end()) {
      cout << geng3.help();
      return 0;
    }
    else if (argv[p][0] == '-') { // unknown option
      cerr << main_help;
      return 1;
    }
  }

  std::istream* istr {};
  std::ifstream inpf;

  if (argc == 2) {
    inpf.open(argv[1]);
    istr = &inpf;
  }
  else {
    istr = &std::cin;
  }

  geng3.read(*istr);
  geng3.write(cout);

  cout << "\nTotal number of errors: " << GenG3::errors() << endl;

  return 0;
}


// ......................................................  .cpp

GenG3::GenG3(GNU_gama::Ellipsoid e) : ellipsoid(e)
{
}

std::string GenG3::ellipsoid_caption() const
{
  return GNU_gama::gama_ellipsoid_caption[ellipsoid.id];
}

std::string GenG3::ellipsoid_id() const
{
  return GNU_gama::gama_ellipsoid_id[ellipsoid.id];
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
  error_count = 0;
  line_count  = 0;
  std::string str;

  while (std::getline(inp, current_line))
  {
    line_count++;
    std::istringstream istr_tokens(current_line);
    std::vector<std::string> vec_tokens;

    std::string str;
    while (istr_tokens >> str)    // Extracts tokens, skipping ws characters
    {
      if (str[0] == '#') break;   // leading and trailing comments are ignored
      vec_tokens.push_back(str);
    }
    if (vec_tokens.empty()) continue;      // skip empty records

    if (vec_tokens[0] == "*")
    {
      if (vec_tokens.size() != 10) {
        error("Wrong number of tokens, must be 10");
        continue;
      }

      const std::set<std::string> position {"fixed", "free", "constr"};
      bool position_error = false;
      if (position.find(vec_tokens[2]) == position.end()) position_error = true;
      if (position.find(vec_tokens[3]) == position.end()) position_error = true;
      if (position_error) {
        error("Bad coordinates status, must be fixed, free or constr");
        continue;
      }

      // point coordinates XYZ or BLH

      double X, Y, Z;
      double B, L, H;

      bool is_X = parse_double(vec_tokens[4], X);
      bool is_Y = parse_double(vec_tokens[5], Y);
      bool is_Z = parse_double(vec_tokens[6], Z);

      if (is_X && is_Y && is_Z)   // the triple of carthesian coordinate
      {
        ellipsoid.xyz2blh(X, Y, Z,  B, L, H);
      }
      else if (is_Z)   //possibly ellipsoidal coordinates BLH
      {
        H = Z;
        bool B_ok = GNU_gama::deg2gon(vec_tokens[4], B);
        bool L_ok = GNU_gama::deg2gon(vec_tokens[5], L);

	if (B_ok && L_ok)
	{
	  ellipsoid.blh2xyz(B, L, H,  X, Y, Z);
	}
	else
	{
	  error("Format error in BL coordinates");
	  continue;
	}
      }
      else
      {
        error("Format error in XYZ / BLH triple");
        continue;
      }

      double db, dl, dh;
      bool err_db = !parse_double(vec_tokens[7], db);
      bool err_dl = !parse_double(vec_tokens[8], dl);
      bool err_dh = !parse_double(vec_tokens[9], dh);
      if (err_db || err_dl || err_dh) {
        error("Bad numeric format in dB / dL / dH");
        continue;
      }

      tokens.push_back(vec_tokens);
    }
    else
    {
      error("unknown record type");
    }

  }
  return inp;
}

void GenG3::write(std::ostream& output)
{
#ifdef GenG3_DEBUG
  cerr << "\n";
#endif

for (auto t1=tokens.begin(); t1!=tokens.end(); t1++)
  {
    std::vector<std::string> record = *t1;

#ifdef GenG3_DEBUG
  for (auto i=0; i<record.size(); i++)
  {
    if (i == 0) cerr << "size " << record.size() << " record  ";
    cerr << record[i] << " ";
  }
  cerr << "\n";
#endif

    // * A fixed fixed 402.35087 -4652995.30109 4349760.77753 0 0 0


  }
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

* A  fixed fixed    402.35087 -4652995.30109  4349760.77753      0 0 0  # trailing comment
* B  fixed fixed   8086.03178 -4642712.84739  4360439.08326      0 0 0
* C  free  free   12046.58080 -4649394.08240  4353160.06450      0 0 0
* D  free  free  43-23-16.3401747 -90-02-16.8958323 894.01416    0 0 0
* E  free  free   -4919.33880 -4649361.21990  4352934.45480      0 0 0
* F  free  free    1518.80120 -4648399.14540  4354116.69140      0 0 0
)GHILANI_V1";

#ifdef GenG3_DEBUG
  std::string errors =
R"ERRORS(
# ERRORS
#
* err01  fixed fixed    402.35087 -4652995.30109  4349760.77753   0 0     # not ennough tokens
* err02  fixed fixed    402.35087 -4652995.30109  4349760.77753   0 0 0 0 # too many tokens
* err03  fixe  fixed    402.35087 -4652995.30109  4349760.77753   0 0 0   # bad status fixe
* err04  fixed  ixed    8086.03178 -4642712.84739  4360439.08326  0 0 0   # bad status ixed

* err05  free  free  43-23-16.3401747err -90-02-16.8958323 894.01416    0 0 0 # bad format B
* err06  free  free  43-23-16.3401747 -90-02-16.8958323err 894.01416    0 0 0 # bad format L
* err07  fixed fixed    402.35087 -4652995.30109  4349760.77753   db 0 0  # bad number format dB
* err08  fixed fixed    402.35087 -4652995.30109  4349760.77753   0 dl 0  # bad number format dL
* err09  fixed fixed    402.35087 -4652995.30109  4349760.77753   0 0 dh  # bad number format dH

)ERRORS";
#else
  std::string errors {};
#endif


  return header + data + errors;
}

std::string GenG3::help() const
{
  return geng3test_help_md;
}

void GenG3::error(std::string message)
{
  const int line_nuber_width {3};
  std::cerr << "line " << std::setw(line_nuber_width) << line_count
            << " : "  << current_line << std::endl
            << "    " << std::setw(line_nuber_width+3)
            << "error: " << message << std::endl;

  error_count++;
}
