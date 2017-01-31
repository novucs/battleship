# -*- MakeFile -*-

all: run

run: target/battleship.exe
	cd target && wine battleship.exe

target/battleship.exe: target/main.o
	x86_64-w64-mingw32-g++ target/main.o -o target/battleship.exe -lws2_32

target/main.o: src/main.cpp target
	x86_64-w64-mingw32-g++ -c src/main.cpp -o target/main.o -lws2_32

target:
	mkdir target

clean:
	rm -rf target
