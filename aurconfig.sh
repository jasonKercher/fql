#!/bin/bash

# Run the configure script with the CFLAGS set by AUR's makepkg

CFLAGS='-march=x86-64 -mtune=generic -g -O2 -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security -fstack-clash-protection -fcf-protection' ./configure
