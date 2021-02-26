SUBDIRS = docs antlr

AM_CPPFLAGS = -Iinclude -I$(top_srcdir)/util/ -I$(top_srcdir)/libcsv/include
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/fql.h fqlhandle.h libfql.c \
					fqlplan.h fqlplan.c \
					process.h process.c fqlprocesses.c \
					query.h query.c \
					table.h table.c \
					schema.h schema.c \
					column.h column.c \
					operation.h operation.c \
					reader.h reader.c libcsvread.c mmapcsvread.c \
					writer.h writer.c \
					logic.h logic.c fqllogic.c \
					select.h select.c \
					function.h function.c fqlfunctions.c \
					record.h record.c \
					field.h field.c
libfql_la_LDFLAGS = -lantlr4-runtime -pthread
libfql_la_LIBADD = antlr/libantlr.la ../util/libutil.la ../libcsv/libcsv.la 
