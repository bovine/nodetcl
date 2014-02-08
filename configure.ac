#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nodetcl],[1.0.0],[jeff@bovine.net])
AC_CONFIG_SRCDIR([src/nodetcl.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([autotools_build-aux])
AC_CONFIG_MACRO_DIR([autotools_m4])
AC_CANONICAL_TARGET
case ${host} in
  *-*-*freebsd*)
    export CPPFLAGS="${CPPFLAGS} -I/usr/local/include/tcl8.5"
    ;;
  *-*-*darwin*)
    if test -d /opt/local/include; then
      export CPPFLAGS="${CPPFLAGS} -I/opt/local/include"
      if test -d /opt/local/include/node; then
        export CPPFLAGS="${CPPFLAGS} -I/opt/local/include/node"
        if test -d /opt/local/include/node/uv-private; then
          export CPPFLAGS="${CPPFLAGS} -I/opt/local/include/node/uv-private"
        fi
      elif test -d /opt/local/include/node8; then
        export CPPFLAGS="${CPPFLAGS} -I/opt/local/include/node8"
        if test -d /opt/local/include/node8/uv-private; then
          export CPPFLAGS="${CPPFLAGS} -I/opt/local/include/node8/uv-private"
        fi
      fi
    fi
    export AM_LDFLAGS="-bundle -undefined dynamic_lookup"
    AC_SUBST([AM_LDFLAGS])
    if test -d /opt/local/lib; then
      export LDFLAGS="${LDFLAGS} -L/opt/local/lib"
      if test -d /opt/local/lib/node8; then
        export LDFLAGS="${LDFLAGS} -L/opt/local/lib/node8"
      fi
    fi
    ;;
esac
AC_SYS_LARGEFILE
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE

# Checks for programs.
AM_PROG_AR
AC_PROG_RANLIB
AC_PROG_CPP
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_OBJCXX
AC_PROG_OBJCXXCPP
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AM_PATH_PYTHON
AC_PATH_PROG([NODE_BIN],[node node8],[],[${PATH}:/opt/local/bin])
AC_PATH_PROG([NODE_WAF],[node-waf],[],[${PATH}:/opt/local/bin:/usr/local/bin])
if test "x${NODE_WAF}" = "x"; then
	test -z "${NODE_WAF}"
	AC_MSG_ERROR([node-waf not found; it is required to build])
fi
AC_SUBST([NODE_WAF])
AC_PATH_PROG([WAF],[waf waf-1.6.2],[],[${PATH}:.])

# Try using libtool for linking:
LT_INIT([dlopen])
LT_LANG([C++])

# Checks for libraries.
AC_SEARCH_LIBS([Tcl_Init],[Tcl Tcl85 Tcl8.5])
# FIXME: Replace `main' with a function in `-lv8':
AC_CHECK_LIB([v8],[main])
# FIXME: Replace `main' with a function in `-lv8_base':
AC_CHECK_LIB([v8_base],[main])
# FIXME: Replace `main' with a function in `-lv8_nosnapshot':
AC_CHECK_LIB([v8_nosnapshot],[main])
# FIXME: Replace `main' with a function in `-lv8_snapshot':
AC_CHECK_LIB([v8_snapshot],[main])

# Checks for header files.
AC_CHECK_HEADERS([node.h stdio.h tcl.h v8.h])
AC_CHECK_HEADERS_ONCE([node/node.h node/v8.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([node-waf],[
test ! -z ${NODE_WAF} && ${NODE_WAF} configure
])
AC_OUTPUT
