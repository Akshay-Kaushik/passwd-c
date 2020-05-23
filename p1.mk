a: server.o client.o
	gcc server.o client.o -o a
client.o: server.h client.c
	gcc -c client.c 
server.o: server.h server.c
	gcc -c server.c 

