# rm -rf build
cmake -S . -B build -G Ninja
cmake --build build
mv build/WebServer WebServer
./WebServer