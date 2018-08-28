dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Astaire, 1.0, https://github.com/Augugrumi/Astaire/issues)
AC_LANG(C++)
AC_CANONICAL_SYSTEM
AC_PROG_CXX
AM_INIT_AUTOMAKE()
AC_CONFIG_FILES(Makefile src/Makefile)
AC_CONFIG_MACRO_DIR([m4])

AX_CXX_COMPILE_STDCXX(11, [noext], [mandatory])

#AC_CONFIG_HEADERS([config.h])

dnl Checking libviface installation
AC_CHECK_LIB(viface, send, [VIFACE_LIBS+=-lviface], [AC_MSG_ERROR([send was not found in libviface])])

AC_CHECK_LIB(pthread, pthread_create, [PTHREAD_LIBS+=-lpthread], [AC_MSG_ERROR([pthread_create was not found in libpthread])])

AC_CHECK_LIB(tins, main, [TINS_LIBS+=-ltins], [AC_MSG_ERROR([main was not found in libtins])])

LDFLAGS="$LDFLAGS $VIFACE_LIBS $PTHREAD_LIBS $TINS_LIBS"

dnl Checking boost dependencies
AX_BOOST_BASE([1.58],, [AC_MSG_ERROR([astaire needs Boost, but it was not found in your system])])
AX_BOOST_THREAD
AX_BOOST_LOG

AC_OUTPUT

