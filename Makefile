all: main

main:
	gg++ --std=c++11 -DCPP -DGPP -I/ibona001/csshare/pkgs/csim_cpp-19.0/lib -m32 /ibona001/csshare/pkgs/csim_cpp-19.0/lib/csim.cpp.a -lm -o main main.cpp
	

