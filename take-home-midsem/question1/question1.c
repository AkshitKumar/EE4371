/* 
	Name : Akshit Kumar
	Roll Number : EE14B127
	Question : Code a FIFO queue into which all the packets are inserted as they arrive
*/

// Including the necessary libraries 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Defining the maximum queue size
#define MAX_QUEUE_SIZE 10000

// Defination of the Packet Structure
typedef struct PACKET{
	int type; // type = 0 for HTTP Packets and type = 1 for Video Packets
	float arrivalTime; // contains the arrival time of the packet in seconds
	int size; // size = 80 for HTTP Packet and size = 400 for Video Packet
}PACKET;

// Helper Functions and Variables for implementation of the Circular Queue
PACKET queue[MAX_QUEUE_SIZE]; // Declaring the Circular Queue with size of 10000
int front = -1, rear = -1; // Initialising the front and rear variables
int videoPacketInserted = 0; // Count of Video Packets Inserted in the simulations
int HTTPPacketInserted = 0; // Count of HTTP Packets Inserted in the simulations
// Check if the queue is full or not
bool isQueueFull(){
	if((front == rear+1) || (front == 0 && rear == MAX_QUEUE_SIZE-1)) 
		return true;
	return false; 
}
// Check if the queue is empty or not
bool isQueueEmpty(){
	if(front == -1)
		return true;
	return false;
}

// Helper function to generate an HTTP packet
PACKET generateHTTPPacket(){
	PACKET newHTTPPacket;
	newHTTPPacket.type = 0; // Setting type to 0 for the HTTP packets
	newHTTPPacket.arrivalTime = 0; // Setting arrival time to 0 for all packets because they arrive simultaneously
	newHTTPPacket.size = 80; // Setting size to 80 as defined in the question
	return newHTTPPacket; // returning the new packet generated
}

// Helper function to generate a video packet
PACKET generateVideoPacket(int i, int videoPacketTotalSize){
	PACKET newVideoPacket;
	newVideoPacket.type = 1; // Setting type to 1 for the video packets
	newVideoPacket.arrivalTime = (float)i + ((32000 - videoPacketTotalSize)/32000); // Setting arrival time of the packet 
	newVideoPacket.size = 400; // Setting size to 400 as defined in the question
	return newVideoPacket; // returning the new packet generated
}

// Helper function to insert HTTP Packets in the queue
void insertHTTPPackets(){
	int HTTPPacketTotalSize = 512000; // Total size of HTTP packets to be inserted
	while(HTTPPacketTotalSize){
		PACKET p = generateHTTPPacket(); // Generate an HTTP packet
		// Check if the queue is full
		if(isQueueFull()){
			printf("Queue Overflow HTTP\n");
			exit(1);
		}
		// Insert a packet if the queue is not full
		else{
			if(front == -1) front = 0;
			rear = (rear + 1) % MAX_QUEUE_SIZE;
			queue[rear] = p; // Adding packet to the queue
			HTTPPacketInserted++; // Increament the HTTP Packet Inserted count
		}
		HTTPPacketTotalSize -= 80;
	}
}

// Helper function to insert Video Packets in the queue
void insertVideoPackets(int i){
	int videoPacketTotalSize = 32000; // Total size of Video Packets to be inserted
	while(videoPacketTotalSize){
		PACKET p = generateVideoPacket(i,videoPacketTotalSize); // Generate a video packet
		// Check if the queue is full
		if(isQueueFull()){
			printf("Queue Overflow Video\n");
			exit(1);
		}
		// Insert a video packet if the queue is not full
		else{
			if(front == -1) front = 0;
			rear = (rear + 1) % MAX_QUEUE_SIZE;
			queue[rear] = p; // Adding packet to the queue
			videoPacketInserted++; // Increment the video packet inserted count
		}
		videoPacketTotalSize -= 400;
	}
}

// Helper function to pop a packet out
PACKET popPacket(){
	PACKET p;
	// Check if the queue is empty or not
	if(isQueueEmpty()){
		printf("Queue Underflow\n");
	}
	else{
		p = queue[front];
		if(front == rear){
			front = -1;
			rear = -1;
		}
		else{
			front = (front + 1) % MAX_QUEUE_SIZE;
		}
		return p; // Return the popped packet
	}
}

int main(){
	// Initializing the parameters 
	int HTTPPacketsDropped = 0;
	int HTTPPacketTransmitted = 0;
	int VideoPacketsDropped = 0;
	int VideoPacketTransmitted = 0;
	// Inserting the HTTP packets as defined in the question
	insertHTTPPackets();
	// Running the simulation for 15 seconds
	for(int i = 0; i <= 15; i++){
		int bandWidth = 64000; // Amount of data that can be dequeued in each iteration of the loop
		insertVideoPackets(i); // Inserting the video packets to the queue
		// Dequeing of packets starts here
		// Dequeue the packets while sufficient bandwidth is available and queue is not empty
		while(bandWidth && !isQueueEmpty()){
			PACKET p = popPacket(); // Pop the packet from the queue
			switch(p.type){
				// If the packet is of HTTP type (type 0)
				// Check if the packet is stale of not
				// If not stale
					// Transmit the packet and decrease the available bandwidth
				// Else
					// Drop the packet
				case 0: if(fabs(i - p.arrivalTime) < 15){
							HTTPPacketTransmitted++;
							bandWidth -= 80;
						}
						else{
							HTTPPacketsDropped++;
						}
						break;
				// If the packet is of video type (type 1)
				// Check if the packet is stale or not (ie delay is less than or equal to 1 second)
				// If not stale
					// Transmit the packet and decrease the available bandwidth
				// Else 
					// Drop the packet
				case 1: if(p.type == 1 && fabs(i - p.arrivalTime) <= 1){
							VideoPacketTransmitted++;
							bandWidth -= 400;
						}
						else{
							VideoPacketsDropped++;
						}
						break;
			}
		}
	}
	// Printing the results of the simulation
	printf("HTTP Packets Inserted : %d\n",HTTPPacketInserted);
	printf("Video Packets Inserted : %d\n", videoPacketInserted);
	printf("Percentage of HTTP Packets Dropped : %f\n", (float)HTTPPacketsDropped/(float)HTTPPacketInserted * 100);
	printf("Percentage of Video Packets Dropped : %f\n", (float)VideoPacketsDropped/(float)videoPacketInserted * 100);
	return 0;
}

/* 
Answers to the Problem 
HTTP Packets Inserted : 6400
Video Packets Inserted : 1280
Percentage of HTTP Packets Dropped : 0.000000
Percentage of Video Packets Dropped : 43.750000

The problem with this approach is that HTTP packets will choke the bandwidth and the video packets won't get sent out in time.
If we follow this approach HTTP packets packets will get sent first and we will have to drop 8 seconds worth of video packets.
*/