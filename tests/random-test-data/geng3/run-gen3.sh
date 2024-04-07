#!/bin/bash

DAT=`date -I`
rm -rf $DAT
mkdir  $DAT

ALG=svd
GEN=$DAT/gen3

ulimit -t 3

# rm -f ttt* test-* gen3-* tst3-*

if [ ! -f ./gama-local ]; then
    echo "Symlink ./gama-local does not exists ..."
    exit 1
fi
if [ ! -f ./check_xml_xml ]; then
    echo "Symlink ./check_xml_xml does not exists ..."
    exit 1
fi

while (true)
do
    let i=i+1
    I=$( printf '%03d' $i )
    echo
    ./gen-test $GEN-$I-a.gkf $GEN-$I-b.gkf

    ./gama-local $GEN-$I-a.gkf --xml $GEN-$I-a.xml --text $GEN-$I-a.txt \
		 --export $GEN-$I-c.gkf --algorithm $ALG ;
    A=$?

    ./gama-local $GEN-$I-b.gkf --xml $GEN-$I-b.xml --text $GEN-$I-b.txt \
	 --algorithm gso
    B=$?

    ./check_xml_xml "xml diff $GEN-$I-a/b.xml ... exact corr. b" \
		       $GEN-$I-a.xml $GEN-$I-b.xml
    D=$?

    ./gama-local $GEN-$I-c.gkf --xml $GEN-$I-c.xml --text $GEN-$I-c.txt \
		    --algorithm $ALG
    C=$?

    ./check_xml_xml "xml diff $GEN-$I-b/c.xml ... export from a" \
		       $GEN-$I-b.xml $GEN-$I-c.xml
    E=$?

    if [ $A -ne 0 ] || \
       [ $B -ne 0 ] || \
       [ $C -ne 0 ] || \
       [ $D -ne 0 ] || \
       [ $E -ne 0 ]
    then
	echo
	echo gen-test a.gkf b.gkf
	echo
	echo A : gama-local a.gkf --xml a.xml --text a.txt \
	     --export c.gkf --algorithm $ALG
	echo B : gama-local b.gkf --xml b.xml --text b.txt --algorithm gso
	echo C : gama-local c.gkf --xml c.xml --text c.txt --algorithm $ALG
	echo D : check_xml_xml "diff a/b exact correction" a.xml b.xml
	echo E : check_xml_xml "diff b/c export from a   " b.xml c.xml
	echo
	echo tests A B C D E : $A $B $C $D $E
    	exit 1
    fi

    if [ $i -ge 1000 ] ;
    then
        echo
        exit 0
    fi
done
