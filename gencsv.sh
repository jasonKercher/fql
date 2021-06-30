#!/bin/bash

# Basically this:
# https://stackoverflow.com/questions/29253591/generate-large-csv-with-random-content-in-bash

hexdump -v -e '5/1 "%02x""\n"' /dev/urandom |
  awk -v OFS='\t' '
    NR == 1 { print "foo", "bar", "baz" }
    { print substr($0, 1, 8), substr($0, 9, 2), int(NR * 32768 * rand()) }' |
  head -n "$1"

