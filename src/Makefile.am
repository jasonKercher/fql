AM_CPPFLAGS = -I$(top_srcdir)/lib/include -I$(top_srcdir)/util -I$(top_srcdir)/libcsv/include
bin_PROGRAMS = fql stdcsv
fql_SOURCES = fql.c
fql_LDADD = $(top_srcdir)/lib/libfql.la $(top_srcdir)/libcsv/libcsv.la -lreadline

stdcsv_SOURCES = stdcsv.c
stdcsv_LDADD = $(top_srcdir)/libcsv/libcsv.la



