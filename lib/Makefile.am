SUBDIRS = docs antlr

AM_CPPFLAGS = -Iinclude -I$(top_srcdir)/util/
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/fql.h fqlhandle.h libfql.c \
					fqlplan.h fqlplan.c \
					process.h process.c fqlprocesses.c \
					query.h query.c \
					table.h table.c \
					schema.h schema.c \
					column.h column.c \
					group.h group.c \
					aggregate.h aggregate.c aggregatefn.c \
					order.h order.c \
					operation.h operation.c \
					reader.h reader.c libcsvread.c \
					writer.h writer.c \
					logic.h logic.c fqllogic.c \
					fqlselect.h fqlselect.c \
					function.h function.c scalarfn.c \
					record.h record.c \
					field.h field.c
libfql_la_LDFLAGS = -l:libcsv.so.1 -lantlr4-runtime -pthread -version-info 1:0:0
libfql_la_LIBADD = antlr/libantlr.la ../util/libutil.la
