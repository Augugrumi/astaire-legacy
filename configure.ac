dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Astaire, 1.0)
AC_LANG(C++)

SAVED_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS -lviface"
AC_LINK_IFELSE(
  [
    AC_LANG_PROGRAM(
      [#include <viface/viface.hpp>],
      [viface::VIface iface("test")])
  ],
  [] [],
  [AC_MSG_ERROR([libviface is not installed.])])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

