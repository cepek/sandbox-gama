/* matvec_test_003.cpp
   Copyright (C) 2000, 2012, 2018, 2024  Ales Cepek <cepek@gnu.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library (see COPYING.LIB); if not, write to the
   Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <iostream>
#include <iomanip>
#include <matvec/hilbert.h>

int main()
{
  using namespace std;
  using namespace GNU_gama;

  using Double = long double;
  using Long = unsigned long;

  cout << "\n   Hilbert matrix  ....   test_003  matvec "
       << GNU_gama::matvec_version() << "  inv()\n"
       << "------------------------------------------------------\n\n";

  cout << "          cond         inv(H)        inv(I) \n";

  cout.setf(ios::scientific, ios::floatfield);
  cout.precision(2);

  for (Long N=1; N<=15; N++)
    {
      cout << setw(2) << N;

      Mat<Double, Long> E(N,N);
      E.set_identity();

      Mat<Double, Long> H = Hilbert<Double, Long>(N);
      Mat<Double, Long> I = InvHilbert<Double, Long>(N);

      Double fh = 0, fi = 0;
      for (Long i=1; i<=N; i++)
        for (Long j=1; j<=N; j++)
          {
            fh += H(i,j)*H(i,j);  // Frobenius norm ||H|| = sqrt(fh)
            fi += I(i,j)*I(i,j);  //                ||I|| = sqrt(fi)
          }
      // estimation of the condition number is ||H|| \times ||I||
      cout << setw(14) << sqrt(fh*fi);


      try {
        cout << setw(14);
        Mat<Double, Long> T = Hilbert<Double, Long>(N);
        T.invert();               // possible exception
        T = H*T - E;

        Double m = 0;
        for (Long i=1; i<=N; i++)
          for (Long j=1; j<=N; j++)
            if (fabs(T(i,j)) > fabs(m))
              m = T(i,j);

        cout << m;
      }
      catch (Exception::matvec&) {
        cout << "failed";
      }

      try {
        cout << setw(14);
        Mat<Double, Long> T = InvHilbert<Double, Long>(N);
        T.invert();               // possible exception
        T = I*T - E;

        Double m = 0;
        for (Long i=1; i<=N; i++)
          for (Long j=1; j<=N; j++)
            if (fabs(T(i,j)) > fabs(m))
              m = T(i,j);

        cout << m;
      }
      catch (const Exception::matvec&) {
        cout << "failed";
      }

      cout << endl;
    }
}
