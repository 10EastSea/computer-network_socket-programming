all: server client

server: socket_server.c
	gcc -o server socket_server.c

client: socket_client.c
	gcc -o client socket_client.c

clean:
	rm server client