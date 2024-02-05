#ifndef LIB_GNU_GAMA_XML_COMPAREXYZ_H
#define LIB_GNU_GAMA_XML_COMPAREXYZ_H

#include <string>
#include <map>
#include <iostream>


namespace GNU_gama {

class CompareXYZ
{
public:
  CompareXYZ() {}

  void fetch_files(std::string name_1, std::string name_2);
  void write_xml(std::ostream& ostr=std::cout);

  std::string version() const { return std::string("0.2"); }
  bool        passed()  const { return !failed_; }
  double      tol_max() const { return tol_max_; }

  void set_tol_max(double tol) { tol_max_ = tol; }

private:

  struct AdjXYZ {
    int dimension {3};
    double x, y, z;

    AdjXYZ() {}
    AdjXYZ(int dim, double p, double q, double r)
      : dimension(dim), x(p), y(q), z(r)
    {
    }
  };

  void fetch_file(std::string file_name, std::map<std::string, AdjXYZ>& adjmap);

  std::map<std::string, AdjXYZ> adjmap_1;
  std::map<std::string, AdjXYZ> adjmap_2;

  bool   failed_  { false };
  double tol_max_ { 1e-5 };
};

}

#endif // LIB_GNU_GAMA_XML_COMPAREXYZ_H
