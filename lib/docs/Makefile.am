if USE_DOXYGEN

directory = $(top_srcdir)/lib/docs/man/man3/

dist_man_MANS = $(directory)/fql.h.3
$(directory)/fql.h.3: doxyfile.stamp

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_srcdir)/lib/docs/man
endif
