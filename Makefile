build/tcp.o: lib/tcp.h src/tcp.cc
	g++ -c src/tcp.cc -o build/tcp.o -I ./
build/server.o: server.cc 
	g++ -c server.cc -o build/server.o
build/client.o: client.cc 
	g++ -c client.cc -o build/client.o
build/loop_server.o: loop_server.cc
	g++ -c loop_server.cc -o build/loop_server.o
build/poll.o: poll.cc
	g++ -c poll.cc -o build/poll.o

target/server: build/server.o build/tcp.o
	g++ build/server.o build/tcp.o -o target/server
target/client: build/client.o build/tcp.o
	g++ build/client.o build/tcp.o -o target/client
target/loop_server: build/loop_server.o build/tcp.o
	g++ build/loop_server.o build/tcp.o -o target/loop_server
target/poll: build/poll.o build/tcp.o
	g++ build/poll.o build/tcp.o -o target/poll
clean:
	rm build/*
target/poll_server: multiplex/server.cc build/tcp.o
	g++ -c multiplex/server.cc -o build/poll_server.o -I ./
	g++ build/poll_server.o build/tcp.o -o target/poll_server
target/poll_client: multiplex/client.cc build/tcp.o
	g++ -c multiplex/client.cc -o build/poll_client.o -I ./
	g++ build/poll_client.o build/tcp.o -o target/poll_client
