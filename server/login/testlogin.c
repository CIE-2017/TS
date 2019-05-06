#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/rand.h>

void print_cryp(unsigned char *salt, size_t length)
{
   for(int i = 0; i < length; i++)
	{
    	printf("%02x", salt[i]);
	}
	printf("\n");
}


void string2hexString(char* input, char* output,size_t length)
{
    int i,j;
    for(i=0,j=0;i<length;i++,j+=2)
    { 
        sprintf((char*)output+j,"%02X",input[i]);
    }
    output[j]='\0'; /*adding NULL in the end*/
     
    printf("Hexadecimal converted string is: \n");
    printf("%s\n",output);
     
}

void to_ascii( unsigned char* dest, const unsigned char *text )
{
    unsigned int ch ;
    for( ; sscanf( (const char*)text, "%2x", &ch ) == 1 ; text += 2 )
          *dest++ = ch ;
    *dest = 0 ; // null-terminate
}

//http://forums.devx.com/showthread.php?173172-Convert-hex-string-to-ascii-string
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
	printf("%s\n",salt);	
	//print_cryp(salt,sizeof(salt));
	unsigned char mpassword="pawee2540";
	unsigned char pass[20];
	strcpy(pass, mpassword);
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, salt, strlen(salt));
	SHA256_Update(&sha256, pass, strlen(pass));
	SHA256_Final(hash, &sha256);
	printf("Hash: ");
	printf("%s\n",hash);	
	//print_cryp(hash,SHA256_DIGEST_LENGTH);
	char salt_h[33]={0};
	string2hexString(salt,salt_h,16);
	char hash_h[65]={0};
	string2hexString(hash,hash_h,SHA256_DIGEST_LENGTH);
	
	char salt_s[33]={0};
	to_ascii(salt_s,salt_h);
	char salt_e[33]={0};
	string2hexString(salt_s,salt_e,16);
    	//printf("String converted hext is:");
    	//printf("%s\n",salt_s);
	
	char hash_s[65]={0};
	to_ascii(hash_s,hash_h);
	char hash_e[65]={0};
	string2hexString(hash_s,hash_e,SHA256_DIGEST_LENGTH);
    	//printf("String converted hext is:");
    	//printf("%s\n",hash_s);
    	return 0;
}
