
CXXFLAGS = -g -Wall
PROGRAMS = icosa knot plane sphere teapot torus

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)
	rm -f core *~

build:
	./torus -s 10 -r 10 > ../torus10.obj
	./torus -s 20 -r 20 > ../torus20.obj
	./torus -s 50 -r 50 > ../torus50.obj
	./torus -s 100 -r 100 > ../torus100.obj
	./sphere -s 10 -r 10 > ../sphere10.obj
	./sphere -s 20 -r 20 > ../sphere20.obj
	./sphere -s 50 -r 50 > ../sphere50.obj
	./sphere -s 100 -r 100 > ../sphere100.obj
	./plane -w 1 -h 1 > ../plane1.obj
	./plane -w 5 -h 5 > ../plane5.obj
	./plane -w 10 -h 10 > ../plane10.obj
	./plane -w 100 -h 100 > ../plane100.obj
	./icosa -d 2 > ../icosa2.obj
	./icosa -d 4 > ../icosa4.obj
	./icosa -d 6 > ../icosa6.obj
	./knot -u 25 -v 25 > ../knot25.obj
	./knot -u 50 -v 50 > ../knot50.obj
	./knot -u 100 -v 100 > ../knot100.obj
	./knot -u 25 -v 25 -a 1.50 -b 0.75 -c 0.75 -d 0.60 > ../knot25a.obj
	./knot -u 50 -v 50 -a 1.50 -b 0.75 -c 0.75 -d 0.60 > ../knot50a.obj
	./knot -u 100 -v 100 -a 1.50 -b 0.75 -c 0.75 -d 0.60 > ../knot100a.obj
	./teapot -u 10 -v 10 > ../teapot10.obj
	./teapot -u 25 -v 25 > ../teapot25.obj
	./teapot -u 50 -v 50 > ../teapot50.obj
