# Created by Wiktor Lazarski 02/04/2020

#!/bin/bash

script_name=$(basename $0)
declare -a files_arr
declare -r LOWERCASE_SED_PATTERN="s/.*/\L&/"
declare -r UPPERCASE_SED_PATTERN="s/.*/\U&/"
declare -r UNSPECIFIED_SED_PATTERN="unspecified"

# function for printing an error
function error_msg()
{
	echo "$script_name: error: $1" 1>&2
}

# function for desplaying help
function display_help()
{
	cat<<EOT
NAME
	modify - modifies file names

SYNOPSIS
	./modify [OPTION]... [FILE]...

DESCRIPTION
	The script is dedicated to lowerizing file names, uppercasing file names or internally calling sed command with a given sed pattern which will operate on file names. Changes may be done either with recursion or without it.

	$script_name [-r] [-l|-u] <dir/file names...>
	$script_name [-r] <sed pattern> <dir/file names...>
	$script_name [-h]

	-h
			display help
		
	-r
			performs recursion modifications
		
	-l|-u
			performs lowerizing/uppercasting of all specified files
		
	<sed pattern>
			set modification pattern according to the one used in sed command

	Options -l, -u or correct <sed pattern> must be specified for a script to be executed.

AUTHOR
	Written by Wiktor Lazarski
EOT
}

# do with command line arguments
recursive=n
sed_pattern=$UNSPECIFIED_SED_PATTERN
while [ "x$1" != "x" ]
do
	case "$1" in
		-r) recursive=y;;
		-l) sed_pattern=$LOWERCASE_SED_PATTERN;;
		-u) sed_pattern=$UPPERCASE_SED_PATTERN;;
		-h) display_help; exit 0;;
		-*) error_msg "bad option $1"; exit 1;;
		*)
			if [ "$sed_pattern" = "$UNSPECIFIED_SED_PATTERN" ]; then
				sed_pattern=$1
			else 
				files_arr+=($1)
			fi;;
	esac

	shift
done

# check if we received proper sed pattern
if ! [[ $sed_pattern =~ s/.+/.*/$ ]]; then
	error_msg "invalid sed pattern specified"
	exit 1
fi

# fuction renames file according to currently set sed pattern
function rename_file()
{
	# get file name without path
	old_name=$(basename $1)
	file_extension=$(echo ".${old_name#*.}")
	if [ -d $1 ]; then
		file_extension=""
	fi	
	
	#remove file extension	 
	old_name=$(echo ${old_name/$file_extension/""})
	
	# modify name 
	new_name=$(echo $old_name | sed $sed_pattern)
	
	if [ "$old_name" = "$new_name" ]; then
		return
	fi
	
	new_name="$(dirname $1)/$(echo "$new_name$file_extension")"
	# rename file
	mv $1 $new_name 
}

function recursive_rename()
{
	# main loop that changes files names
	if [ $recursive = 'y' ] && [ -d $1 ]; then
		for file in "$1"/*
		do
			recursive_rename $file
		done
	fi

	rename_file $1
}

# init renaming
for arr_elem in "${files_arr[@]}"
do
	recursive_rename $arr_elem 
done
