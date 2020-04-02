#!/bin/bash
# Created by Wiktor Lazarski 02/04/2020

# Example 1: Display help function
echo "------------------------------ PRINT HELP ----------------------------"
echo "	./modify.sh -h"

#./modify.sh -h

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

# display file tree structure
tree --noreport 
echo ""

# perform non-recursive uppercasting
echo "					Step 2: Perform non-recursive uppercasting"
echo "./modify.sh -u test test.txt"

./modify.sh -u test test.txt

echo "File tree structure:"
tree --noreport
echo ""

# perform recursive uppercasting
echo "					Step 3: Perform recursive uppercasting"
echo "./modify.sh -r -u TEST"
./modify.sh -r -u TEST

echo "File tree structure:"
tree --noreport
echo ""

# perform non-recursive lowerizing 
echo "					Step 4: Perform non-recursive lowerizing"
echo "./modify.sh -l TEST TEST.txt"

./modify.sh -l TEST TEST.txt

echo "File tree structure:"
tree --noreport
echo ""

# perform recursive lowerizing
echo "					Step 5: Perform recursive lowerizing"
echo "./modify.sh -r -l TEST"
./modify.sh -r -l test

echo "File tree structure:"
tree --noreport
echo ""

echo "---------------------------- <sed pattern> modification --------------"

# remove file tree structure
rm -r test test.txt
