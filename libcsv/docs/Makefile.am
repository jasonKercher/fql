if HAVE_DOXYGEN

directory = $(top_srcdir)/libcsv/docs/man/man3/

dist_man_MANS = $(directory)/csv.h.3 $(directory)/csv_record.3 $(directory)/csv_reader.3 $(directory)/csv_writer.3
$(directory)/csv.h.3: doxyfile.stamp
$(directory)/csv_reader.3: doxyfile.stamp
$(directory)/csv_writer.3: doxyfile.stamp
$(directory)/csv_record.3: doxyfile.stamp


doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_srcdir)/lib/docs/man
endif
