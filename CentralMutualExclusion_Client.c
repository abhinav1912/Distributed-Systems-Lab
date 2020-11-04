// Lab Assignment 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
#define PORT 8080
#define LENGTH_LIMIT 1024
 
int main(int argc, char* argv[]) {

    // here argv[1] is the requested resource

    int sockfd; // socket descriptor
    char buffer[LENGTH_LIMIT];
    struct sockaddr_in servaddr; // store server details
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket
    
    // check socket creation
    if ( sockfd < 0 ) {
        printf("Socket Error");
        return 0;
    }
    memset(&servaddr, 0, sizeof(servaddr));

    // set details for the server
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    
    int n;
    socklen_t len = sizeof (struct sockaddr_in);

    // set messages for communication
    char *request, *hello = "PING", *release="RELEASE";
    request = malloc(1024 + 8);

    // add requested resource number to REQUEST message
    strcpy(request, "REQUEST_");
    strcat(request, argv[1]);

    // send a PING message to initiate communication
    sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, len);
    n = recvfrom(sockfd, (char *)buffer, LENGTH_LIMIT, 0, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';

    // Check if requested resource can be accessed or not
    if(!strcmp(buffer, "OK"))
    {
        sendto(sockfd, (const char *)request, strlen(request), 0, (const struct sockaddr *) &servaddr, len);
        n = recvfrom(sockfd, (char *)buffer, LENGTH_LIMIT, 0, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';

        // resource in use
        if(!strcmp(buffer, "DENIED"))
        {
            printf("Request denied\n");
            n = recvfrom(sockfd, (char *)buffer, LENGTH_LIMIT, 0, (struct sockaddr *) &servaddr, &len);
            buffer[n] = '\0';
        }

        //resource available to use
        printf("Performing critical section\n");
        // using resource for 15 seconds
        sleep(15);

        // release resource
        sendto(sockfd, (const char *)release, strlen(release), 0, (const struct sockaddr *) &servaddr, len);
    }

    // close socket descriptor
    close(sockfd);
    return 0;
}