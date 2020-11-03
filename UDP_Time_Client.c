#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT 8080 
#define BUFF 1024 
  
// Driver code 
void main() { 
    int sfd; 
    char buffer[BUFF]; 
    char *message = "GET TIME"; 
    struct sockaddr_in, servaddr; 
  
    // Creating socket
    sfd = socket(AF_INET, SOCK_DGRAM, 0)
    if (sfd<0) { 
        perror("Socket Error"); 
        exit(0); 
    } 
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Setting server variables
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    int n, len; 
      
    sendto(sfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("Time Request sent.\n"); 
    n = recvfrom(sfd, (char *)buffer, BUFF, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
    buffer[n] = '\0'; 
    printf("Server Response: %s\n", buffer); 
    close(sfd); 
} 