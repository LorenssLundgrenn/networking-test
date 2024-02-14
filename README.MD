using mingw32 with cmake:
create dir ./build
cmake -S ./ -B ./build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++