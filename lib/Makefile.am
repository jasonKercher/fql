SUBDIRS = docs antlr util

AM_CPPFLAGS = -Iinclude
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/fql.h lib.c \
					plan.h plan.c 
					process.h process.c fqlprocesses.c \
					prop.h prop.c \
					query.h query.c \
					table.h table.c \
					schema.h schema.c \
					column.h column.c \
					operation.h operation.c \
					reader.h reader.c \
					logic.h logic.c \
					select.h select.c \
					function.h function.c fqlfunctions.c \
					field.h field.c
libfql_la_LIBADD = antlr/libantlr.la util/libutil.la -lantlr4-runtime -lcsv
