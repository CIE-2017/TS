#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/rand.h>

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
  
  if (mysql_query(con, "SELECT Salt FROM login WHERE Username='paweemew'")) 
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
  
    if (mysql_query(con, "SELECT hash FROM login WHERE Username='paweemew'")) 
  {
      finish_with_error(con);
  }
//////  
	printf("%s\n", this_salt[0]);

	char salt_s[33]={0};
	to_ascii(salt_s,this_salt[0]);
	unsigned char mpassword="pawee2540";
	unsigned char pass[20];
	strcpy(pass, mpassword);
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, salt_s, strlen(salt_s));
	SHA256_Update(&sha256, pass, strlen(pass));
	SHA256_Final(hash, &sha256);
	printf("Hash: ");
	printf("%s\n",hash);	
	char hash_h[65]={0};
	string2hexString(hash,hash_h,SHA256_DIGEST_LENGTH);
	
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
      
  } while(status == 0);
      printf("%s\n", this_hash[0]);
  mysql_close(con);  
  exit(0);
}
