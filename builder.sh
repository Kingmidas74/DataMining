#!/bin/bash

files="*.c *.cpp *.h *.hpp"
sum=""
while [ true ]; do
	current_sum=$(ls -laR src | grep -e '\.hpp$' -e '\.c$' -e '\.cpp$' -e '\.h$' 2>/dev/null | md5sum | cut -f 1 -d ' ')
	echo "check diff............................"
	echo "current_sum="+$current_sum
	if [ "$sum" != "$current_sum" ]; then
		sum=$current_sum
		echo "--------------------------------------------------"
		make ACC=MIC
		echo "--------------------------------------------------"
	fi;
	sleep 5
done
