build/tcp.o: lib/tcp.h src/tcp.cc
	g++ -c src/tcp.cc -o build/tcp.o

target/server: server.cc build/tcp.o
	g++ -c server.cc -o build/server.o
	g++ build/server.o build/tcp.o -o target/server
target/client: client.cc build/tcp.o
	g++ -c client.cc -o build/client.o
	g++ build/client.o build/tcp.o -o target/client

clean:
	rm build/*
