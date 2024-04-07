# gen-test v. 3.00

Program `gen-test` generates random XML input network data for testing
of 3D local geodetic network adjustment (GNU Gama). The purpose of the
program is not to generate realistic simulations but crash test data
and verification of from_dh/to_dh corrections and `gama-local --export`.

`gen-test` can be run without paramaters or with one or two output files.

```
$ ./gen-test --help

gen-test [ file1 | - [ file2 | - ]]

$
```

When run without parameters `gen-test` writes random generated input data for `gama-local`
to the standard output device

```./gen-test
<?xml version='1.0' ?>
<gama-local xmlns='http://www.gnu.org/software/gama/gama-local'>
<network>

<description>
gen-test 3.00
</description>

<points-observations>

<point id='0' x='799.052380' y='850.909306' z='337.261661' adj='z'   />
<point id='1' x='167.654893' y='305.839843' z='251.237979' adj='xyz' />
   ...
```

When run with one parameter (output file name), gen-test writes the
generated XML directly to the given name.

When run with two parameters `gen-test` writes the generated XML to the first file
(similarly as in the previous case) and the equivalent XML data with reduced all
`from_dh` and `to_dh` to zero to the second file. These random data can be used for
verification of from_dh/to_dh corrections used in `gama-local`.

Insted of file names you can use symbol `-` (minus sign) to denote standard output.
