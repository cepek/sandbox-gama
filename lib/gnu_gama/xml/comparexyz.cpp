#include <cmath>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include "comparexyz.h"
#include <gnu_gama/xml/localnetwork_adjustment_results.h>  // parser gama-local
#include <gnu_gama/xml/dataparser.h>                       // parser gama-g3

using namespace std;
using namespace GNU_gama;


void CompareXYZ::fetch_files(std::string file_1, std::string file_2)
{
  fetch_file(file_1, adjmap_1);
  fetch_file(file_2, adjmap_2);
}


void CompareXYZ::fetch_file(string file_name, std::map<std::string, AdjXYZ>& adjmap)
{
  ifstream inp(file_name);
  std::string str_file((std::istreambuf_iterator<char>(inp)),
                       std::istreambuf_iterator<char>());

  bool format_local = (str_file.find("<gama-local-adjustment") != std::string::npos);
  bool format_g3 = (str_file.find("<g3-adjustment-results>") != std::string::npos);

  if (!format_local && !format_g3)
  {
    // ERROR
  }

  if (format_local)
  {
    //cout << "# gama-local: " << file_name << "\n";
    using GNU_gama::LocalNetworkAdjustmentResults;
    unique_ptr<LocalNetworkAdjustmentResults> adjres( new LocalNetworkAdjustmentResults );
    istringstream istr(str_file);
    adjres->read_xml(istr);

    for (const auto& point : adjres->adjusted_points)
    {
      cout.precision(14);
      if (point.hxy && point.hz)
      {
        adjmap[point.id] = AdjXYZ(3, point.x, point.y, point.z);
      }
    }
  }
  else if (format_g3)
  {
    std::list<GNU_gama::DataObject::Base*> objects;

    GNU_gama::DataParser parser(objects);
    try {
      parser.xml_parse(str_file.c_str(), static_cast<int>(str_file.length()), 0);
      parser.xml_parse("", 0, 1);
    }
    catch(const GNU_gama::Exception::parser& p)
    {
      cerr << "\nXML parser error on line " << p.line
           << " of input data  "
           << "\t(error code " << p.error_code << ")\n"
           << p.str << "\n\n";
      return ; // ***** 1;
    }
    catch(...)
    {
      cerr << "catch ... \n";
      return ; // ***** 1;
    }

    for (const auto& obj : objects)
    {
      if (auto adjptr = dynamic_cast<GNU_gama::DataObject::g3_adj_results*>(obj))
      {
        for (const auto& p : adjptr->adjres->points)
        {
          if (!p.x_adjusted.empty())
            adjmap[p.id] = AdjXYZ(3, stod(p.x_adjusted),
                                     stod(p.y_adjusted),
                                     stod(p.z_adjusted));
        }
      }
    }
  }
}


void CompareXYZ::write_xml(std::ostream& ostr)
{
  int maxid = 0;
  for (const auto& a : adjmap_1)
  {
    auto p = adjmap_2.find(a.first);
    if (p == adjmap_2.end()) continue;

    if (maxid < a.first.length()) maxid = a.first.length();
  }

  int id_length=1;
  double DX {0}, DY {0}, DZ {0};
  cout << std::fixed;
  for (const auto& a : adjmap_1)
  {
    auto p = adjmap_2.find(a.first);
    if (p == adjmap_2.end()) continue;

    const auto& data1 = a.second;
    std::string id = a.first;
    while (id.size() < id_length) id.push_back(' ');
    std::cout << id << " " << data1.dimension << " "
              << setw(17+9) << data1.x << " "
              << setw(17+9) << data1.y << " "
              << setw(17+9) << data1.z << "\n";

    double dx = p->second.x - data1.x;
    double dy = p->second.y - data1.y;
    double dz = p->second.z - data1.z;

    if (fabs(dx) > fabs(DX)) DX = dx;
    if (fabs(dy) > fabs(DY)) DY = dy;
    if (fabs(dz) > fabs(DZ)) DZ = dz;

    for (int i=1; i<=id_length+3; i++) cout << " ";
    std::cout << setw(17+9) << dx << " "
              << setw(17+9) << dy << " "
              << setw(17+9) << dz << "\n";
  }

  std::string txt_max = "max ";
  cout << "\nmax ";
  for (int i=1; i<=id_length+3-4; i++) cout << " ";
  std::cout << setw(17+9) << DX << " "
            << setw(17+9) << DY << " "
            << setw(17+9) << DZ << "\n";
}


