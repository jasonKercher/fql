SUBDIRS = antlr
if USE_DOXYGEN
SUBDIRS += docs
endif

schemadir = $(datadir)/fql/schema/

AM_CPPFLAGS = -Iinclude -I$(top_srcdir)/util/
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
schema_DATA = schema/noheader
libfql_la_SOURCES = include/fql.h fqlhandle.h libfql.c 	    \
	query.h query.c                                     \
		table.h table.c                             \
			schema.h schema.c                   \
		expression.h expression.c                   \
			switchcase.h switchcase.c           \
			function.h function.c               \
				scalarfn.c                  \
		logic.h logic.c                             \
			fqllogic.c                          \
				like.c                      \
				inlist.c                    \
		group.h group.c                             \
			aggregate.h aggregate.c             \
				aggregatefn.c               \
		operation.h operation.c                     \
			fqlselect.h fqlselect.c             \
			fqldelete.h fqldelete.c             \
			fqlupdate.h fqlupdate.c             \
		order.h order.c                             \
	                                                    \
	fqlplan.h fqlplan.c                                 \
		process.h process.c                         \
			fqlprocesses.c                      \
	                                                    \
	io.h                                                \
		reader.h reader.c                           \
			libcsvread.c fixedread.c subquery.c \
		writer.h writer.c                           \
			libcsvwrite.c fixedwrite.c          \
	                                                    \
	record.h record.c                                   \
	field.h field.c                                     \
	fqlsig.h fqlsig.c                                   \
	misc.h

libfql_la_CFLAGS = -Wextra -Wall -Wno-unused-parameter -DSCHEMA_DATADIR=$(schemadir)
libfql_la_LDFLAGS = -l:libcsv.so.1 -lpcre2-8 -lantlr4-runtime -pthread -version-info 1:1:0
libfql_la_LIBADD = antlr/libantlr.la ../util/libutil.la
