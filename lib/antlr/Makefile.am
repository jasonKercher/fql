SUBDIRS = gen

AM_CPPFLAGS = -I$(ANTLR4_INCLUDE_PATH)/antlr4-runtime -I../
noinst_LTLIBRARIES = libantlr.la
libantlr_la_SOURCES = upperstream.h upperstream.cpp errorlistener.h errorlistener.cpp listenerinterface.h listenerinterface.cpp antlr.h antlr.cpp
libantlr_la_LIBADD = gen/libgen.la
