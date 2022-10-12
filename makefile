all: main

build: cleanall main

main: main.o shell.o filesys.o sdisk.o
	g++ main.o shell.o filesys.o sdisk.o -o sdisk

main.o: main.cpp shell.o
	g++ shell.o -c main.cpp

shell.o: shell.cpp
	g++ -c shell.cpp

filesys.o: filesys.cpp
	g++ -c filesys.cpp

sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp

clean:
	rm -f sdisk *.o

cleanall:
	rm -f sdisk *.o test1