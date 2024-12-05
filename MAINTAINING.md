<!--
HTML non-breaking space &nbsp; is used when three back ticks ```
occure at the beginning of a line, but the monospaced font applies only
to a single word/phrase, like ```cmake```, for example.
-->

<!--
  Code blocks are normally indented four spaces or one tab.
  When they’re in a list, indent them eight spaces or two tabs
-->


# GNU Gama

## Table of contents

- Release process
- Maintainer prerequisites
    - autotools
- Source Organization
- New release
    - Generate an announcement message
    - Builds on WSL
    - Distributing the new release
        - Gnulib gnupload
        - Gnulib announce-gen
    - Online documentation
    - GNU Gama Home Page
- Cmake - alternative build system
- Optional features
    - YAML
        - The primary build system (autotools)
        - Alternative build system (cmake)
- Doxygen
- Notes


## Release process

The logical flow of a release is a tag in git, and then "make dist" of
a checkout of that tag, with the resulting tarball made available.

Before tagging,

  - choose a version number
  - update NEWS and ChangeLog
  - update the version in the following three files
    - configure.ac
    - lib/gnu_gama/version.cpp
    - CMakeLists.txt

    Make an individual commit for the updated version, i.e. follow
    the general rule to make multiple commits for unrelated
    changes; namely if you replace a whole file, like an xml example,
    make it a single commit.

  - run `make distcheck' to ensure that the distfile includes all
    necessary files for building, and that the tests pass.

Apply the tag, and ask other active contributors to test.  They can
each run "make distcheck" on their platforms, and test the resulting
distfile as the source for packaging systems (but should NOT publish
the packaging changes).

If there are problems, fix with a commit and move the tag.  If there
are no problems, the distfile from make distcheck or make dist can be
published.

Once a gama-x.y.tar.gz is made available in the download area, it may
not be changed, because some packaging systems store hashes and a
maintainer-replaced tarball looks the same as an attacker changing the
distfile.  We have an infinite number of integers, so a new release
can always be made if there is a critical flaw.  It is OK to remove a
bad tarball after a replacement one has been published, but this is a
practice probably reserved for truly problematic situations.


## Maintainer prerequisites

### autotools

In addition to the prerequisites in README, when building gama from
the repository rather than a released tarball, one must additionally
have autoconf, automake and libtool.

## Source Organization

   * [src] the sources of all binaries of the package, namely
           gama-local, build binaries are stored also there.

   * [lib] C++ library of classes and functions used to buils
           gama-local and other helper applications.

   * [doc] documentation.

   * [tests] unit tests

   * [scripts] helper tests and applications

   * [xml] xml schema definitions, examples and language files.

## New release

* Update files ```NEWS``` and ```ChangeLog```

* Update release version in the following three files

        configure.ac
        lib/gnu_gama/version.cpp
        CMakeLists.txt

  Unit test ```tests/gama-local/gama-local-version``` checks that the
  release version is identical in all these files (run ```make check```).

* Run full set of unit tests with *extra tests* enabled

        ./autogen.sh
        ./configure  --enable-extra-tests
        make check

   Script ```autogen.sh``` usually only needs to be run for the first
   time after substantial changes in the project configuration.

* GNU Gama comes with an alternative ```cmake``` build system
  defined in CMakeLists.txt with a limited set of unit tests.

  This *unofficial* build is available for users who need to build
  Gama on Windows without the MinGW development enviroment, neither
  directly with Visual Studio or within Windows Subsystem for Linux
  (WSL).

  The unit test ```gama-local-version``` cannot check if all file
  names are properly defined in ```CMakeLists.txt``` and you need to
  check the *cmake build*, for example like this

        mkdir build      # temporary directory
        cd build
        cmake -S .. -B .
        make
        ctest

  or if you want to use the Ninja build system for a faster run

        cmake -S .. -B . -G Ninja
        ninja
        ctest

  The following steps work on Windows as well as Linux:

        mkdir build
        cd build
        cmake -S .. -B .
        cmake --build . --config Release
        ctest -C Release

* Now we need to check if all the files have been committed to the
  Gama git server. Create a *working/temporary* directory, clone the
  project and repeat all the steps above

        git clone https://git.savannah.gnu.org/git/gama.git temp-gama
        cd temp-gama

* If everything passed, you can generate the new distribution tarball

        ./autogen.sh
        ./configure
        make distcheck

* Now when you have the release tarball and all other release file ready
  (for example ```gama-2.29.tar.gz``` and ```announce-2.29.txt```),
  you commit all changes and tag the git repository, for example

        git tag gama-2.29

  Push the tag to the repository (set the origin repo to ssh first if
  needed):

        # git remote set-url origin ssh://user@git.sv.gnu.org/srv/git/gama.git
        git push origin <tag_name>

   where *user* is your login name.

* You also have to send an email about the new release to
  lists&nbsp;```info-gama@gnu.org``` and ```info-gnu@gnu.org```,
  describing what is new in the release and what bugs were fixed (if
  any). Here is an example text sent for version 2.29

        Subject: GNU gama 2.29 released

        We are pleased to announce the release of GNU Gama 2.29.

        There is one major fix visible to end users. Implicit value of XML
        parameter 'update_constrained_coordinates' was changed to "yes". The
        behavior of adjustment calculation was well and clearly described in
        the documentation, but in some cases the old implicit value ("no")
        might had lead to poor numerical results. The input data XML parameter
        'update_constrained_coordinates' is preserved only for backward
        compatibility and is likely to be removed in some future release.

        Several other minor internal issues were fixed, none of them visible to
        end users.

        About

        GNU Gama package is dedicated to adjustment of geodetic networks. It
        is intended for use with traditional geodetic surveyings which are
        still used and needed in special measurements (e.g., underground or
        high precision engineering measurements) where the Global Positioning
        System (GPS) cannot be used.

        Adjustment in local coordinate systems is fully supported by a
        command-line program gama-local that adjusts geodetic (free) networks
        of observed distances, directions, angles, height differences, 3D
        vectors and observed coordinates (coordinates with given
        variance-covariance matrix). Adjustment in global coordinate systems
        is supported only partly as a gama-g3 program.

        https://www.gnu.org/software/gama/

### Generate an announcement message

The announce message can and should be generated
with&nbsp;```announce-gen``` script from directory ```build-aux``` of
GNU Gnulib https://www.gnu.org/software/gnulib/manual/gnulib.html

    ./announce-gen --help
    Usage: announce-gen [OPTIONS]
    Generate an announcement message.  Run this from builddir.

    OPTIONS:

    These options must be specified:

    --release-type=TYPE          TYPE must be one of alpha beta stable
    --package-name=PACKAGE_NAME
    --previous-version=VER
    --current-version=VER
    --gpg-key-id=ID         The GnuPG ID of the key used to sign the tarballs
    --url-directory=URL_DIR

    The following are optional:

    --news=NEWS_FILE             include the NEWS section about this release
                                 from this NEWS_FILE; accumulates.
    ......

   For example:

    ./announce-gen --package-name gama \
       --release-type stable \
       --current-version 2.28 \
       --previous-version 2.27 \
       --gpg-key-id 1b77fc09 \
       --url https://ftp.gnu.org/gnu/gama \
       --news NEWS

   Including the ```NEWS``` file (the NEWS section about this release)
is highly recommended, ```NEWS``` are generally more interesting to
users rather than ChangeLog, which is of interest mainly to developpers.

### Builds on WSL

This section is given with no guarantee, GNU Gama is not tested on
Windows Subsystem Linux. It seems that on WSL you need to install

    sudo apt install automake
    sudo apt install build-essential

and it seems that you also need to explicitly enable build with local copy
of expat parser

    ./autogen
    ./configure --enable-expat_1_1  --enable-extra-tests

    make check    # build all and run all tests including the extra tests

### Distributing the new release
#### Gnulib gnupload

For uploading new release tarball to the GNU FTP server, use
script&nbsp;```gnupload``` from directory ```build-aux``` from *GNU
portability library* (gnulib, see
https://savannah.gnu.org/git/?group=gnulib)

    git clone git://git.sv.gnu.org/gnulib.git
    ln -s gnulib/build-aux/gnupload     # create a symlink or copy ...

To upload your new release tarball, simply run

    ./gnupload --to ftp.gnu.org:gama gama-2.29.tar.gz

You will be asked for your gpg passphrase (twice) and you will get
an email when your upload is finished.

**Warning:** Never replace tarball already uploaded to the GNU
FTP server, a post-release distfile modification makes the release
unusable for packaging systems.


**Note:** When uploading ```qgama``` new release tarball use

    ./gnupload --to ftp.gnu.org:gama/qgama qgama-2.07.tar.gz

<!--http://git.savannah.gnu.org/cgit/gama/qt.git download file
    qt-qgama-2.07.tar.gz is renamed to qgama-2.07.tar.gz
-->

or

    ./gnupload --to ftp.gnu.org:gama/windows qgama-2.07.zip

when uploading Windows ```qgama``` zip executable binaries
generated by windeployqt.exe (see deploy-win64-qgama.ps1 script).


<!-- !!! NEWER DO THIS !!!
Similarly you can replace the tarball, in case it is needed

      ./gnupload --replace --to ftp.gnu.org:gama gama-2.29.tar.gz
-->

Use ```gnupload --help``` for the full list of options with examples, you
can always try running ```gnupload``` with the option ```--dry-run```

* Note: If you encounter this error when running gnulib

    gpg: signing failed: Inappropriate ioctl for device

Simply run in the command line:

    export GPG_TTY=$(tty)

#### Gnulib announce-gen

You may find the announce-gen script useful for creating
announcements, which is available from the build-aux/ directory of the
gnulib project at https://savannah.gnu.org/projects/gnulib.

Suppose we have NEWS file, current and previous gama tarballs in a
directory; run announce-gen script to generate new release
announcement

    ../gnulib/build-aux/announce-gen \
        --package-name=gama \
        --release-type=stable \
        --current-version=$1 \
        --previous-version=$2 \
        --gpg-key-id 1b77fc09 \
        --url https://ftp.gnu.org/gnu/gama \
        --news NEWS \
          2> announce-$1.err | tee announce-$1.txt

<!--
**_NOTE:_** announce-gen version 2023-12-29 produced following warnings/errors

      fatal: ambiguous argument 'v2.28..v2.29': unknown revision or path not in the working tree.

      Use '--' to separate paths from revisions, like this:
      'git <command> [<revision>...] -- [<file>...]'
      fatal: ambiguous argument 'v2.28..v2.29': unknown revision or path not in the working tree.
      Use '--' to separate paths from revisions, like this:
      'git <command> [<revision>...] -- [<file>...]'
      fatal: ambiguous argument 'v2.28': unknown revision or path not in the working tree.
      Use '--' to separate paths from revisions, like this:
      'git <command> [<revision>...] -- [<file>...]'
      fatal: ambiguous argument 'v2.29': unknown revision or path not in the working tree.
      Use '--' to separate paths from revisions, like this:
      'git <command> [<revision>...] -- [<file>...]'
      announce-gen: Argument "" isn't numeric in subtraction (-) at ../gnulib/build-aux/announce-gen line 555.
      announce-gen: Argument "" isn't numeric in subtraction (-) at ../gnulib/build-aux/announce-gen line 555.
      fatal: ambiguous argument 'v2.28..v2.29': unknown revision or path not in the working tree.
      Use '--' to separate paths from revisions, like this:
      'git <command> [<revision>...] -- [<file>...]'
-->

### Online documentation

To update Gama webpages and online documentation, you need to check
out its repository from CVS (Concurrent Version System):

    cvs -z3 -d:ext:user@cvs.savannah.gnu.org:/web/gama co -P gama
    mv gama gama-web    # rename the local web directory


Here, replace ```user``` with your login name, and ```gama ```is the
directory for web pages to be checked out. The option ```-P``` is used
to prune empty directories. Please note that this CVS repository is
not to be confused with the Git repository of the same name. After
checking out, the local web directory is renamed to ```web-gama```.

* For generating documentation in various formats from texinfo sources,
  download and run ```gendocs``` (see ```doc/Makefile.am``` for details)

        cd gama/doc                  # in gama git/ftp repository
        make download-gendocs.sh
        make run-gendocs.sh

* Copy the ```manual``` directory generated by the gendocs script from gama
  repository to CVS ```web-gama```

* Run script ```./update-manual.sh``` from CVS repositorsy ```web-gama```

        #!/bin/sh

        find . -type f -name "*.html" -print0 | xargs -0 cvs add
        find . -type f -name "*.gz"   -print0 | xargs -0 cvs add
        find . -type f -name "*.pdf"  -print0 | xargs -0 cvs add
        find . -type f -name "*.txt"  -print0 | xargs -0 cvs add

        for m in html_chapter html_node html_section
        do
            echo
            echo manual/$m/.symlinks :
            cat manual/$m/.symlinks
        done

* add ```gama.html index.html``` to ```manual/html_*/.symlinks```
  (if not there already)

* remove all files index.html from manual ```find manual -name index.html```

  Run

        cvs commit .   # only cvs is available for GNU web pages, no git


### GNU Gama Home Page

To updade GNU Gama [Home Page](https://www.gnu.org/software/gama/) you
need to download web pages from CVS as described above

    cvs -z3 -d:ext:user@cvs.savannah.gnu.org:/web/gama co -P gama

edit the file ```gama/gama.html``` and commit your changes

    cvs commit .


## Cmake - alternative build system

Cmake is an alternative build system to ```GNU autotools```. To build
GNU Gama release with ```cmake``` run

    cmake -S . -B    build -G Ninja
    cmake --build    build --config Release
    ctest --test-dir build

For more information on ```cmake``` consult the
[documentation](https://cmake.org/).

## Optional features

<!-- YAML section was copied from deleted file lib/README.yaml-cpp -->
### YAML
#### The primary build system (autotools)

To build GNU Gama with YAML support you need to have library ```yaml-cpp```
installed (on Debian based systems package ```libyaml-cpp-dev```).

#### Alternative build system (cmake)

Alternatively with cmake build you can compile gama with source codes
of yaml-cpp C++ parser included. To do this you must clone
the&nbsp;```yaml-cpp project``` from the git repository

    cd "GNU Gama sources"/lib
    git clone https://github.com/jbeder/yaml-cpp


### Doxygen

Doxygen generator for source codes documentation is available for Gama
project, but it is not consistantly used for  all classes and
functions. The Doxygen main page is defined in the file ```gama.dox```
and the the configuration in ```Doxygen``` file.

To generate the Doxygen documentation run

    doxygen

from the top project directory, ```doxygen``` creates directory
```dox``` with two subdirectories ```html``` and ```latex``` with
corresponding output format. For html open the file
```html/index.html```. For latex change to ```latex``` subdirectory
and run

    make [ ps | pdf ]

to build ```refman.dvi``` (implicit format),
```refman.ps``` or ```refman.pdf``` format.


### Notes

#### Languages

The program ```gama-local``` supports output in various languages. The
language is specified using the ```--language en``` option (the
default is English). Language codes follow the two-letter ISO 639
standard. Translations are defined in the following files:

    lib/gnu_gama/local/language.[h|cpp]

These files are generated by the program ```scripts/slovnikar.cpp```,
which contains all the essential information in its comments. The best
way to add a new language is by running the script:

    scripts/build-dictionaries.sh
