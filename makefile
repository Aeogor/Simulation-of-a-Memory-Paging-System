all: vmpager run

clean:
	rm vmpager

vmpager: vmpager.cpp
	g++ -o vmpager vmpager.cpp

run: vmpager
	./vmpager Chevalier_473.nz 0 256
