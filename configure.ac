#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([jhako], [0.2.0], [info@jhako.com])
AC_CONFIG_SRCDIR([src/jhako_server/server.c])
AC_CONFIG_HEADERS([include/config.h])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([disable])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O
AC_PROG_RANLIB

APR_FIND_APR([], [], 1, 1)
APR_SETIFNULL(CC, `$apr_config --cc`)
APR_SETIFNULL(CPP, `$apr_config --cpp`)
APR_ADDTO(CPPFLAGS, `$apr_config --cppflags --includes`)
APR_ADDTO(LDFLAGS, `$apr_config --ldflags`)
APR_ADDTO(LIBS, `$apr_config --libs --link-ld`)

# Checks for libraries.
AC_CHECK_LIB(m, main)
AC_CHECK_LIB(confuse, main)
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(dbi, main)
AC_CHECK_LIB(ssh2, main)
AC_CHECK_LIB(csv, main)
AC_CHECK_LIB(xml2, main)
AC_CHECK_LIB(curl, main)
AC_CHECK_LIB(wsman, main)
AC_CHECK_LIB(wsman_client, main)
AC_CHECK_LIB(wsman_curl_client_transport, main)

# Checks for header files.
PKG_CHECK_MODULES(OPENWSMAN, [openwsman >= 2.2.3])
APR_ADDTO(CFLAGS, ${OPENWSMAN_CFLAGS})

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
    Makefile
    conf/Makefile
    src/Makefile
    src/libs/Makefile
    src/libs/basic/Makefile
    src/libs/common/Makefile
    src/libs/network/Makefile
    src/libs/models/Makefile
    src/jhako_server/Makefile
    src/jhako_server/expander/Makefile
    src/jhako_server/loader/Makefile
    src/jhako_server/bootstrap/Makefile
    src/jhako_server/executive/Makefile
    src/jhako_server/monitor/Makefile
    src/jhako_server/housekeeper/Makefile
])
AC_OUTPUT

echo
echo 
echo "Configuration:"
echo
echo "  Detected OS:           ${host_os}"
echo "  Install path:          ${prefix}"
echo "  Compilation arch:      ${ARCH}"
echo
echo "  Compiler:              ${CC}"
echo "  Compiler flags:        ${CFLAGS}"
echo
echo "  Linker flags:          ${LDFLAGS}"
echo "  Libraries:             ${LIBS}"

echo
echo "***********************************************************"
echo "*            Now run '${am_make} install'                       *"
echo "*                                                         *"
echo "*            Thank you for using jHako!                   *"
echo "***********************************************************"
echo
