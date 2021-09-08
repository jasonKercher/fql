#!/bin/bash

while read -r cpath; do
	if [ -d "${cpath}/antlr4-runtime" ]; then
		printf "%s/" "$cpath"
		break
	fi
done < <(cpp -v /dev/null -o /dev/null 2>&1 \
       		| grep -A8 'include <...> search starts here:' \
		| grep '^ ' \
		| sed 's/^\s*//')
