#ifndef GENG3TEST_H
#define GENG3TEST_H

#include <iostream>
#include <string>
#include <vector>
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

  std::string example() const;
  std::string help() const;

private:

  struct gend3point {
    std::string id;               // point ID
    double B{0}, L{0}, H{0};      // ellipsoidal coordinates
    double X{0}, Y{0}, Z{0};      // ... corresponding XZY
    double dB{0}, dL{0}, dH{0};   // simulated coordinate errors
    enum Status {
      fixed, free, constr         // "constrained"
    } BL_status{fixed}, H_status{fixed};
  };

  using Tokens = std::vector<std::vector<std::string>>;
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

#endif
