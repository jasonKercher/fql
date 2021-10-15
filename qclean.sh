#!/bin/bash

# clean only c binaries ... skip c++ build

rm -v \
	lib/*.o lib/*.lo \
	util/*.o util/*.lo \
	src/*.o tests/*.o 2>> /dev/null || true
