#
# ln -s ../build/Desktop_Qt_6_8_2-Debug/tests/gama-g3/geng3test
# ln -s ../build/Desktop_Qt_6_8_2-Debug/gama-g3
#
./geng3test -e > 01-demo-a.txt
cp 01-demo-a.txt 01-demo-b.txt
head 01-demo-a.txt

sed -i  '/^\*\ D/  s/0\ \ 0\ \ 0/10\ \ 0\ \ 0/'  01-demo-b.txt
sed -i  '/^\*\ E/  s/0\ \ 0\ \ 0/0\ \ 20\ \ 0/'  01-demo-b.txt
sed -i  '/^\*\ F/  s/0\ \ 0\ \ 0/0\ \ 0\ \ 30/'  01-demo-b.txt

diff -su 01-demo-a.txt 01-demo-b.txt

./geng3test 01-demo-a.txt > 01-demo-a.xml
./geng3test 01-demo-b.txt > 01-demo-b.xml

# diff 01-demo-a.xml 01-demo-b.xml

./gama-g3  01-demo-a.xml  01-demo-A.txt
./gama-g3  01-demo-b.xml  01-demo-B.txt
