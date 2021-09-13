SUBDIRS = util lib src

if USE_CHECK
SUBDIRS += tests
endif

ACLOCAL_AMFLAGS = -I m4


# un-related note...AUR build options:
# -march=x86-64 -mtune=generic -g -O2 -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security -fstack-clash-protection -fcf-protection
