project: process.o scheduler.o
	g++ process.o scheduler.o -o project.out
process.o: process.cpp process.h
	g++ -c process.cpp
sceduler.o: scheduler.cpp process.h
	g++ -c scheduler.cpp
clean:
	rm *.o *.out