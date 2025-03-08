#!/bin/sh

./build/Tests
if [[ "$1" == "--coverage" ]]; then
    gcovr --root ../styx -j 10 --html-details -o coverage/ --exclude 'tests/*' --exclude 'src/main.c'
fi