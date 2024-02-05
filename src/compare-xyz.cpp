#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>

#include <gnu_gama/xml/comparexyz.h>

using namespace std;
using namespace GNU_gama;

auto help = R"HELP(
Usage: compare-xyz file1.xml file2.xml

where file1/file2 are GNU Gama adjustment XML results,
either in gama-local and/or gama-g3 format.

The program compares adjusted coordinates common to both input files.

Options:

   --help
   --version
   --tolerance            get tolerance for differences in coordinates
   --set-tolerance 4e-6   set the tolerance
)HELP";

std::set<string> help_opt {"-h", "--h", "-help", "--help"};


int main(int argc, char* argv[])
{
  CompareXYZ compare_xyz;

  vector<string> files;

  for (int i=1; i<argc; i++)
  {
    if (string(argv[i]) == string("--version"))
    {
      cout << compare_xyz.version() << endl;
      return 0;
    }

    if (string(argv[i]) == string("--tolerance"))
    {
      cout << setprecision(4) << scientific
           << compare_xyz.tol_max() << endl;
      return 0;
    }
    if (string(argv[i]) == string("--set-tolerance"))
    {
      i++;
      if (i == argc) {
         cout << help << endl;
         return 0;
      }
      try {
        size_t pos;
        double result = std::stod(string(argv[i]), &pos);
        compare_xyz.set_tol_max(result);
      }
      catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << std::endl;
        return 0;
      }
      catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        return 0;
      }

      continue;
    }

    if (help_opt.find(argv[i]) != help_opt.end())
    {
      cout << help << endl;
      return 0;
    }

    files.push_back( argv[i] );
  }

  if (files.size() != 2)
  {
    cout << help << endl;
    return 0;
  }


  compare_xyz.fetch_files(files[0], files[1]);
  compare_xyz.write_xml();

  return compare_xyz.passed() ? 0 : 1;
}
