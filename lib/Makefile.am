SUBDIRS = docs util antlr

AM_CPPFLAGS = -Iinclude
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/fql.h lib.c plan.h prop.h prop.c
libfql_la_LIBADD = antlr/libantlr.la util/libutil.la
