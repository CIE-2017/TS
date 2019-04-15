#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

//list of functions
int create_socket(int port);
void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);


//main//
int main(int argc, char **argv)
{
    int sock;
    SSL_CTX *ctx;

    init_openssl(); //initiate configuration
    ctx = create_context(); //create ctx

    configure_context(ctx);//configure the ctx
    sock = create_socket(4433);//create a normal socket
    fprintf(stderr,"Simple echo TLS server\n");
    /* Handle connections */
    while(1) {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "testeieizaza\n";
	char *buff;
	buff = calloc(2048,sizeof(char));
        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);//sets the file descriptor fd as the input/output facility for the TLS/SSL (encrypted) side of ssl. fd 

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            while(SSL_accept(ssl)>0)
            {   
 	        SSL_read(ssl,buff,2048);
	        fprintf(stderr,"read:%s\n",buff);
            	SSL_write(ssl, buff, strlen(buff));
	        memset(buff,0,2048);
            }
        }
        printf("done");
        SSL_free(ssl);
        close(client);
    }
    printf("laew");
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}

//------functions-------//
int create_socket(int port)
{
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	perror("Unable to create socket");
	exit(EXIT_FAILURE);
    }

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
	perror("Unable to bind");
	exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
	perror("Unable to listen");
	exit(EXIT_FAILURE);
    }

    return s;
}

void init_openssl()
{ 
    SSL_load_error_strings(); //Initializes the internal OpenSSL table of error descriptions.
    OpenSSL_add_ssl_algorithms(); //no need for OpenSSL 1.1.0
    //SSL_library_init();//same as OpenSSL_add_ssl_algorithms
}

//no need for OpenSSL 1.1.0
void cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;//Represents a factory of SSL objects

    method = SSLv23_server_method();//use the method that makes the peers negotiate the highes version supported by both

    ctx = SSL_CTX_new(method);//Allocate a new SSL factoru implementing
    if (!ctx) {
	perror("Unable to create SSL context");
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);//set the supported curves fot ctx to clistlen

    /* Set the key and cert from file */
    if (SSL_CTX_use_certificate_file(ctx, "./pem/cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "./pem/key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }
}
