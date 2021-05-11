build/tcp.o: lib/tcp.h src/tcp.cc
	g++ -c -g src/tcp.cc -o build/tcp.o -I ./
build/server.o: server.cc 
	g++ -c -g server.cc -o build/server.o
build/client.o: client.cc 
	g++ -c -g client.cc -o build/client.o
build/loop_server.o: loop_server.cc
	g++ -c -g loop_server.cc -o build/loop_server.o
build/poll.o: poll.cc
	g++ -c -g poll.cc -o build/poll.o

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


target/poll-server: server/poll.cc build/tcp.o
	g++ -c -g server/poll.cc -o build/poll-server.o -I ./
	g++ -g build/poll-server.o build/tcp.o -o target/poll-server

target/epoll-server: server/epoll.cc build/tcp.o
	g++ -c -g server/epoll.cc -o build/epoll-server.o -I ./
	g++ -g build/epoll-server.o build/tcp.o -o target/epoll-server

target/poll-server2: server/poll2.cc build/tcp.o
	g++ -c -g server/poll2.cc -o build/poll-server2.o -I ./
	g++ -g build/poll-server2.o build/tcp.o -o target/poll-server2

target/epoll-server2: server/epoll2.cc build/tcp.o
	g++ -c -g server/epoll2.cc -o build/epoll-server2.o -I ./
	g++ -g build/epoll-server2.o build/tcp.o -o target/epoll-server2
