// Lab Assignment 3
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
 
void main() {

    int sockfd; // socket descriptor
    char buffer[LENGTH_LIMIT];
    struct sockaddr_in servaddr; // store server details
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket
    
    // check socket creation
    if ( sockfd < 0 ) {
        printf("Socket Creation Error");
        return;
    }
    memset(&servaddr, 0, sizeof(servaddr));

    // set details for the server
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    
    int n;
    socklen_t len = sizeof (struct sockaddr_in);

    int curr_time;
    printf("Enter current time (in 24H format) : ");
    scanf("%d", &curr_time);

    // set messages for communication
    char *time;
    time = malloc(1024);
    
    // store current time in outgoing message
    snprintf(time, sizeof(time), "%d", curr_time);
    
    // send the time to initiate communication
    sendto(sockfd, (const char *)time, strlen(time), 0, (const struct sockaddr *) &servaddr, len);
    n = recvfrom(sockfd, (char *)buffer, LENGTH_LIMIT, 0, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';

    // store the time adjustment
    int difference;
    sscanf(buffer, "%d", &difference);
    printf("Time Adjustment : %d\n", (-1)*difference);
    printf("Corrected Time : %d\n", curr_time-difference);

    // close socket descriptor
    close(sockfd);
    return;
}