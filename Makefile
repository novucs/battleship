# -*- MakeFile -*-

all: run

run: target/battleship.exe
	cd target && wine battleship.exe

target/battleship.exe: target/main.o target/ship.o target/connection.o
	x86_64-w64-mingw32-g++ target/main.o target/ship.o target/connection.o -o target/battleship.exe -lws2_32

target/main.o: src/main.cpp src/ship.hpp src/connection.hpp target
	x86_64-w64-mingw32-g++ -c src/main.cpp -o target/main.o -lws2_32

target/ship.o: src/ship.cpp src/ship.hpp target
	x86_64-w64-mingw32-g++ -c src/ship.cpp -o target/ship.o -lws2_32

target/connection.o: src/connection.cpp src/connection.hpp target
	x86_64-w64-mingw32-g++ -c src/connection.cpp -o target/connection.o -lws2_32

target:
	mkdir target

clean:
	rm -rf target
