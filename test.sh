cmake -S . -B build -G Ninja -DBUILD_TESTS=ON
cmake --build build
mv build/Tests Tests
./Tests