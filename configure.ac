AC_INIT([stdcsv], [1.0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_CFLAGS="-g -O0 -fsanitize=address -Wall"

AM_PROG_AR
LT_INIT
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile lib/docs/Makefile])

# Check
AC_ARG_WITH([check],
    AS_HELP_STRING([--without-check], [Ignore presence of check and disable it]))

AS_IF([test "x$with_check" != "xno"],
      [PKG_CHECK_MODULES([CHECK], [check >= 0.12.0], [have_check=yes], [have_check=no])],
      [have_check=no])

AS_IF([test "x$have_check" = "xyes"],
      [AC_CONFIG_FILES([tests/Makefile])],
      [AS_IF([test "x$with_check" = "xyes"],
             [AC_MSG_ERROR([check not found.])
      ])
])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

AM_CONDITIONAL([HAVE_DOXYGEN],[test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([lib/docs/Doxyfile])])

AC_OUTPUT

