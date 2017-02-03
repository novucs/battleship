# -*- MakeFile -*-

run: target/battleship.exe
	cd target && wine battleship.exe

target/battleship.exe: target target/bot-master.o target/bot-zombie.o target/connection.o target/main.o target/network_manager.o target/ship.o
	x86_64-w64-mingw32-g++ target/bot-master.o target/bot-zombie.o target/connection.o target/main.o target/network_manager.o target/ship.o -o target/battleship.exe -lws2_32

target/bot-master.o: src/bot-master.cpp src/bot.hpp src/network_manager.hpp
	x86_64-w64-mingw32-g++ -c src/bot-master.cpp -o target/bot-master.o -lws2_32

target/bot-zombie.o: src/bot-zombie.cpp src/bot.hpp src/network_manager.hpp
	x86_64-w64-mingw32-g++ -c src/bot-zombie.cpp -o target/bot-zombie.o -lws2_32

target/connection.o: src/connection.cpp src/connection.hpp target
	x86_64-w64-mingw32-g++ -c src/connection.cpp -o target/connection.o -lws2_32

target/main.o: src/main.cpp src/bot.hpp target
	x86_64-w64-mingw32-g++ -c src/main.cpp -o target/main.o -lws2_32

target/network_manager.o: src/network_manager.cpp src/network_manager.hpp src/settings.hpp target
	x86_64-w64-mingw32-g++ -c src/network_manager.cpp -o target/network_manager.o -lws2_32

target/ship.o: src/ship.cpp src/ship.hpp target
	x86_64-w64-mingw32-g++ -c src/ship.cpp -o target/ship.o -lws2_32

target:
	mkdir target

clean:
	rm -rf target
