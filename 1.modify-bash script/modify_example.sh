#!/bin/bash
# Created by Wiktor Lazarski 02/04/2020


function display_tree_structure()
{
	echo "File tree structure:"
	tree --noreport
	echo ""
}

# Example 1: Display help function
echo "------------------------------ PRINT HELP ----------------------------"
echo "	./modify.sh -h"

./modify.sh -h

echo ""

echo "------------------------------ -l/-u lowerize uppercast --------------"
echo "					Step 1: Create file tree structure."
# create file tree structure
mkdir test
touch test.txt
mkdir test/test
touch test/test.txt
mkdir test/test/test
touch test/test/test.txt

display_tree_structure

# perform non-recursive uppercasting
echo "					Step 2: Perform non-recursive uppercasting"
echo "./modify.sh -u test test.txt"

./modify.sh -u test test.txt

display_tree_structure

# perform recursive uppercasting
echo "					Step 3: Perform recursive uppercasting"
echo "./modify.sh -r -u TEST"
./modify.sh -r -u TEST

display_tree_structure

# perform non-recursive lowerizing 
echo "					Step 4: Perform non-recursive lowerizing"
echo "./modify.sh -l TEST TEST.txt"

./modify.sh -l TEST TEST.txt

display_tree_structure

# perform recursive lowerizing
echo "					Step 5: Perform recursive lowerizing"
echo "./modify.sh -r -l TEST"
./modify.sh -r -l test

display_tree_structure

echo "---------------------------- <sed pattern> modification --------------"
echo "					Step 1: Replace first t into WUT in test and test.txt non-recursively"
echo "./modify.sh s/t/WUT/ test test.txt" 
./modify.sh s/t/WUT/ test test.txt

display_tree_structure

echo "					Step 2: Replace first t into WUT in test recursively"
echo "./modify.sh -r s/t/WUT/ test" 
./modify.sh -r s/t/WUT/ WUTest

display_tree_structure

echo "					Step 3: Replace first WUT into t in all files recursively"
echo "./modify.sh -r s/WUT/t/ WUTesWUT WUTest.txt" 
./modify.sh -r s/WUT/t/ WUTesWUT WUTest.txt

display_tree_structure

echo "					Step 4: Wrong pattern specified by the user"
echo "Missing '/' at the end of sed pattern."
echo "./modify.sh -r s/WUT/t tesWUT" 
./modify.sh -r s/WUT/t tesWUT

echo "Missing 's' at the beginning of sed pattern."
echo "./modify.sh -r WUT/WUT/t/ tesWUT" 
./modify.sh -r WUT/WUT/t/ tesWUT 

echo "No first sed pattern argument specified"
echo "./modify.sh -r s//t/ tesWUT" 
./modify.sh -r s//t/ tesWUT

echo ""
echo "Error messages are displayed and file tree structure remains unchanged"
display_tree_structure

# remove file tree structure
rm -r tesWUT test.txt
