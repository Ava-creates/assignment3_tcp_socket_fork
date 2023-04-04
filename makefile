TARGET=client server 

CFLAGS= -Wall -Wextra -g
normal: $(TARGET)
client: client.c
	gcc $(CFLAGS) client.c -o client
server: server.c
	gcc $(CFLAGS) server.c -o server


#reference to the article i used - https://stackoverflow.com/questions/48261680/how-can-i-create-makefile-for-gcc-on-a-server-client-program