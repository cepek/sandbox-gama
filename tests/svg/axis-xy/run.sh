#!/bin/sh

GAMA=../../../src/gama-local
VER=2.26


for ANG in left right
do
for XY in en es ne nw se sw wn ws
do
    echo ${ANG} ${XY} ${VER}
    ${GAMA} gama-local-${ANG}-${XY}.gkf \
            --text /dev/null  \
            --svg  gama-local-${ANG}-${XY}-${VER}.svg
done
done
