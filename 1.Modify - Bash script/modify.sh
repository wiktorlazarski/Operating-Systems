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
usage:
	$script_name [-r] [-l|-u] <dir/file names...>
	$script_name [-r] <sed pattern> <dir/file names...>
	$script_name [-h]
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
		-h) display_help;;
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

# fuction renames file according to currently set sed pattern
function rename_file()
{
	file_name=$1

	# get file name without path
	old_name=`basename $file_name`
	
	file_extension=$(echo ".${old_name#*.}")
	#remove file extension	 
	old_name=$(echo ${old_name/$file_extension/""})

	# modify name 
	new_name=$(echo $old_name | sed $sed_pattern)
	
	if [ "$old_name" = "$new_name" ]; then
		return
	fi
	
	new_name=$(echo "$new_name$file_extension")

	# rename file
	mv $file_name $new_name 
}

# main loop that changes files names
for file_name in "${files_arr[@]}"
do
	rename_file $file_name
done
