SUBDIRS = docs antlr

AM_CPPFLAGS = -Iinclude
include_HEADERS = include/fql.h
lib_LTLIBRARIES = libfql.la
libfql_la_SOURCES = include/util.h util.c include/fql.h lib.c
libfql_la_LIBADD = antlr/libantlr.la
