AC_INIT([fql], [1.0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AM_PROG_AR
LT_INIT

#CFLAGS+=" -std=gnu11"

AC_PROG_CC
AC_PROG_CXX

#AC_CHECK_HEADER([antlr4-runtime.h])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile lib/antlr/Makefile lib/antlr/gen/Makefile util/Makefile src/Makefile ])

# Check
AC_ARG_WITH([check],
    AS_HELP_STRING([--without-check], [Ignore presence of check and disable it]))

AS_IF([test "x$with_check" != "xno"],
      [PKG_CHECK_MODULES([CHECK], [check >= 0.12.0], [have_check=yes], [have_check=no])],
      [have_check=no])

AS_IF([test "x$have_check" = "xyes"], [AC_CONFIG_FILES([tests/Makefile])],
      [AS_IF([test "x$with_check" = "xyes"],
             [AC_MSG_ERROR([check not found.])
      ])
])

AM_CONDITIONAL([USE_CHECK], [test "x$with_check" != "xno" -a "x$have_check" = "xyes"])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
AS_IF([test "x$DOXYGEN" = "x"],
      [AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])],
      [AC_CONFIG_FILES([lib/docs/Makefile])])

AM_CONDITIONAL([USE_DOXYGEN],[test -n "$DOXYGEN"])
AM_COND_IF([USE_DOXYGEN], [AC_CONFIG_FILES([lib/docs/Doxyfile])])

AC_OUTPUT

