TESTS =          check_hashmap check_constants check_read check_logic check_join check_subquery check_group check_having check_order check_top check_union check_difficult check_failure
check_PROGRAMS = check_hashmap check_constants check_read check_logic check_join check_subquery check_group check_having check_order check_top check_union check_difficult check_failure

check_hashmap_SOURCES = $(top_builddir)/util/hashmap.h check_hashmap.c
check_hashmap_CFLAGS = @CHECK_CFLAGS@
check_hashmap_LDADD = $(top_builddir)/util/libutil.la @CHECK_LIBS@

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

check_join_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_join.c
check_join_CFLAGS = @CHECK_CFLAGS@
check_join_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_subquery_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_subquery.c
check_subquery_CFLAGS = @CHECK_CFLAGS@
check_subquery_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_group_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_group.c
check_group_CFLAGS = @CHECK_CFLAGS@
check_group_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_having_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_having.c
check_having_CFLAGS = @CHECK_CFLAGS@
check_having_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_order_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_order.c
check_order_CFLAGS = @CHECK_CFLAGS@
check_order_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_top_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_top.c
check_top_CFLAGS = @CHECK_CFLAGS@
check_top_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_union_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_union.c
check_union_CFLAGS = @CHECK_CFLAGS@
check_union_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_difficult_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_difficult.c
check_difficult_CFLAGS = @CHECK_CFLAGS@
check_difficult_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_failure_SOURCES = $(top_builddir)/lib/include/fql.h \
			check_common.h check_common.c \
			check_failure.c
check_failure_CFLAGS = @CHECK_CFLAGS@
check_failure_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@
