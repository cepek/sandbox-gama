# Invoking gendocs.sh

The script gendocs.sh eases the task of generating the Texinfo documentation
output for your web pages. It has a companion template file, 
used as the basis for the HTML index pages. Both are available 
from the Gnulib development: 

    https://git.savannah.gnu.org/cgit/gnulib.git/tree/build-aux/gendocs.sh
    https://git.savannah.gnu.org/cgit/gnulib.git/tree/doc/gendocs_template


## 1

    cd gama/doc
    make download-gendocs.sh
    make run-gendocs.sh

You can always get the latest version directly from the Gnulib source tree.
If the script’s location changed in your environment, or if a fork removed it,
switching to the official Gnulib repository ensures you have the most current
and maintained version.

    git clone https://git.savannah.gnu.org/git/gnulib.git

| Item             | Location on Savannah Git |
| ---------------- | ------------------------ |
| gendocs.sh	     | build-aux/gendocs.sh     |
| Full template	   | doc/gendocs_template     |
| Minimal template | doc/gendocs_template_min |

## 2  

copy manual to gama-web

    find . -type f -print0| xargs -0 cvs add

### working alternative ###

    cd manual
    cvs add `find -type f | grep -v CVS`

## 3 

copy and add .symlinks where needed (manual, manual/html_node, ...)
example:

    echo ../fig fig > html_chapter/.symlinks

## 4

* add "gama.html index.html"  in manual/html_*/.symlinks
* remove all files index.html in manual/html_*/.symlinks

## 5 
    cd web-gama
    cvs commit .
