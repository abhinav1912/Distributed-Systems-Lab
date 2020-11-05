// Lab Assignment 3
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

// predefine max number of processes as 15
#define PORT 8080
#define LENGTH_LIMIT 1024
#define PROCESS_LIMIT 15

// create array for storing client address and its time
struct sockaddr_in Client_List[PROCESS_LIMIT];
int Client_Time[PROCESS_LIMIT];

// create buffer to store messages
char buffer[LENGTH_LIMIT];
 
void main() {

    int sockfd; // socket file descriptor
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket

	//check if socket was successfully created
	if ( sockfd < 0 ) {
		printf("Socket Creation Failed");
		return;
	}

	struct sockaddr_in servaddr; // store server details
	memset(&servaddr, 0, sizeof(servaddr));

	// enter server details
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// bind socket to server address, check if it was successful
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 )
	{
		printf("Binding Error");
		return;
	}

	int total_time = 0; // to store the sum of all time values and compute difference
	printf("Enter current time : ");
	scanf("%d", &total_time);
	Client_Time[0] = total_time;
	int client_time, average; 
	
	// create struct to handle incoming clients
	struct sockaddr_in cliaddr;
	int n, process_count;
	socklen_t len = sizeof (struct sockaddr_in);
	printf("Enter total number of processes : ");
	scanf("%d", &process_count);

	int i = 1;
	// loop while connections are active
	while(i<process_count)
	{
		memset(&cliaddr, 0, sizeof(cliaddr));

		// receive data packet from client process
        n = recvfrom(sockfd, (char *)buffer, LENGTH_LIMIT, 0, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';

		// print the port and message sent by client
		printf("[+] Time received from node %d : %s \n", cliaddr.sin_port, buffer);
		
		// convert char buffer to int 
		sscanf(buffer, "%d", &client_time);

		// add client_time to total_time
		total_time += client_time;

		// store client details
		Client_List[i] = cliaddr;
		Client_Time[i] = client_time;

		// increment process counter
		i++;
	
	}

	// calculate average time
	average = total_time / process_count;

	// print time correction for current node
	printf("Time Adjustment : %d\n", average - Client_Time[0]);
	printf("Corrected Time: %d\n", average);
	
	i = 1;
	while(i<process_count)
	{
		// calculate the time adjustment for process i
		int difference = Client_Time[i] - average;
		cliaddr = Client_List[i];

		// buffer to store the response
		char response[LENGTH_LIMIT];

		// copy the response to the buffer
		snprintf(response, LENGTH_LIMIT, "%d", difference);
		
		// send the adjustment back to client
		sendto(sockfd, (const char *)response, strlen(response), 0, (const struct sockaddr *) &cliaddr, len);
		i++;
	}

	// close socket descriptor
	close(sockfd);
}