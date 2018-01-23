@echo off
mkdir build
cd build

mkdir vc
cd vc

cmake -G "NMake Makefiles" ../..
cmake --build .

cd ../..
