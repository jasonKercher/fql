#!/bin/bash

# clean only c binaries

rm -v \
	lib/*.o lib/*.lo \
	util/*.o util/*.lo \
	src/*.o tests/*.o 2>> /dev/null
