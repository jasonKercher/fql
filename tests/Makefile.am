TESTS =                        \
	check_hashmap          \
	check_constants        \
	check_read             \
	check_logic            \
	check_scalar           \
	check_switch           \
	check_null             \
	check_join             \
	check_subquery         \
	check_group            \
	check_having           \
	check_order            \
	check_top              \
	check_union            \
	check_difficult        \
	check_operations       \
	check_controlflow      \
	check_stdin            \
	check_output           \
	check_thread_output    \
	check_failure
check_PROGRAMS =               \
	check_hashmap          \
	check_constants        \
	check_read             \
	check_logic            \
	check_scalar           \
	check_switch           \
	check_null             \
	check_join             \
	check_subquery         \
	check_group            \
	check_having           \
	check_order            \
	check_top              \
	check_union            \
	check_difficult        \
	check_operations       \
	check_controlflow      \
	check_stdin            \
	check_output           \
	check_thread_output    \
	check_failure

common_src = $(top_builddir)/lib/include/fql.h check_common.h check_common.c

check_hashmap_SOURCES = $(top_builddir)/util/hashmap.h check_hashmap.c
check_hashmap_CFLAGS = @CHECK_CFLAGS@
check_hashmap_LDADD = $(top_builddir)/util/libutil.la @CHECK_LIBS@

check_constants_SOURCES =  $(common_src) check_constants.c
check_constants_CFLAGS = @CHECK_CFLAGS@
check_constants_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_read_SOURCES =  $(common_src) check_read.c
check_read_CFLAGS = @CHECK_CFLAGS@
check_read_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_scalar_SOURCES = $(common_src) check_scalar.c
check_scalar_CFLAGS = @CHECK_CFLAGS@
check_scalar_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_logic_SOURCES = $(common_src) check_logic.c
check_logic_CFLAGS = @CHECK_CFLAGS@
check_logic_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_switch_SOURCES = $(common_src) check_switch.c
check_switch_CFLAGS = @CHECK_CFLAGS@
check_switch_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_null_SOURCES = $(common_src) check_null.c
check_null_CFLAGS = @CHECK_CFLAGS@
check_null_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_join_SOURCES = $(common_src) check_join.c
check_join_CFLAGS = @CHECK_CFLAGS@
check_join_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_subquery_SOURCES = $(common_src) check_subquery.c
check_subquery_CFLAGS = @CHECK_CFLAGS@
check_subquery_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_group_SOURCES = $(common_src) check_group.c
check_group_CFLAGS = @CHECK_CFLAGS@
check_group_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_having_SOURCES = $(common_src) check_having.c
check_having_CFLAGS = @CHECK_CFLAGS@
check_having_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_order_SOURCES = $(common_src) check_order.c
check_order_CFLAGS = @CHECK_CFLAGS@
check_order_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_top_SOURCES = $(common_src) check_top.c
check_top_CFLAGS = @CHECK_CFLAGS@
check_top_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_union_SOURCES = $(common_src) check_union.c
check_union_CFLAGS = @CHECK_CFLAGS@
check_union_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_difficult_SOURCES = $(common_src) check_difficult.c
check_difficult_CFLAGS = @CHECK_CFLAGS@
check_difficult_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_stdin_SOURCES = $(common_src) check_stdin.c
check_stdin_CFLAGS = @CHECK_CFLAGS@
check_stdin_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_operations_SOURCES = $(common_src) check_operations.c
check_operations_CFLAGS = @CHECK_CFLAGS@
check_operations_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_controlflow_SOURCES = $(common_src) check_controlflow.c
check_controlflow_CFLAGS = @CHECK_CFLAGS@
check_controlflow_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_output_SOURCES = $(common_src) check_output.c output.h output.c
check_output_CFLAGS = @CHECK_CFLAGS@
check_output_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_thread_output_SOURCES = $(common_src) check_thread_output.c output.h output.c
check_thread_output_CFLAGS = @CHECK_CFLAGS@
check_thread_output_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@

check_failure_SOURCES = $(common_src) check_failure.c
check_failure_CFLAGS = @CHECK_CFLAGS@
check_failure_LDADD = $(top_builddir)/lib/libfql.la @CHECK_LIBS@
