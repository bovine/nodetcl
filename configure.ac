#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nodetcl], [0.0.1])
AC_CONFIG_SRCDIR([src/nodetcl.cc])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

# Checks for programs.
AC_LANG([C++])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PATH_PROG([NODE_WAF], [node-waf], [], [$PATH:/opt/local/bin:/usr/local/bin])
if test "x$NODE_WAF" = "x"; then
	AC_MSG_ERROR([node-waf not found; it is required to build])
fi
AC_SUBST([NODE_WAF])
AC_PATH_PROG([WAF], [waf])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

$NODE_WAF configure
