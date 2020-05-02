#!/bin/bash

# sets up simulator for experiments
function setup_simulator()
{
	# add folder to a classpath
	CLASSPATH=.:$CLASSPATH
	export CLASSPATH

	# unpack files
	tar -xf task3.tgz
	cd task3/ftp
	./setUp

	# jump out of folder
	cd ../..

	# display final message
	echo "Setting up simulator - DONE"
}

# cleans experiment results and simulator env
function clean()
{
	rm -r task3 results

	# display final message
	echo "Cleaning - DONE" 
}

# performs set of experiments and saves in results folder
function perform_experiments()
{
	SCHEDULER_CLASSNAME="Scheduling"
	SUMMARY_PROCESSES_PATH="./task3/work/Summary-Processes"	
	SUMMARY_RESULTS_PATH="./task3/work/Summary-Results"
	CONFIGS_PATH="./configurations"

	mkdir results

	for file in "$CONFIGS_PATH"/*
	do	
		basename=$(basename $file)

		cd "task3/work"
		java $SCHEDULER_CLASSNAME "../../configurations/$basename"
		cd "../.."

		no_extension_basename=$(echo $basename | sed "s/\..*//")
		output_file="./results/$no_extension_basename-results.txt"
		touch $output_file

		cat $SUMMARY_PROCESSES_PATH > $output_file
		cat $SUMMARY_RESULTS_PATH >> $output_file 
	done
	
	echo "Task completed. Results are READY."
}

# lab script
if [ "$1" == "clean" ]; then
	clean
else
	setup_simulator
	perform_experiments
fi
