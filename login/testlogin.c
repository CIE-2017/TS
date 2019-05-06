#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/rand.h>

void print_cryp(unsigned char 24173c85d7474db7cb89fff0b29c88c60e777b0d71ad1b79621e6e4df4469a65 *salt, size_t length)
{
   for(int i = 0; i < length; i++)
	{
    	printf("%02x", salt[i]);
	}
	printf("\n");
}

int main()
{
	unsigned char id[] = "paweemew";
	unsigned char password[] = "pawee2540";
	
	//create salt with size of 16 bytes
        unsigned char salt[16];    //32 is just an example  
	int rb = RAND_bytes(salt,16);
	printf("Id:%s\n",id);
	printf("Pass:%s\n",password);
	printf("Salt: ");
	print_cryp(salt,sizeof(salt));
	unsigned char pass[20];
	strcpy(pass, password);
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, salt, strlen(salt));
	SHA256_Update(&sha256, pass, strlen(pass));
	SHA256_Final(hash, &sha256);
	printf("Hash: ");
	print_cryp(hash,SHA256_DIGEST_LENGTH);
    	return 0;
}
