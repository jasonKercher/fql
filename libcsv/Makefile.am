SUBDIRS = docs

AM_CPPFLAGS = -Iinclude -I../util/
include_HEADERS = include/csv.h
lib_LTLIBRARIES = libcsv.la
libcsv_la_LIBADD = ../util/libutil.la
libcsv_la_SOURCES = misc.h misc.c \
					charnode.h charnode.c \
					csverror.h csverror.c \
					csvsignal.h csvsignal.c \
					safegetline.h safegetline.c \
					reader.c writer.c csv.c
