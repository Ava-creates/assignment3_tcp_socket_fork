#include <stdio.h> // header for input and output from console : printf, perror
#include<string.h> // strcmp
#include<sys/socket.h> // for socket related functions
#include<arpa/inet.h> // htons
#include <netinet/in.h> // structures for addresses

#include<unistd.h> // contains fork() and unix standard functions
#include<stdlib.h>


#include<unistd.h> // header for unix specic functions declarations : fork(), getpid(), getppid()
#include<stdlib.h> // header for general fcuntions declarations: exit()


int main()
{
	//socket
	int server_sd = socket(AF_INET,SOCK_STREAM,0);
	if(server_sd<0)
	{
		perror("socket:");
		exit(-1);
	}

	struct sockaddr_in my_addr;
	char myIP[16];
    unsigned int myPort;
	//setsock
	int value  = 1;
	setsockopt(server_sd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value)); //&(int){1},sizeof(int)
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY, INADDR_LOOP

	//connect
    if(connect(server_sd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        perror("connect");
        exit(-1);
    }

	// Get my ip address and port
    bzero(&my_addr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
    getsockname(server_sd, (struct sockaddr *) &my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
    myPort = ntohs(my_addr.sin_port);
	//printf("%d\n", myPort);
	//accept
	char buffer[256];

	while(1)
	{
	   printf("Enter message : ");
       fgets(buffer,sizeof(buffer),stdin);
       buffer[strcspn(buffer, "\n")] = 0;  //remove trailing newline char from buffer, fgets does not remove it
       if(strcmp(buffer,"bye")==0)
        {
        	printf("closing the connection to server \n");
			send(server_sd,buffer,strlen(buffer), 0);
        	close(server_sd);
            break;
        }
        if(send(server_sd,buffer,strlen(buffer),0)<0)   //sending the message to server 
        {
            perror("send");
            exit(-1);
        }
        
		if(recv(server_sd,buffer,sizeof(buffer),0)>=0)	  //receiving the echo from server and printing it
		{
			printf("Server response: %s\n", buffer);
			bzero(buffer,sizeof(buffer));		
		}
		else
		{
			printf("ERROR!!\n");
		}
	}
	
	return 0;
}
