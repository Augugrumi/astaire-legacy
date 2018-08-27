dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Astaire, 1.0, https://github.com/Augugrumi/Astaire/issues)
AC_LANG(C++)
AC_CANONICAL_SYSTEM
AC_PROG_CXX
AM_INIT_AUTOMAKE()
AC_CONFIG_FILES(Makefile src/Makefile)

LDFLAGS="$LDFLAGS -lviface"
AC_LINK_IFELSE(
  [
    AC_LANG_PROGRAM(
      [#include <viface/viface.hpp>],
      [viface::VIface iface("test")])
  ],
  [] [],
  [AC_MSG_ERROR([libviface is not installed.])])

AC_OUTPUT

