#ifndef GENG3TEST_H
#define GENG3TEST_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <gnu_gama/ellipsoids.h>

// #define GenG3_DEBUG

class GenG3 {

public:
  GenG3(GNU_gama::Ellipsoid e=GNU_gama::ellipsoid_wgs84);

  std::string ellipsoid_caption() const;
  std::string ellipsoid_id() const;
  std::string xml_header() const;
  std::string xml_end() const;

  std::string xml_points() const;
  std::string xml_observations();

  std::istream& read(std::istream&);
  std::istream& read_obs(std::istream&);
  void write(std::ostream&);

  std::string example() const;
  std::string help() const;

  static int errors() { return GenG3::error_count; }

private:

    std::string current_line;
    int line_count;
    static int error_count;
    void error(std::string message);


  struct geng3point {
    std::string id;                   // point ID
    double B{0}, L{0}, H{0};          // ellipsoidal coordinates
    double X{0}, Y{0}, Z{0};          // ... corresponding XYZ

    double dB{0}, dL{0}, dH{0};       // simulated BLH coordinate errors ss, ss, mm

    double errB{0}, errL{0}, errH{0}; // simulated incorrect coordinates
    double errX{0}, errY{0}, errZ{0};

    /*enum Status {
      fixed, free, constr             // "constrained"
    }*/ std::string BL_status{"fixed"}, H_status{"fixed"};

    std::string line;                 // reference to original input data
    int line_number {0};
  };

  std::map<std::string, geng3point> points;
  std::ostringstream ostrobs;

  using Tokens = std::vector<std::vector<std::string>>;
  Tokens tokens;

  GNU_gama::Ellipsoid ellipsoid;

  std::string text() { return ""; }

  enum point_status_BLH {
    BLH_undefined = 0,
    BLH_fixed_fixed, BLH_fixed_free,   BLH_fixed_constr,
    BLH_free_free,   BLH_free_fixed,   BLH_free_constr,
    BLH_constr_free, BLH_constr_fixed, BLH_constr_constr
  };
};

#endif
