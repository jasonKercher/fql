SUBDIRS = docs antlr

AM_CPPFLAGS = -Iinclude -I$(top_srcdir)/util/
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/fql.h fqlhandle.h libfql.c \
					fqlplan.h fqlplan.c \
					query.h query.c \
					schema.h schema.c \
					table.h table.c \
					expression.h expression.c switchcase.h switchcase.c function.h function.c scalarfn.c \
					group.h group.c \
					aggregate.h aggregate.c aggregatefn.c \
					order.h order.c \
					operation.h operation.c fqlselect.h fqlselect.c \
					io.h \
					reader.h reader.c libcsvread.c fixedread.c subquery.c \
					writer.h writer.c libcsvwrite.c fixedwrite.c \
					logic.h logic.c fqllogic.c like.c inlist.c\
					process.h process.c fqlprocesses.c \
					record.h record.c \
					field.h field.c \
					misc.h
libfql_la_CFLAGS = -Wextra -Wall -Wno-unused-parameter
libfql_la_LDFLAGS = -l:libcsv.so.1 -lpcre2-8 -lantlr4-runtime -pthread -version-info 1:1:0
libfql_la_LIBADD = antlr/libantlr.la ../util/libutil.la
