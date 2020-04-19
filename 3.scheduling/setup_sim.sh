#!/bin/bash

# add folder to a classpath
CLASSPATH=.:$CLASSPATH
export CLASSPATH

# unpack files
tar -xf task3.tgz
cd task3/ftp
./setUp

# display final message
echo "Setting up simulator"
