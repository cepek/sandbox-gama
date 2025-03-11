#include "geng3test.h"
#include "geng3test-md.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <charconv>
#include <regex>

#include <gnu_gama/version.h>

namespace
{
  class StreamGuard {
    std::ios& stream;
    std::ios::fmtflags flags;
    std::streamsize precision;

  public:
    explicit StreamGuard(std::ios& s)
        : stream(s), flags(s.flags()), precision(s.precision()) {}

    ~StreamGuard() {
      stream.flags(flags);
      stream.precision(precision);
    }
  };
}

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
  "        -v  Print the program version\n"
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
  const std::set<string> version {"-v", "-vesion",  "--version"};

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
    else if (version.find(string(argv[p])) != version.end()) {
      cout << geng3.version();
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

  cerr << "\nTotal number of errors: " << GenG3::errors() << endl;

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

  s << "<text>" << "geng3test " << version() << "</text>\n\n";

  s << "<g3-model>\n"
    << "   <constants>\n"
    << "      <apriori-standard-deviation>" << 10
    <<        "</apriori-standard-deviation>\n"
    << "      <confidence-level>" << 0.95 << "</confidence-level>\n"
    << "      <angular-units-gons/>\n"
    << "      <ellipsoid><id>" << ellipsoid_id() << "</id></ellipsoid>\n"
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

  std::ostringstream s("\n<!-- Points -->\n", std::ios::ate);

  StreamGuard guard(s);
  s.setf(std::ios::fixed);
  s.precision(5);

  int blh_prev {-1}, blh_code {0};

  for (auto p=points.begin(); p!=points.end(); p++) {

    const geng3point g3p = p->second;

    if      (g3p.BL_status == "fixed")  blh_code =  0;
    else if (g3p.BL_status == "free")   blh_code = 10;
    else if (g3p.BL_status == "constr") blh_code = 20;
    else blh_code = -100;  // error code
    if      (g3p.H_status  == "fixed")  blh_code += 0;
    else if (g3p.H_status  == "free")   blh_code += 1;
    else if (g3p.H_status  == "constr") blh_code += 2;
    else blh_code = -100;

    if (blh_code != blh_prev) {
      s << "\n";
      switch (blh_code)
      {
        case  0+0: s << "<fixed> <n/> <e/> <u/> </fixed>";
          break;
        case  0+1: s << "<fixed> <n/> <e/> </fixed> <free> <u/> </free>";
          break;
        case  0+2: s << "<fixed> <n/> <e/> </fixed> <constr> <u/> </constr>";
          break;

        case 10+0: s << "<free> <n/> <e/> </free> <fixed> <u/> </fixed>";
	  break;
	case 10+1: s << "<free> <n/> <e/> <u/> </free>";
	  break;
	case 10+2: s << "<free> <n/> <e/> </free> <constr> <u/> </constr>";
	  break;

	case 20+0: s << "<constr> <n/> <e/> </constr> <fixed> <u/> </fixed>";
	  break;
	case 20+1: s << "<constr> <n/> <e/> <constr>  <free> <u/> </free>";
	  break;
	case 20+2: s << "<constr> <n/> <e/> <u/> </constr>";
	  break;

	default:
	  s << "<UNKNOWN-STATUS>"
	    << " <n/> <e/> " << g3p.BL_status << " <u/> " << g3p.H_status
	    << " </UNKNOWN-STATUS>";    // generate XML error
	  break;
      }
      s << "\n\n";
    }
    blh_prev = blh_code;

    s << "<point> <id>" << g3p.id << "</id>"
      << " <x>" << g3p.X << "</x>"
      << " <y>" << g3p.Y << "</y>"
      << " <z>" << g3p.Z << "</z>"
      << " </point>\n";
  }

  return s.str();
}

std::string GenG3::xml_observations() const
{
  return ostrobs.str();   // initialized by GenG3::read_obs(std::istream&)
}

std::istream& GenG3::read(std::istream& inp)
{
  std::regex  input_regex("^<<<<*.*");
  std::cmatch input_match;

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
      if (str[0] == '#')
      {
        break;   // leading and trailing comments are ignored
      }
      vec_tokens.push_back(str);
    }
    if (vec_tokens.empty()) continue;      // skip empty records


    if (vec_tokens[0] == "*")
    {
      if (vec_tokens.size() != 10) {
        error("Wrong number of tokens, must be 10");
        continue;
      }

      geng3point g3p;
      g3p.id = vec_tokens[1];

      const std::set<std::string> position {"fixed", "free", "constr"};
      bool position_error = false;
      if (position.find(vec_tokens[2]) == position.end()) position_error = true;
      if (position.find(vec_tokens[3]) == position.end()) position_error = true;
      if (position_error) {
        error("Bad coordinates status, must be fixed, free or constr");
        continue;
      }

      g3p.BL_status = vec_tokens[2];
      g3p.H_status  = vec_tokens[3];

      // point coordinates XYZ or BLH

      double X, Y, Z;
      double B, L, H;

      bool valid_X = parse_double(vec_tokens[4], X);
      bool valid_Y = parse_double(vec_tokens[5], Y);
      bool valid_Z = parse_double(vec_tokens[6], Z);

      if (valid_X && valid_Y && valid_Z)   // the triple of carthesian coordinate
      {
        ellipsoid.xyz2blh(X, Y, Z,  B, L, H);
      }
      else if (valid_Z)   // possibly ellipsoidal coordinates BLH
      {
        H = Z;
        bool valid_B = GNU_gama::deg2gon(vec_tokens[4], B);
        bool valid_L = GNU_gama::deg2gon(vec_tokens[5], L);

        if (valid_B && valid_L)
        {
          // M_PI is defined in <gnu_gama/gon2deg.h>
          B *= M_PI/200;    // gon to rad
          L *= M_PI/200;

          ellipsoid.blh2xyz(B, L, H,  X, Y, Z);
        #if 0
          if (g3p.id == "D") {
              cerr.setf(std::ios::fixed);
              cerr.precision(6);
              cerr << "*** blh2xyz point D diff XYZ "
                   << X - -3081.58313 << " " << Y - -4643107.36915 << " " << Z - 4359531.12333 << endl;
          }
        #endif
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

      g3p.X = X;
      g3p.Y = Y;
      g3p.Z = Z;

      g3p.B = B;
      g3p.L = L;
      g3p.H = H;

      double db, dl, dh;
      bool valid_db = !parse_double(vec_tokens[7], db);
      bool valid_dl = !parse_double(vec_tokens[8], dl);
      bool valid_dh = !parse_double(vec_tokens[9], dh);
      if (valid_db || valid_dl || valid_dh) {
        error("Bad numeric format in dB / dL / dH");
        continue;
      }

      const double seconds = 180*60*60;
      g3p.dB = db/seconds * M_PI;    // arc seconds to radians
      g3p.dL = dl/seconds * M_PI;
      g3p.dH = dh/1000;              // millimeters to meters

      g3p.errB = g3p.B + g3p.dB;
      g3p.errL = g3p.L + g3p.dL;
      g3p.errH = g3p.H + g3p.dH;

      ellipsoid.blh2xyz(g3p.errB, g3p.errL, g3p.errH,
                        g3p.errX, g3p.errY, g3p.errZ);

      points[g3p.id] = g3p;

    } // "*" point record

    else if (std::regex_match(vec_tokens[0].c_str(), input_match, input_regex))
    {
      // std::ostringstream s;
      read_obs(inp);
    }
    else
    {
      error("Unknown record type");
    }

  }

  return inp;
}

std::istream& GenG3::read_obs(std::istream& inp)
{
  ostrobs << "\n<!-- Observations -->\n";

  while (std::getline(inp, current_line))
  {
    line_count++;
    std::istringstream istr_tokens(current_line);
    std::vector<std::string> vec_tokens;
    std::string str;

    while (istr_tokens >> str)  vec_tokens.push_back(str);
    if (vec_tokens.empty()) {
      ostrobs << "\n";
      continue;
    }

    if (vec_tokens[0] == "<vector>")
    {
      if  (vec_tokens.size() == 3)
      {
        geng3point g3p_from = points[vec_tokens[1]];
        double Xfrom = g3p_from.errX;
        double Yfrom = g3p_from.errY;
        double Zfrom = g3p_from.errZ;

	geng3point g3p_to = points[vec_tokens[2]];
	double Xto = g3p_to.errX;
	double Yto = g3p_to.errY;
	double Zto = g3p_to.errZ;

	double dx = Xto - Xfrom;
	double dy = Yto - Yfrom;
	double dz = Zto - Zfrom;

	StreamGuard ostr(ostrobs);
	ostrobs.setf(std::ios::fixed);
	ostrobs.precision(5);
	{
	  for (int i=0; i<current_line.length(); i++)  // keep original spacing
	  {
	    if (!std::isspace(current_line[i])) break;
	    ostrobs << " ";
	  }
	  ostrobs << "<vector>"
		  << " <from>" << vec_tokens[1] << "</from> "
		  << " <to>"   << vec_tokens[2] << "</to> "
		  << " <dx>" << dx << "</dx> "
		  << " <dy>" << dy << "</dy> "
		  << " <dz>" << dz << "</dz> "
		<< " </vector>" << endl;
	}
      }
      else
      {
        error("bad number of tokens, must be 3");
      }
    }
    else
    {
      ostrobs << current_line << "\n";
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
  }

  output << xml_header();
  output << xml_points();
  output << xml_observations();
  output << xml_end();
}


std::string GenG3::example() const
{
  std::string header =
      "# geng3test " + version() + "\n"
      "# file = ghilani-gnss-v1.xml\n"
      "# ellipsoid id = " + ellipsoid_id() +
      "   " + ellipsoid_caption() + "\n\n";

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

std::string data =
R"GHILANI_V1(# Example from Section 17.8
#
# Ghilani Charles D. (2010): Adjustment Computations. Spatial Data
# Analysis. Fifth Edition, John Wiley &amp; Sons, Inc.,
# ISBN 16 978-0-470-46491-5, Ch. 17.6, p 337-352

* A  fixed fixed    402.35087 -4652995.30109  4349760.77753      0 0 0  # trailing comment
* B  fixed fixed   8086.03178 -4642712.84739  4360439.08326      0 0 0
* C  free  free   12046.58076 -4649394.08256  4353160.06443      0 0 0
# D  free  free   -3081.58313 -4643107.36915  4359531.12333      0 0 0
* D  free  free  43-23-16.3401742 -90-02-16.8958335 894.01408    0  0  0
* E  free  free   -4919.33908 -4649361.21987  4352934.45480      0  0  0
* F  free  free    1518.80119 -4648399.14533  4354116.69141      0  0  0

      <<<< separator between points and observations

<obs>
  <vector> A C

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>988.4</flt> <flt>-9.58</flt> <flt>9.52</flt>
  <flt>937.7</flt> <flt>-9.52</flt>
  <flt>982.7</flt>
  </cov-mat>
</obs>

<obs>
  <vector> B C

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>230.5</flt> <flt>-2.23</flt> <flt>2.07</flt>
  <flt>254.6</flt> <flt>-2.23</flt>
  <flt>225.2</flt>
  </cov-mat>
</obs>

<obs>
  <vector> B D

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>270  </flt> <flt>-2.75</flt> <flt>2.85</flt>
  <flt>272.1</flt> <flt>-2.72</flt>
  <flt>267  </flt>
  </cov-mat>
</obs>

<obs>
  <vector> D C

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>146.1</flt> <flt>-1.43</flt> <flt>1.34</flt>
  <flt>161.4</flt> <flt>-1.44</flt>
  <flt>130.8</flt>
  </cov-mat>
</obs>

<obs>
  <vector> D E

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>123.1</flt> <flt>-1.19</flt> <flt>1.22</flt>
  <flt>127.7</flt> <flt>-1.21</flt>
  <flt>128.3</flt>
  </cov-mat>
</obs>

<obs>
  <vector> F A

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>74.75</flt> <flt>-0.79</flt> <flt>0.88</flt>
  <flt>65.93</flt> <flt>-0.81</flt>
  <flt>76.16</flt>
  </cov-mat>
</obs>

<obs>
  <vector> F C

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>256.7</flt> <flt>-2.25</flt> <flt>2.4</flt>
  <flt>216.3</flt> <flt>-2.27</flt>
  <flt>239.7</flt>
  </cov-mat>
</obs>

<obs>
  <vector> F E

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>94.42</flt> <flt>-0.92</flt> <flt>1.04</flt>
  <flt>99.59</flt> <flt>-0.89</flt>
  <flt>88.26</flt>
  </cov-mat>
</obs>

<obs>
  <vector> F D

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt> 93.3 </flt> <flt>-0.99</flt> <flt>0.90</flt>
  <flt> 98.75</flt> <flt>-0.99</flt>
  <flt>120.4 </flt>
  </cov-mat>
</obs>

<obs>
  <vector> F B

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>66.43</flt> <flt>-0.65</flt> <flt>0.69</flt>
  <flt>74.65</flt> <flt>-0.64</flt>
  <flt>60.48</flt>
  </cov-mat>
</obs>

<obs>
  <vector> B F

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>55.12</flt> <flt>-0.63</flt> <flt>0.61</flt>
  <flt>74.72</flt> <flt>-0.63</flt>
  <flt>66.29</flt>
  </cov-mat>
</obs>

<obs>
  <vector> A F

  <cov-mat> <dim>3</dim> <band>2</band>
  <flt>66.19</flt> <flt>-0.80</flt> <flt>0.90</flt>
  <flt>81.08</flt> <flt>-0.82</flt>
  <flt>93.76</flt>
  </cov-mat>
</obs>

)GHILANI_V1";

  return header + errors + data;
}

std::string GenG3::help() const
{
  return geng3test_help_md;
}

std::string GenG3::version() const
{
  return GNU_gama::sub_version("0.90");
}

void GenG3::error(std::string message)
{
  const int line_number_width {3};
  std::cerr << "line " << std::setw(line_number_width) << line_count
            << " : "  << current_line << std::endl
            << "    " << std::setw(line_number_width+3)
            << "error: " << message << std::endl;

  error_count++;
}
