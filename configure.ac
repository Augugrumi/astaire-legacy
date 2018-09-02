dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Astaire, 1.0, https://github.com/Augugrumi/Astaire/issues)
AC_LANG(C++)
AC_CANONICAL_SYSTEM
AC_PROG_CXX
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_FILES(Makefile src/Makefile)
AC_CONFIG_MACRO_DIR([m4])

AX_CXX_COMPILE_STDCXX(11, [noext], [mandatory])

AC_CONFIG_HEADERS([src/config.h])

dnl Checking libviface installation
AC_CHECK_LIB(viface, send, [astaire_LIBS+=" -lviface"], [AC_MSG_ERROR([send was not found in libviface])])

AC_CHECK_LIB(pthread, pthread_create, [astaire_LIBS+=" -lpthread"], [AC_MSG_ERROR([pthread_create was not found in libpthread])])

AC_CHECK_LIB(tins, main, [astaire_LIBS+=" -ltins"], [AC_MSG_ERROR([main was not found in libtins])])

AC_CHECK_LIB(pcap, pcap_loop, [astaire_LIBS+=" -lpcap"], [AC_MSG_ERROR([pcap_loop was not found in libpcap])])

AC_CHECK_LIB(jsoncpp, main, [astaire_LIBS+=" -ljsoncpp"], [AC_MSG_ERROR("main was not found in libjsoncpp")])


dnl Checking boost dependencies
AX_BOOST_BASE([1.58],[astaire_ADDITIONALFLAGS="-DBOOST_LOG_DYN_LINK"], [AC_MSG_ERROR([astaire needs Boost, but it was not found in your system])])
AX_BOOST_THREAD
AX_BOOST_LOG

# Add additional flags
astaire_LIBS+=" -lboost_log"
astaire_LIBS+=" -lboost_thread"
astaire_LIBS+=" -lboost_system"

dnl Adding the library to link to the Makefile
AC_SUBST(astaire_LIBS, "$astaire_LIBS -L$JAVA_HOME/jre/lib/amd64/server")
AC_SUBST(astaire_LIBS, "$astaire_LIBS -ljvm")
AC_SUBST(astaire_ADDITIONALFLAGS, "$astaire_ADDITIONALFLAGS -I$JAVA_HOME/include/")
AC_SUBST(astaire_ADDITIONALFLAGS, "$astaire_ADDITIONALFLAGS -I$JAVA_HOME/include/linux/")
AC_SUBST(astaire_CUSTOMFLAGS, $astaire_ADDITIONALFLAGS)

AC_OUTPUT