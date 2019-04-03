#include <string.h>
#include <stdio.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
//https://www.codepool.biz/how-to-use-openssl-generate-rsa-keys-cc.html

 unsigned char key[16];
//char key[] = "thisisasecretkey";

int main(){
 RAND_bytes(key,sizeof(key));

int i;

for (i = 0; i < sizeof(key); i++)
{
    if (i > 0) printf(":");
    printf("%02X", key[i]);
}

 printf("\n");
 unsigned char text[]="hello world";
 unsigned char enc_out[80];
 unsigned char dec_out[80];

 AES_KEY enc_key, dec_key;

 AES_set_encrypt_key(key, 128, &enc_key);
 AES_encrypt(text, enc_out, &enc_key);

 printf("original:%s\t",text);
 printf("\nencrypted:%s\t",enc_out);
 printf("\n");

 return 0;
}
