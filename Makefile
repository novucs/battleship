# -*- MakeFile -*-

all: run

run: target/battleship.exe
	cd target && wine battleship.exe

target/battleship.exe: target/main.o
	x86_64-w64-mingw32-gcc target/main.o -o target/battleship.exe -lws2_32

target/main.o: src/main.c target
	x86_64-w64-mingw32-gcc -c src/main.c -o target/main.o -lws2_32

target:
	mkdir target

clean:
	rm -rf target
