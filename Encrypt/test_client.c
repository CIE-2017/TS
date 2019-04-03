// Client side C/C++ program to demonstrate Socket programming 
//http://hayageek.com/rsa-encryption-decryption-openssl-c/
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
#include <time.h>
#include <openssl/err.h>
#define PORT 5000 
char * IP_server = "127.0.0.1";

int padding = RSA_PKCS1_PADDING;

//generate random bytes
unsigned char *gen_rdm_bytestream (size_t num_bytes)
{
  unsigned char *stream = malloc (num_bytes);
  size_t i;

  for (i = 0; i < num_bytes; i++)
  {
    stream[i] = rand ();
  }

  return stream;
}
 
//RSA structure
RSA * createRSA(unsigned char * key,int public)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        printf( "Failed to create key BIO");
        return 0;
    }
    if(public)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA");
    }
 
    return rsa;
}

//encrypt from key 
int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

//decrypt from key
int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,0);
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}
 
//encrypt from key 
int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,0);
    int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}
//decrypt from key
int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,1);
    int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}

//print error
void printLastError(char *msg)
{
    char * err = malloc(130);;
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    printf("%s ERROR: %s\n",msg, err);
    free(err);
}

//main
int main(void) 
{ 
	//generate a key
	size_t key_size =10;
  	srand ((unsigned int) time (NULL));
  	char* key = gen_rdm_bytestream(key_size);

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
	char *pb_key;
	pb_key = malloc(sizeof(buffer));
	strcpy(pb_key,buffer);
	printf("key: %s\n",pb_key);
	printf("Number of char: %d\n",valread);
	printf("%s\n",buffer ); 
	memset(&buffer, 0, sizeof(buffer));
	//end handshake
 
	//send symethickey
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
	printf("\n\n");
	printf("%s",pb_key);
	printf("Key =%s\n", key);
	printf("%s",pb_key);
	unsigned char  encrypted[4098]={};
  	//unsigned char decrypted[4098]={};

  	int encrypted_length= public_encrypt(key,strlen(key),pb_key,encrypted);
  	if(encrypted_length == -1)
 	{
    	printLastError("Public Encrypt failed ");
    	exit(0);
  	}
  	printf("Encrypted Text =%s\n",encrypted);
  	printf("Encrypted length =%d\n",encrypted_length);

	send(sock , encrypted , encrypted_length , 0 ); 
	printf("message sent\n"); 
	valread = read( sock , buffer, sizeof(buffer));
	printf("Number of char: %d\n",valread);
	printf("%s\n",buffer ); 
	memset(&buffer, 0, sizeof(buffer));
	//symethic key sent
	// 
	return 0; 
} 

