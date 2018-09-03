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

dnl Include Java bindings
AC_ARG_WITH([java], 
	       [AS_HELP_STRING([--with-java],
			       [Enable Java bindings])],
		[jni_on=1],
		[jni_on=0])
AM_CONDITIONAL([ENABLE_JAVA], [test x$jni_on = x1])
AM_COND_IF([ENABLE_JAVA],
	   AC_MSG_RESULT([Java bindings enabled])
	   [astaire_ADDITIONALFLAGS+=" -I$JAVA_HOME/include -I$JAVA_HOME/include/linux"]
	   [astaire_LIBS+="  -L$JAVA_HOME/jre/lib/amd64/server -ljvm"]
	   AC_DEFINE([ENABLE_JAVA], [1], [Include Java bindings compilation]),
	   AC_MSG_RESULT([Java bindings disabled])
	   AC_DEFINE([ENABLE_JAVA], [0], [Don't include Java bindings compilation]))
AM_CONDITIONAL([JAVA_HOME_SET], [test ! -z "$JAVA_HOME"])

dnl Checking other required libraries
AC_CHECK_LIB(viface, send, [astaire_LIBS+=" -lviface"], [AC_MSG_ERROR([send was not found in libviface])])

AC_CHECK_LIB(pthread, pthread_create, [astaire_LIBS+=" -lpthread"], [AC_MSG_ERROR([pthread_create was not found in libpthread])])

AC_CHECK_LIB(tins, main, [astaire_LIBS+=" -ltins"], [AC_MSG_ERROR([main was not found in libtins])])

AC_CHECK_LIB(pcap, pcap_loop, [astaire_LIBS+=" -lpcap"], [AC_MSG_ERROR([pcap_loop was not found in libpcap])])

AC_CHECK_LIB(jsoncpp, main, [astaire_LIBS+=" -ljsoncpp"], [AC_MSG_ERROR("main was not found in libjsoncpp")])


dnl Checking boost dependencies
AX_BOOST_BASE([1.58],[astaire_ADDITIONALFLAGS+=" -DBOOST_LOG_DYN_LINK"], [AC_MSG_ERROR([astaire needs Boost, but it was not found in your system])])
AX_BOOST_THREAD
AX_BOOST_LOG

# Add additional flags
astaire_LIBS+=" -lboost_log"
astaire_LIBS+=" -lboost_thread"
astaire_LIBS+=" -lboost_system"

dnl Adding the library to link to the Makefile
AC_SUBST(astaire_LIBS, "$astaire_LIBS")
AC_SUBST(astaire_LIBS, "$astaire_LIBS")
AC_SUBST(astaire_CXXFLAGS, "$astaire_ADDITIONALFLAGS")

AC_OUTPUT