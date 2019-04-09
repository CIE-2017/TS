// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#define PORT 4333 
char * IP_server = "127.0.0.1";
int main(void) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char word[5120] = {0} ;
	char buffer[5120] = {0}; 
	//handshake
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, IP_server, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	send(sock , "handshake" , 9 , 0 ); 
	printf("message sent\n"); 
	valread = read( sock , buffer, sizeof(buffer));
	char *pb_key = buffer;
	strcpy(pb_key,buffer);
	printf("key: %s\n",pb_key);
	printf("Number of char: %d\n",valread);
	printf("%s\n",buffer ); 
	memset(&buffer, 0, sizeof(buffer));
	//end handshake
 
	while(1){
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, IP_server, &serv_addr.sin_addr)<=0) 
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
	unsigned char key[16];
	RAND_bytes(key,sizeof(key));
	unsigned char enc_out[80];
	AES_KEY enc_key, dec_key;

 	AES_set_encrypt_key(pb_key, 128, &enc_key);
 	AES_encrypt(key, enc_out, &enc_key);
	
	send(sock , key , strlen(key) , 0 ); 
	printf("message sent\n"); 
	valread = read( sock , buffer, sizeof(buffer));
	printf("Number of char: %d\n",valread);
	printf("%s\n",buffer ); 
	memset(&buffer, 0, sizeof(buffer)); 
	}
	return 0; 
} 

