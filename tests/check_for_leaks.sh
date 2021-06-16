#!/bin/bash

we_leaky=N

vgrun()
{
    local test_bin="$1"
    local test_name="$2"
        # --track-origins=yes \
    valgrind \
        --leak-check=full \
        "$test_bin" 2> ${test_name}.vgreport

    if grep -q 'definitely lost:' ${test_name}.vgreport; then
        we_leaky=Y
    elif grep -q '0 errors from 0 contexts' ${test_name}.vgreport; then
        rm ${test_name}.vgreport
    fi
}

while read -r test_name; do
    if [ -f ".libs/${test_name}" ]; then
        test_bin=".libs/${test_name}"
    elif [ -f "$test_name" ]; then
        test_bin="./${test_name}"
    else
        echo "Test $test_name does not exist, run 'make check'"
        continue
    fi

    # spawn background process
    vgrun "$test_bin" "$test_name" &

done < <(make -n check | \
         awk '{if(NR==1){for (i=2; i <= NF; i++) print $i}}')

# wait for said background processes
wait

if [ $we_leaky = Y ]; then
  printf '\n\nLeaks found. See reports.\n\n'
fi
