main: main.o eventDispatcher.o mutex.o socket.o netlib.o proxyConnection.o commonConnection.o utils.o
	g++ main.o eventDispatcher.o mutex.o socket.o netlib.o proxyConnection.o commonConnection.o utils.o \
	-o main -std=c++11


main.o: main.cpp
	g++ -c main.cpp -std=c++11

eventDispatcher.o: eventDispatcher.cpp
	g++ -c eventDispatcher.cpp  -std=c++11

mutex.o: mutex.cpp
	g++ -c mutex.cpp -std=c++11

socket.o: socket.cpp
	g++ -c socket.cpp -std=c++11

netlib.o: netlib.cpp
	g++ -c netlib.cpp -std=c++11

proxyConnection.o: proxyConnection.cpp
	g++ -c proxyConnection.cpp -std=c++11

commonConnection.o: commonConnection.cpp
	g++ -c commonConnection.cpp -std=c++11

utils.o: utils.cpp
	g++ -c utils.cpp -std=c++11
clean:
	rm -rf ./*.o
