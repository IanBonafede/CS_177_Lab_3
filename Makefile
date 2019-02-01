all: main

main:
	g++ --std=c++11 -DCPP -DGPP -I/usr/csshare/pkgs/csim cpp-19.0/lib -m32
	

clean:
	rm -rf *.exe
