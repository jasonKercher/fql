TESTS = check_constants check_read check_logic
check_PROGRAMS = check_constants check_read check_logic

check_constants_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_constants.c
check_constants_CFLAGS = @CHECK_CFLAGS@
check_constants_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_read_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_read.c
check_read_CFLAGS = @CHECK_CFLAGS@
check_read_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_logic_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_logic.c
check_logic_CFLAGS = @CHECK_CFLAGS@
check_logic_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@
