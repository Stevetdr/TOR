#!/bin/bash

echo "parte comparazione dei due file :"

dir_a = "/home/pi/GIT-REPO/steve/"
dir_b = "/home/pi/steve/"


diff -r --brief $dir_a $dir_b