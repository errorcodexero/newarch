#!/bin/bash

if [ $# -ne 1 ]; then
	echo Expected one argument, either p or c
	echo p is for practice robot
	echo c is for competition robot
	exit 1
fi

FILE=""
if [ $1 == "p" ]; then
	echo Copying parameters for practice bot
	FILE="practice"
else
	if [ $1 == "c" ]; then
		echo Copying paramster for competition bot
		FILE="comp"
	else
		echo Expected argument to be 'p' or 'c'.
		exit 1
	fi
fi
	

scp lvuser@10.14.25.2:/home/lvuser/params.txt params-$FILE.txt
