#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>
#include <time.h>

//list of functions
int create_socket(int port);
void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);
void *myThread(void *input);
void broadcast(SSL **array, char *buff);
//////////////////////////

struct args
{
    SSL *ssl;
    int no;
    int client;
};

SSL *ssl_array[4];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

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
    init_openssl();             //initiate configuration
    ctx = create_context();     //create ctx
    configure_context(ctx);     //configure the ctx
    sock = create_socket(4433); //create a normal socket
    ///////////////////

    /* Loop Handle connections */
    fprintf(stderr, "Simple echo TLS server\n");
    while (1)
    {
        //establish connection with client with ssl
        int client = accept(sock, (struct sockaddr *)&addr, &len); //block function
        if (client <= 0)
        {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client); //sets the file descriptor fd as the input/output facility for the TLS/SSL (encrypted) side of ssl. fd
        SSL_accept(ssl);
        ///////////////////

        // Start a new thread
        struct args *argument = (struct args *)malloc(sizeof(struct args));
        argument->ssl = ssl;
        argument->client = client;
        //argument->no = i;
    
        int i = 0; 
        int exit = 1;
        while (exit&&i<5)
        {
            if (ssl_array[i] == NULL)
            {
                ssl_array[i] = ssl;
                argument->no = i;
                exit = 0;
            }
            i++;
        }
        for(int i=0;i<5;i++){
            fprintf(stderr, "client:%ld\n", ssl_array[i]);
        }    
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, myThread, (void *)argument);
        /////////////////////
        i++;
    }
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}

//------functions-------//

void broadcast(SSL **array, char *buff)
{
    pthread_mutex_lock(&mutex1);
    for (int i = 0; i < 5; i++)
    {
        if (array[i] != NULL)
        {
            SSL_write(array[i], buff, strlen(buff));
        }
    }
    pthread_mutex_unlock(&mutex1);
}

void *myThread(void *input)
{
    char *buff;
    buff = calloc(2048, sizeof(char));

    SSL *ssl = ((struct args *)input)->ssl;
    int client = ((struct args *)input)->client;
    int no = ((struct args *)input)->no;

    int upper = 100;
    int lower = 1;
    char user_id[10];
    char pos_x[10];
    char pos_z[10];
    srand(time(0));
    int num = (rand() % (upper - lower + 1)) + lower;
    int p_x = (rand() % 9);
    int p_z = (rand() % 9);

    sprintf(user_id, "%d", num);
    sprintf(pos_x, "%d", p_x);
    sprintf(pos_z, "%d", p_z);

    SSL_write(ssl, user_id, strlen(user_id)); //send Id
    SSL_write(ssl, pos_x, strlen(pos_x));     //send pos X
    SSL_write(ssl, pos_z, strlen(pos_z));     //send pos Z

    while (1)
    {
        if (SSL_read(ssl, buff, 2048) > 0)
        {
            fprintf(stderr, "read:%s\n", buff);
            if (strcmp(buff, "exit") == 0)
            {
                ssl_array[no] = NULL;
            }
            //SSL_write(ssl, buff, strlen(buff));
            else
            {
                broadcast(ssl_array, buff);
            }
            memset(buff, 0, 2048);
        }
    }

    fprintf(stderr, "end\n");
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
    if (s < 0)
    {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0)
    {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return s;
}

void init_openssl()
{
    SSL_load_error_strings();     //Initializes the internal OpenSSL table of error descriptions.
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
    SSL_CTX *ctx; //Represents a factory of SSL objects

    method = SSLv23_server_method(); //use the method that makes the peers negotiate the highes version supported by both

    ctx = SSL_CTX_new(method); //Allocate a new SSL factoru implementing
    if (!ctx)
    {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1); //set the supported curves fot ctx to clistlen

    /* Set the key and cert from file */
    if (SSL_CTX_use_certificate_file(ctx, "./pem/cert.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "./pem/key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}
