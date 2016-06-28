#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([cpp-logger],[m4_esyscmd_s(echo $(git describe --always --abbrev=0))],[herbert.koelman@me.com],[],[http://herbertkoelman.github.io/cpp-logger/])
AC_LANG(C++)
AC_CONFIG_SRCDIR([logger.cpp])
AC_CONFIG_AUX_DIR([/usr/bin ../ ../bin])
AC_CONFIG_HEADERS([../include/logger/config.h])

AC_DEFINE_UNQUOTED(CPP_LOGGER_VERSION, ["$PACKAGE_NAME $PACKAGE_VERSION - $(git log --pretty='%H' | head -1) - compiled on `uname -sv`."], [logging facility.])

CXXFLAGS=""
CFLAGS="-I../include"

AC_ARG_VAR([AUXDIR], [auxiliary include and lib directories])
AC_ARG_ENABLE([auxdir], AS_HELP_STRING([--enable-auxdir=PATH],[set AUXDIR to point to this path]), AUXDIR=$enable_auxdir)

AC_ARG_ENABLE([localdir], AS_HELP_STRING([--enable-localdir],[adds -I and -L to /usr/local include and lib respectively.]), CFLAGS="-I /usr/local/include" LDFLAGS=" -L /usr/local/lib" )

# Checks for programs.
AC_PROG_CXX([xlC_r xlC g++ c++ gcc cl KCC CC cxx cc++ aCC ])
AC_PROG_CC([xlc xlc_r gcc cl cc])
AC_CHECK_PROG([AR],[ar],[ar -rv])
AC_CHECK_PROG([MV],[mv],[mv],[echo "no mv command found"])
AC_PROG_RANLIB

if test -n "$AUXDIR"
then
  CFLAGS="$CFLAGS -I$AUXDIR/include"
  LDFLAGS="$LDFLAGS -L$AUXDIR/lib"
  AC_MSG_NOTICE([WARNING using auxiliary directory $AUXDIR (CFLAGS=$CFLAGS, LDFLAGS=$LDFLAGS)])
fi

AC_CHECK_SIZEOF([long])
if test $ac_cv_sizeof_long == "8" 
then
  AC_SUBST(BITS,"64")
  bits=64
else
  AC_MSG_RESULT([using 32 bits (default)])
  AC_SUBST(BITS,"32")
  bits=32
fi

OBJECTS=""
for src in *.cpp
do
  obj=`basename $src .cpp`.o
  OBJECTS="$OBJECTS $obj"
done
AC_SUBST(OBJECTS,$OBJECTS)

AC_MSG_CHECKING([for specific $CXX compiler options])
case "$CXX" in
  xlC_r | xlC)
    CPPFLAGS="-D__IBMCPP_TR1__ -qlanglvl=extended0x:decltype:static_assert:rvaluereferences -qsourcetype=c++ $CPPFLAGS"
    CFLAGS="-qmaxmem=-1 -q$BITS $CFLAGS"
    CXXFLAGS="-O"
    LDFLAGS="$LDFLAGS -brtl -bmaxdata:0x80000000"
    AC_MSG_RESULT([using large memory model (maxdata:0x80000000), optimising level 2 (-O),using run-time link method (-brtl)])
    ;;
  g++ | gcc)
    CPPFLAGS="-x c++ -std=c++11 -frtti $CPPFLAGS"
    CXXFLAGS="-O2"
    AC_MSG_RESULT([yes])
  ;;
  *)
  AC_MSG_RESULT(none found.)
esac

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create],[],[AC_ERROR([missing pthread library.])])
AC_CHECK_LIB([cpp-pthread], [cpp_pthread_version],[],[AC_ERROR([missing cpp-pthread library.])])

# Checks for header files.
#AC_HEADER_STDC

AC_CHECK_HEADER([mutex],  [AC_DEFINE([HAVE_CPP11_MUTEX])])
AC_CHECK_HEADER([chrono], [AC_DEFINE([HAVE_CPP11_CHRONO])])
AC_CHECK_HEADER([thread], [AC_DEFINE([HAVE_CPP11_THREAD])])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CHECK_FUNCS([gettimeofday])

AC_CONFIG_FILES([Makefile cpp-logger.cpp])
AC_OUTPUT

