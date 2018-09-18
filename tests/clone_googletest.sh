#!/bin/bash

# prepare googletest lib
if [ -d googletest ]
then
	cd googletest
	git pull
else
	git clone https://github.com/google/googletest.git
fi

# clean build dir
# if [ -d build ]
# then
# 	cd build
# 	rm -rf *
# else
# 	mkdir build
# 	cd build
# fi

# vim: set ft=sh ts=4 sw=4 noet ai :
