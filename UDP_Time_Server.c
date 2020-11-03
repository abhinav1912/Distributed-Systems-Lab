#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <time.h>

// define port  
#define COMM_PORT 8080 
#define BUFF_LEN 1024 
  
void main() { 
	// define socket
    int sfd, bind;
    // define server and client address
    struct sockaddr_in server_address, client_address;
    // message buffer
    char buffer[BUFF_LEN];
    char *hello = "Hello from server"; 
      
    // Creating socket
    sfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sfd<0){
        perror("Socket Failure"); 
        exit(0); 
    }
     
    memset(&server_address, 0, sizeof(server_address)); 
    memset(&client_address, 0, sizeof(client_address)); 
      
    // Assigning server values
    server_address.sin_port = htons(COMM_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
      
    // Server address bind to socket
    bind = bind(sfd, (const struct sockaddr *)&server_address, sizeof(server_address));
    if (bind<0) 
    { 
        perror("Binding Error"); 
        exit(0); 
    } 
    int n;
    int len = sizeof(client_address);

    // receive datagram from client
    n = recvfrom(sfd, (char *)buffer, BUFF_LEN, MSG_WAITALL, ( struct sockaddr *) &client_address, &len); 
    buffer[n] = '\0'; 
    printf("Message from Client: %s\n", buffer); 

    // check info received in datagram
    if (strcmp(buffer, "GET TIME\r\n") == 0){
    	current_time = time(NULL);   
	    // Send data back 
	    if (sendto(sd, (char *)&current_time, 
	         (int)sizeof(current_time), 0, 
	         (struct sockaddr *)&client, client_length) != 
	                                 (int)sizeof(current_time))
	    {
	        printf("Error sending datagram.\n");
	        exit(0);
	    }
	    printf("Current Time sent.\n")
    }  
} 