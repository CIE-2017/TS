#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 
#define PORT 5000 

int main(void) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char word[5120] = {0} ;
	char buffer[5120] = {0}; 

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	scanf("%s",word);
	send(sock , word , strlen(word) , 0 ); 
	printf("message sent\n"); 
	valread = read( sock , buffer, sizeof(buffer));
	printf("Number of char: %d\n",valread);
	printf("%s\n",buffer ); 
	return 0; 
} 

