# geng3test

Program `geng3test` generate a testing input files for the program
adjustment program `gama-g3` (adjustment in global coordinate system),
see

    https://www.gnu.org/software/gama/

`Geng3test` reads a textual file, defining parameters of the testing
task, with simple syntax. The input file is read by lines, the first
character of each line defines the following contents.

* comments start with `#`, empty lines are ignored,
* a line starting with `*` defines information about a point
* and description of an observation starts with `>`


## Elliopsoid


## Points

A point information line starts an asterisk (`*`) followed by the
point id and BLH ellipsoid coordinates in gradian (BL) and meters H.
Following is information on coordinates type for B,L and H (free,
constr and fixed, where constr is the abbreviation for
constrained). The three are differences for B and L in centesimal
second of arc (`cc`) and dH in millimeters.

* id   B L H  BL_type H_type  dB dL dH

[note] Angular data can be alternatively entered in sexadecimal format
dd-mm-ss for B, L, dB and dL
