#!/bin/bash

# sets up simulator for experiments
function setup_simulator()
{
	# add folder to a classpath
	CLASSPATH=.:$CLASSPATH
	export CLASSPATH

	# unpack files
	tar -xf task4.tgz
	cd task4/ftp
	./setUp

	# compile simulator
	cd ../work
	javac -nowarn *.java

	# jump out of folder
	cd ../..

	# display final message
	echo "Setting up simulator - DONE"
}

# runs simulation
function run_simulator()
{
	cd task4/work
	java MemoryManagement ../../commands ../../memory.conf
	cd ../..
}

# cleans experiment results and simulator env
function clean()
{
	rm -r task4 tracefile

	# display final message
	echo "Cleaning - DONE"
}

# lab script
if [ "$1" == "clean" ]; then
	clean
elif [ "$1" == "setup" ]; then 
	setup_simulator
elif [ "$1" == "run" ]; then
	run_simulator
fi
