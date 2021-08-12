#!/bin/bash

count=$1
count=${count:-10}
((++count)) # header
hexdump -v -e '5/1 "%02x""\n"' /dev/urandom |
awk -v OFS='\t' '
  NR == 1 { print "foo", "bar", "baz" }
  { print substr($0, 1, 8), substr($0, 9, 2), int(NR * 32768 * rand()) }' |
head -n "$count"
