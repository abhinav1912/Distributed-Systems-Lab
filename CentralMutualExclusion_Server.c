// Lab Assignment 2
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

// predefine resource count as 2, number of processes as 15
#define PORT 8080
#define RESOURCES 2
#define LENGTH_LIMIT 1024
#define PROCESS_LIMIT 15

// create queue for managing resource requests by processes
struct sockaddr_in cs_queue[RESOURCES][PROCESS_LIMIT], resource_holder[RESOURCES];
int queue_front[RESOURCES]={0}, queue_back[RESOURCES]={0};

// create buffer to store messages
char buffer[LENGTH_LIMIT];

// create mutex lock for exclusion
int CS_MUTEX[RESOURCES] = {0};

// create function to add process to queue
void enqueue(struct sockaddr_in client, int resource_id)
{
	if(queue_back[resource_id] == PROCESS_LIMIT)
	{
		printf("Queue is full\n");
		return;
	}
	cs_queue[resource_id][queue_back[resource_id]] = client;
	queue_back[resource_id]++;
}

// create function to remove process from queue
struct sockaddr_in dequeue(int resource_id)
{
	struct sockaddr_in top;
	if(queue_back == queue_front)
	{
		printf("Queue is empty");
		return top;
	}
	top = cs_queue[resource_id][queue_front[resource_id]];
	queue_front[resource_id]++;
	return top;
}

// check if queue is empty
int queue_empty(int resource_id)
{
	if (queue_back[resource_id] == queue_front[resource_id]) return 1;
	return 0;
}
 
void main() {
	printf("[+] Server listening at port 8080\n");

    int sockfd; // socket file descriptor
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket

	//check if socket was successfully created
	if ( sockfd < 0 ) {
		printf("Socket Failed");
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

	// create struct to handle incoming clients
	struct sockaddr_in cliaddr;
	int n;
	socklen_t len = sizeof (struct sockaddr_in);

	// loop while connections are active
	while(1)
	{
		memset(&cliaddr, 0, sizeof(cliaddr));

		// receive data packet from client process
        n = recvfrom(sockfd, (char *)buffer, LENGTH_LIMIT, 0, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
		char response[LENGTH_LIMIT];

		// print the port and message sent by client
		printf("%d : %s \n", cliaddr.sin_port, buffer);

		// if connection test, reply OK
		if(!strcmp(buffer, "PING"))
		{
			strcpy(response, "OK");
		}

		// if resource request
		else if(!strncmp(buffer, "REQUEST", 7))
		{
			int resource_id = buffer[8] - '0';

			// check if resource is free
			if(CS_MUTEX[resource_id] == 0)
			{
				CS_MUTEX[resource_id] = 1;
				strcpy(response, "GRANTED");
				printf("%d : GRANTED RESOURCE %d \n", cliaddr.sin_port, resource_id);
				resource_holder[resource_id] = cliaddr;
			}

			// add process to queue, return DENIED 
			else
			{
				enqueue(cliaddr, resource_id);
				strcpy(response, "DENIED");
				printf("%d : DENIED RESOURCE %d \n", cliaddr.sin_port, resource_id);
			}
		}

		// if releasing already occupied resource
		else if(!strncmp(buffer, "RELEASE", 7))
		{
			int resource_id = buffer[8] - '0';

			// check if any other resource is queued to use the same resource
			if(queue_empty(resource_id)) CS_MUTEX[resource_id] = 0;
			
			else{

				// assign the resource to the topmost process in the queue
				struct sockaddr_in queue_top = dequeue(resource_id);
				strcpy(response, "GRANTED");
				sendto(sockfd, (const char *)response, strlen(response), 0, (const struct sockaddr *) &queue_top, len);
				resource_holder[resource_id] = queue_top;
			}

			// send an acknowledgement as response
			strcpy(response, "ACK");
		}

		// erraneous messaged received
		else
		{
			strcpy(response, "ERROR");
		}

		// send response back to client process
		sendto(sockfd, (const char *)response, strlen(response), 0, (const struct sockaddr *) &cliaddr, len);
	}

	// close socket descriptor
	close(sockfd);
}