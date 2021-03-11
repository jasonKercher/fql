SUBDIRS = gen

AM_CPPFLAGS = -std=c++17 -Wno-attributes -I$(ANTLR4_CPATH)/antlr4-runtime -I../ -I../include
noinst_LTLIBRARIES = libantlr.la
libantlr_la_SOURCES = upperstream.h upperstream.cpp errorlistener.h errorlistener.cpp listenerinterface.h listenerinterface.cpp antlr.h antlr.cpp
libantlr_la_LIBADD = gen/libgen.la
