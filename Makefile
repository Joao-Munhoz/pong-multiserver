FLAGS=-lncurses -lpthread -lportaudio -std=c++11
CC=g++

all: server client

server: $(C_SOURCE) $(H_SOURCE)
	$(CC) -oserver src_server/*.cpp $(FLAGS)

client: $(C_SOURCE) $(H_SOURCE)
	$(CC) -oclient src_client/*.cpp $(FLAGS)

testserver:
	./server 2>/dev/null

testclient:
	./client 2>/dev/null

commit:
	git commit -am "Updates"
	git push

clean:
	rm client server

