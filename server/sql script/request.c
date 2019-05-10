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

size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return (0); //success
}

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

void to_ascii( unsigned char* dest, const unsigned char *text )
{
    unsigned int ch ;
    for( ; sscanf( (const char*)text, "%2x", &ch ) == 1 ; text += 2 )
          *dest++ = ch ;
    *dest = 0 ; // null-terminate
}
int main(int argc, char **argv)
{ 
   int status = 0;  
   char *server = "35.240.157.83";
   char *user = "paweemew";
   char *password = "pawee2540"; /* set me first */
   char *database = "game";  
   MYSQL *con = mysql_init(NULL);  
  unsigned char id[20];
	printf("Login\nInsertID: ");
	scanf("%[^\n]%*c", id);
  char pbuffer[200];
memset(pbuffer,0,sizeof(pbuffer));
snprintf(pbuffer, sizeof(pbuffer), "SELECT Salt FROM login WHERE Username='%s'",id);


  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
 
  if (mysql_real_connect(con, server, user, password, 
          database, 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL) 
  {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, pbuffer)) 
  {
      finish_with_error(con);
  }
  MYSQL_ROW this_salt;
  
  do {  
      MYSQL_RES *result = mysql_store_result(con);
        
      if (result == NULL) 
      {
          finish_with_error(con);
      }
            
      this_salt = mysql_fetch_row(result);
   
      mysql_free_result(result);
                 
      status = mysql_next_result(con); 
     
      if (status > 0) {
          finish_with_error(con);
      }
      
  } while(status == 0);
  printf("%s\n",this_salt[0]);
  char* base64DecodeOutput;
  size_t test;
  Base64Decode(this_salt[0], &base64DecodeOutput, &test);
  printf("Output: %s %d\n", base64DecodeOutput, test);
	
  	unsigned char tpassword[20];
	printf("InsertPass: ");
	scanf("%[^\n]%*c", tpassword);
  char pass[20];
  strcpy(pass,tpassword);
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, base64DecodeOutput, strlen(base64DecodeOutput));
	SHA256_Update(&sha256, pass, strlen(pass));
	SHA256_Final(hash, &sha256);
  //printf("thishash%s",hash);
  char *hash_h;	
  Base64Encode(hash, SHA256_DIGEST_LENGTH, &hash_h); 
  printf("hash: %s\n",hash_h);
 memset(pbuffer,0,sizeof(pbuffer));
snprintf(pbuffer, sizeof(pbuffer), "SELECT hash FROM login WHERE Username='%s'",id);
  if (mysql_query(con, pbuffer)) 
  {
      finish_with_error(con);
  }
  MYSQL_ROW this_hash;
  
  do {  
      MYSQL_RES *result = mysql_store_result(con);
        
      if (result == NULL) 
      {
          finish_with_error(con);
      }
            
      this_hash = mysql_fetch_row(result);
   
      mysql_free_result(result);
                 
      status = mysql_next_result(con); 
     
      if (status > 0) {
          finish_with_error(con);
      }
      printf("hash: %s\n",this_hash[0]);
  } while(status == 0);
		//printf("this:%s",this_hash[0]);
      int res = strcmp(this_hash[0],hash_h);
      if(res==0){
		printf("Right Pass");	
	}else{
		printf("Wrong Pass");		
	}
}
