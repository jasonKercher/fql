SUBDIRS = docs util antlr

AM_CPPFLAGS = -Iinclude
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/fql.h lib.c plan.h plan.c prop.h prop.c query.h query.c expression.h expression.c table.h table.c schema.h schema.c column.h column.c operation.h operation.c reader.h
libfql_la_LIBADD = antlr/libantlr.la util/libutil.la
