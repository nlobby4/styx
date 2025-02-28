#!/bin/bash

./build/Tests
# LOCATION=coverage
#cd src
#find -name '*.c' -exec cp -t ../$LOCATION {} +
#find -name '*.gcno' -exec cp -t ../$LOCATION {} +
#find -name '*.gcda' -exec cp -t ../$LOCATION {} +
#cd ..
#cd build/CMakeFiles/Tests.dir/tests
#find -name '*.c' -exec cp -t ../../../../$LOCATION {} +
#find -name '*.gcno' -exec cp -t ../../../../$LOCATION {} +
#find -name '*.gcda' -exec cp -t ../../../../$LOCATION {} +
#cd ../../../../$LOCATION
#rm main.c
#find -name '*.c' -exec gcov -bf {} \;

#gcov -o build/CMakeFiles/Tests.dir/src $(find src -name '*.c')