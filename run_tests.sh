#!/bin/bash

./build/Tests
gcovr --root ../styx -j 10 --html-details -o coverage/ --exclude 'tests/*' --exclude 'src/main.c'