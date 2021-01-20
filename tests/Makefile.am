TESTS = check_constants
check_PROGRAMS = check_constants
check_constants_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_constants.h check_constants.c
check_constants_CFLAGS = @CHECK_CFLAGS@
check_constants_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

