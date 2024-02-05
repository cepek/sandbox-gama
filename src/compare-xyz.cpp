#include <iostream>
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
   --toldiff 4e-6   tolerance for differences in coordinates
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
