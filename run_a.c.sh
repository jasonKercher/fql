#!/bin/bash

if gcc -O0 -g util/*.c a.c; then
  ./a.out "$1"
fi
