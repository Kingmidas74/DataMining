#!/bin/sh

DATE=`date +%d-%m-%Y-%H-%M-%S`;
touch $DATE.NODE.csv
TITLEROW="#;N\T;"
for t in `seq 6 6 24`;
	do
		TITLEROW=$TITLEROW$t";"
	done
echo "$TITLEROW" >> $DATE.NODE.csv
for n in `seq 10000 10000 100000`;
	do	
		for i in `seq 0 1 99`;
			do
				ROW="$i;$n;";
				for t in `seq 6 6 24`
					do
						./$1 -n $n -t $t
						PROPERTIES=$(tac log.csv |egrep -m 1 .)				
						RUNTIME=${PROPERTIES##*;}						
						ROW=$ROW$RUNTIME";"
					done;
				echo "$ROW" >> $DATE.NODE.csv;
			done;
		echo "\n" >>$DATE.NODE.csv;
	done   