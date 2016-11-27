AC_INIT([feias], [0.1], [tomas@halman.net])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
PKG_CHECK_MODULES([LIBCURL], [libcurl >= 7.0], [], [AC_MSG_ERROR([cannot find libcurl >= 7])])
AC_CHECK_HEADERS(libmilter/mfapi.h,,[
    AC_MSG_ERROR([Please install mfapi.h from the sendmail distribution])
])

AC_SUBST(systemd_unit_dir)
AC_ARG_WITH(
    systemd-unit-dir,
    AS_HELP_STRING(
	    [--with-systemdsystemunitdir=DIR],
	    [systemd unit directory @<:@SYSTEMD_UNIT_DIR@:>@
        Defaults to the correct value for debian /etc/systemd/system/]
    ),
    [
	    systemd_unit_dir="$withval"
    ],
    [
        # default to the systemd admin unit directory
        which pkg-config
        pkg_config_exists=$?
        if test x"$pkg_config_exists" = x"0"; then
            systemd_unit_dir=`pkg-config systemd --variable=systemdsystemunitdir`
        else
            systemd_unit_dir="/etc/systemd/system/"
        fi
    ]
)
my_CXXFLAGS="$my_CXXFLAGS \
-std=c++11 \
-Wall"

AC_SUBST([my_CXXFLAGS])
AC_CONFIG_FILES([Makefile misc/ferias.service])
AC_OUTPUT
