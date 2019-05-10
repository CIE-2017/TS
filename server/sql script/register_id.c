#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

//Encodes Base64
int Base64Encode(const unsigned char* buffer, size_t length, char** b64text) { //Encodes a binary safe base 64 string
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*b64text=(*bufferPtr).data;

	return (0); //success
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
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

int main(int argc, char **argv)
{
   char *server = "35.240.157.83";
   char *user = "paweemew";
   char *mypassword = "pawee2540"; /* set me first */
   char *database = "game";
  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con,server, user, mypassword, 
          database, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    

	unsigned char id[20];
	printf("Create ID\n");
	printf("InputID: ");
	scanf("%[^\n]%*c", id);
	printf("InputPassword: ");
	unsigned char password[20];
	scanf("%[^\n]%*c", password);
	unsigned char salt[17];    //32 is just an example  
	int rb = RAND_bytes(salt,16);
	salt[17]='\0';
	printf("Id:%s\n",id);
	printf("Pass:%s\n",password);
	printf("Salt:%s\n",salt);
	//print_cryp(salt,sizeof(salt));
	unsigned char pass[20];
	strcpy(pass, password);
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, salt, strlen(salt));
	SHA256_Update(&sha256, pass, strlen(pass));
	SHA256_Final(hash, &sha256);
	//printf("Hash: ");
	//print_cryp(hash,SHA256_DIGEST_LENGTH);
	//char sallt_h[33];	
	char *salt_h;
	//string2hexString(salt,salt_h,16);
    	Base64Encode(salt, strlen(salt), &salt_h);
	//char hash_h[65]={0};
	char *hash_h;	
	Base64Encode(hash, SHA256_DIGEST_LENGTH, &hash_h);

//string2hexString(hash,hash_h,SHA256_DIGEST_LENGTH);
	
	
char buffer[200];
memset(buffer,0,sizeof(buffer));
snprintf(buffer, sizeof(buffer), "INSERT INTO login(Username,Salt,Hash) VALUES('%s','%s','%s')",id,salt_h,hash_h);
printf("\n\n\n%s\n\n\n",buffer);  
  
if (mysql_query(con,buffer)) {
      finish_with_error(con);
  }
  mysql_close(con);
  exit(0);
}
