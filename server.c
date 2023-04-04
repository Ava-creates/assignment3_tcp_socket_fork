#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <netinet/in.h>

#include<unistd.h>
#include<stdlib.h>

int main()
{
	//socket
	int server_sd = socket(AF_INET,SOCK_STREAM,0);
	if(server_sd<0)
	{
		perror("socket:");
		exit(-1);
	}
	//setsock
	int value  = 1;
	setsockopt(server_sd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value)); //&(int){1},sizeof(int)
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY, INADDR_LOOP
	struct sockaddr_in my_addr;
	char myIP[16];
    unsigned int myPort;
	socklen_t len = sizeof(my_addr);


	//bind
	if(bind(server_sd, (struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		perror("bind failed");
		exit(-1);
	}
	//listen
	if(listen(server_sd,5)<0)
	{
		perror("listen failed");
		close(server_sd);
		exit(-1);
	}

	struct sockaddr_in client_addr;
	socklen_t addr_size;

	printf("Server is listening...\n");
	while(1)
	{
		//accept
		int client_sd = accept(server_sd,&client_addr, &addr_size);
		
		printf("[%s:%d] Connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		int pid = fork(); //fork a child process
		if(pid == 0)   //if it is the child process
		 {
		 	close(server_sd); //close the copy of server/master socket in child process
		 	char buffer[256];
			while(1)
			{
				bzero(buffer,sizeof(buffer));
				int bytes = recv(client_sd,buffer,sizeof(buffer),0);  //receiveing the message from client
				if(bytes==0 || strcmp(buffer,"bye")==0)   //client has closed the connection
				{
					printf("[%s:%d] Disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					close(client_sd);
					exit(1); // terminate client program
				}


				printf("[%s:%d] Received message: %s\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
				send(client_sd, buffer, strlen(buffer), 0);  //sending the received message back to the client 
				printf("[%s:%d] Sent message: %s\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),  buffer);
				
			}
		 }
		 else //if it is the parent process
		 {
		 	close(client_sd); //close the copy of client/secondary socket in parent process 
		 }
	}


	//recv/send

	//close
	close(server_sd);
	return 0;
}
