EXTRA_DIST = gama-g3-adjustment.in gama-g3-xmllint-xsd.in \
             gama-g3-ellipsoid-xyz2blh.in \
             gama-g3-ellipsoid-xyz2blh-list.in

check_PROGRAMS = check_adjustment \
                 check_ellipsoid_xyz2blh \
                 check_ellipsoid_xyz2blh_list

check_adjustment_SOURCES  = check_adjustment.cpp
check_adjustment_LDADD    = $(top_builddir)/lib/libgama.a
check_adjustment_CPPFLAGS = -I $(top_srcdir)/lib

check_ellipsoid_xyz2blh_SOURCES  = check_ellipsoid_xyz2blh.cpp
check_ellipsoid_xyz2blh_LDADD    = $(top_builddir)/lib/libgama.a
check_ellipsoid_xyz2blh_CPPFLAGS = -I $(top_srcdir)/lib

check_ellipsoid_xyz2blh_list_SOURCES  = check_ellipsoid_xyz2blh_list.cpp
check_ellipsoid_xyz2blh_list_LDADD    = $(top_builddir)/lib/libgama.a
check_ellipsoid_xyz2blh_list_CPPFLAGS = -I $(top_srcdir)/lib
