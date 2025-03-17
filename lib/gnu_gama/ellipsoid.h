/*
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2002, 2003, 2023  Ales Cepek <cepek@gnu.org>

    This file is part of the GNU Gama C++ library.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef GNU_gama_gnu_gama_local_ellipsoid_H_ELLIPSOID_H_
#define GNU_gama_gnu_gama_local_ellipsoid_H_ELLIPSOID_H_

namespace GNU_gama {

  class Ellipsoid {
  public:

    Ellipsoid(int ellipsoid_id=1);    // wgs84, see ellipsoids.h

    double a() const { return A;  }   // semi-major axis
    double b() const { return B;  }   // semi-minor axis
    double f() const { return ff; }   // flattening

    double M(double b) const;   // meridional radius of curvature
    double N(double b) const;   // prime vertical radius of curvature
    double W(double b) const;   // first geodetic function
    double V(double b) const;   // second geodetic function

    void blh2xyz(double, double, double, double&, double&, double&) const;
    void xyz2blh(double, double, double, double&, double&, double&) const;

    void set_ab (double pa, double pb) { set_abff1( pa, pb,  0,  0); }
    void set_af (double pa, double pf) { set_abff1( pa,  0, pf,  0); }
    void set_af1(double pa, double pf) { set_abff1( pa,  0,  0, pf); }

    int id {1};

  private:

    double F(double b) const;   // auxiliary function

    double A, B, ff, n, e2, e22;
    double Ime2, Ipe22, AIme2, AB;

    void set_abff1(double, double, double, double);
  };

}   // namespace GNU_gama

#endif
