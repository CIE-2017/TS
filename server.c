#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 5000 
int main() 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int addrlen = sizeof(address); 
	char buffer[5120] = {0}; 
	char *word = "received from server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 5000 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 5000 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT);
	//listen
	if (listen(server_fd, 10) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	puts("Waiting for connections ..."); 
	while(1){
	//accept
	memset(buffer,0,sizeof(buffer));
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept");          
		exit(EXIT_FAILURE); 
	} 

	//send,receive
	while(new_socket>0){
	valread = read( new_socket , buffer, sizeof(buffer));
	printf("Character count: %d\n",valread); 
	printf("Message: %s\n",buffer ); 
	send(new_socket , word , strlen(word) , 0 ); 
	printf("message sent to client\n"); 
			}
	}
	return 0; 
} 

int tryGetLock( char const *lockName)
{
    mode_t m = umask( 0 );
    int fd = open( lockName, O_RDWR|O_CREAT, 0666 );
    umask( m );
    printf("Opened the file. Press enter to continue...");
    fgetc(stdin);
    printf("Continuing by acquiring the lock.\n");
    if( fd >= 0 && flock( fd, LOCK_EX | LOCK_NB ) < 0 )
    {
        close( fd );
        fd = -1;
    }
    return fd;
}

static const char *lockfile = "/tmp/mylock.lock";

int main(int argc, char *argv[0])
{
    int lock = tryGetLock(lockfile);
    if (lock == -1) {
        printf("Getting lock failed\n");
        return 1;
    }

    printf("Acquired the lock. Press enter to release the lock...");
    fgetc(stdin);

    printf("Releasing...");
    releaseLock(lock, lockfile);
    printf("Done!\n");
    return 0;
