AC_INIT([feias], [0.1], [tomas@halman.net])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
PKG_CHECK_MODULES([LIBCURL], [libcurl >= 7.0], [], [AC_MSG_ERROR([cannot find libcurl >= 7])])
AC_CHECK_HEADERS(libmilter/mfapi.h,,[
    AC_MSG_ERROR([Please install mfapi.h from the sendmail distribution])
])

my_CXXFLAGS="$my_CXXFLAGS \
-std=c++11 \
-Wall"

AC_SUBST([my_CXXFLAGS])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
