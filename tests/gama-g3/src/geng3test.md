# geng3test

Program `geng3test` generates testing input files for the geodetic 
adjustment program `gama-g3`, which performs network adjustment 
in a global coordinate system. See:

   https://www.gnu.org/software/gama/

`geng3test` reads a textual file that defines the parameters of the testing task
using a simple syntax. The input file is read line by line, where the first 
character of each line determines the content:

* Lines starting with # are comments; empty lines are ignored.
* Lines starting with * define point information.
* Lines starting with > describe observations.


## Elliopsoid


## Points

A point information line starts with an asterisk (*), followed by the point ID 
and ellipsoidal coordinates (B, L, and H), where B (latitude) and L (longitude) 
are given in gradians, and H (height) is given in meters. The line also specifies
the coordinate status for B, L, and H—whether they are free, constrained (constr), 
or fixed. Additionally, the line includes differences for B and L in 
centesimal seconds of arc (cc) and dH in millimeters.

### Format

    *  id   B L H   dB dL dH   BL_status H_status
    *  id   X Y Z   dB dL dH   BL_status H_status

Coordinates B, L, H must be given in sexagesimal format 
degrees-minutes-seconds (for example 56-21-38.74). Values dB and dL are
always given in seconds of arc, dH is given in millimeters. 

BL_status and H_status must be fixed, free or constr ("constrained").

[note] If point information is too long to fit in a single line, 
it may contine on the following lines (to form all 10 tokens).


### Internal Representation

    struct gend3point {
      std::string id;        // Point ID
      double B, L, H;        // Ellipsoidal coordinates
      double X, Y, Z;        // Corresponding XYZ coordinates
      double dB, dL, dH;     // Simulated coordinate errors
      enum Status {
        fixed, free, constr  // "constrained"
      } BL_status, H_status;
    };
    
Missing triple XYZ or BLH is calculated internally.




