#!/bin/bash

while read -r test_name; do
  if [ -f ".libs/${test_name}" ]; then
    test_bin=".libs/${test_name}"
  elif [ -f "$test_name" ]; then
    test_bin=$test_name
  else
    echo "Test $test_name does not exist, run 'make check' to rebuild it"
    continue
  fi

  if ! valgrind --leak-check=full --track-origins=yes "$test_bin"; then
    echo "Test $test_name FAILED!"
  fi

  >&2 printf '\n\n%s\n\n' "${test_name^^}"
done < <(make -n check | head -1 | awk '{for (i=2; i <= NF; i++) print $i; exit}') 2> leak_report.txt

if grep -q 'definitely lost:' leak_report.txt; then
  printf '\n\nLeaks found. See leak_report.txt\n\n'
fi
