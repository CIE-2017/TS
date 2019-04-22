#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>

//list of functions
int create_socket(int port);
void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);
void *myThread(void* input);
//////////////////////////


struct args {
    SSL* ssl;
    int client;
};

typedef struct node{
    SSL* ssl;
    struct node* before;
}node_t;

node_t* this_node;


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void print_list(node_t * head,char * buff) {
    pthread_mutex_lock(&mutex1);
    node_t * current = head;
    while (current != NULL) {
        //printf("%ld\n", current->ssl);
	SSL_write(current->ssl, buff, strlen(buff));
        current = current->before;
    }
    pthread_mutex_unlock(&mutex1);
}


//main//
int main(int argc, char **argv)
{
    //declare variable
    int sock;
    SSL_CTX *ctx;
    SSL *ssl;
    struct sockaddr_in addr;
    uint len = sizeof(addr);
    ////////////////////////////


    ///initiation
    init_openssl(); //initiate configuration
    ctx = create_context(); //create ctx
    configure_context(ctx);//configure the ctx
    sock = create_socket(4433);//create a normal socket
    ///////////////////
    
    /* Loop Handle connections */
    fprintf(stderr,"Simple echo TLS server\n");
    node_t * prev_node;
    prev_node = malloc(sizeof(node_t));
    prev_node = NULL;
    while(1) {
	//establish connection with client with ssl
        int client = accept(sock, (struct sockaddr*)&addr, &len); //block function
        if (client <= 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }
        ssl = SSL_new(ctx);
	SSL_set_fd(ssl, client);//sets the file descriptor fd as the input/output facility for the TLS/SSL (encrypted) side of ssl. fd 	
	SSL_accept(ssl);
	///////////////////

	// Start a new thread
	struct args *argument = (struct args *)malloc(sizeof(struct args));
	argument->ssl = ssl;
	argument->client = client;
	//SSL_write(ssl,"sfdsf",strlen("sfdsf"));
    	node_t * new_node;
    	new_node = malloc(sizeof(node_t));
	this_node = new_node;
	new_node->ssl = ssl;
	new_node->before = prev_node;   	
	pthread_t thread_id;
    	pthread_create(&thread_id, NULL, myThread, (void *)argument);
	/////////////////////
	prev_node = new_node;
	//SSL_write(ssl,"sfdsf",strlen("sfdsf"));
    }
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}


//------functions-------//

void *myThread(void * input) 
{  		
		char *buff;
		buff = calloc(2048,sizeof(char));
		SSL* ssl = ((struct args*)input)->ssl;
	        int client = ((struct args*)input)->client;	
		
		while(1){
				node_t * current = this_node;
				if(SSL_read(ssl,buff,2048)>0){
	        		fprintf(stderr,"read:%s\n",buff);
            			//SSL_write(ssl, buff, strlen(buff)); 
    				print_list(this_node,buff);
				//SSL_write(ssl, buff, strlen(buff));
	        		memset(buff,0,2048); 
				}

		}
        	SSL_free(ssl);
		close(client);
} 

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
