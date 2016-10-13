/* 
	Name : Akshit Kumar
	Roll No. : EE14B127
	Question : Implement two separate circular buffers each containing either HTTP or video packets and dequeue packets
	from the two circular buffers such that all HTTP packets are dequeued while some video packets may be dropped
*/

// Inclusion of necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Defining the maximum queue size
#define MAX_QUEUE_SIZE 10000

// Defination of the PACKET structure
typedef struct PACKET{
	int type; // type = 0 for HTTP Packets and type = 1 for Video Packets
	float arrivalTime; // Arrival Time of the Packet
	int size; // size = 80 for HTTP Packets and size = 400 for Video Packets
}PACKET;

// Helper function and variables for the implementation of 2 circular buffers
PACKET httpQueue[MAX_QUEUE_SIZE]; // Declaring the Circular Queue with size of 10000
PACKET videoQueue[MAX_QUEUE_SIZE]; // Declaring the Circular Queue with size of 10000
int httpFront = -1, httpRear = -1; // Initialising the front and rear variables
int videoFront = -1, videoRear = -1; // Initialising the front and rear variables

int videoPacketsInserted = 0; // Count of Video Packets Inserted in the simulations
int httpPacketsInserted = 0; // Count of HTTP Packets Inserted in the simulations

// Check if the queue is full or not
bool httpQueueFull(){
	if((httpFront == httpRear+1) || (httpFront == 0 && httpRear == MAX_QUEUE_SIZE-1)) 
		return true;
	return false; 
}
// Check if the queue is empty or not
bool httpQueueEmpty(){
	if(httpFront == -1)
		return true;
	return false;
}
// Check if the queue is full or not
bool videoQueueFull(){
	if((videoFront == videoRear + 1) || (videoFront == 0 && videoRear == MAX_QUEUE_SIZE-1))
		return true;
	return false;
}
// Check if the queue is empty or not
bool videoQueueEmpty(){
	if(videoFront == -1)
		return true;
	return false;
}
// Helper function to generate an HTTP packet
PACKET generateHTTPPacket(float i){
	PACKET newPacket;
	newPacket.type = 0; // Setting type to 0 for the HTTP packets
	newPacket.arrivalTime = -16 + i * (0.0025) ; // Setting arrival time 
	newPacket.size = 80; // Setting size to 80 as defined in the question
	return newPacket;
}
// Helper functions to return the age of last package queued
float last_package_age(int i){
	return fabs(httpQueue[httpRear].arrivalTime) + i;
}
// Helper function to return the current queue length
int current_queue_length(int HTTPPacketsRemaining){
	return HTTPPacketsRemaining;
}
// Helper function to generate a video packet
PACKET generateVideoPacket(float i,int videoPacketTotalSize){
	PACKET newVideoPacket;
	newVideoPacket.type = 1; // Setting type to 1 for the video packets
	newVideoPacket.arrivalTime = (float)i + ((32000 - videoPacketTotalSize)/32000); // Setting arrival time of the packet 
	newVideoPacket.size = 400; // Setting size to 400 as defined in the question
	return newVideoPacket; // returning the new packet generated
}

// Helper function to insert HTTP Packets in the queue
void insertHTTPPackets(){
	int httpTotalPacketSize = 512000; // Total size of HTTP packets to be inserted
	int i = 0;
	while(httpTotalPacketSize){
		PACKET p = generateHTTPPacket(i); // Generate an HTTP packet
		// Check if the queue is full
		if(httpQueueFull()){
			printf("HTTP Queue Overflow\n");
			exit(1);
		}
		// Insert a packet if the queue is not full
		else{
			if(httpFront == -1) httpFront = 0;
			httpRear = (httpRear + 1) % MAX_QUEUE_SIZE;
			httpQueue[httpRear] = p; // Adding packet to the queue
			httpPacketsInserted++; // Increament the HTTP Packet Inserted count
		}
		httpTotalPacketSize -= 80;
	}
}
// Helper function to insert Video Packets in the queue
void insertVideoPackets(int i){
	int videoTotalPacketSize = 32000; // Total size of Video Packets to be inserted
	while(videoTotalPacketSize){
		PACKET p = generateVideoPacket(i,videoTotalPacketSize); // Generate a video packet
		// Check if the queue is full
		if(videoQueueFull()){
			printf("Video Queue Overflow\n");
			exit(1);
		}
		// Insert a video packet if the queue is not full
		else{
			if(videoFront == -1) videoFront = 0;
			videoRear = (videoRear + 1) % MAX_QUEUE_SIZE;
			videoQueue[videoRear] = p; // Adding packet to the queue
			videoPacketsInserted++; // Increment the video packet inserted count
		}
		videoTotalPacketSize -= 400;
	}
}

// Helper function to pop a packet out
PACKET popHTTPPacket(){
	PACKET p;
	if(httpQueueEmpty()){
		printf("HTTP Queue Underflow\n");
	}
	else{
		p = httpQueue[httpFront];
		if(httpFront == httpRear){
			httpFront = -1;
			httpRear = -1;
		}
		else{
			httpFront = (httpFront + 1) % MAX_QUEUE_SIZE;
		}
		return p;
	}
}

// Helper function to pop a packet out
PACKET popVideoPacket(){
	PACKET p;
	if(videoQueueEmpty()){
		printf("Video Queue Empty\n");
	}
	else{
		p = videoQueue[videoFront];
		if(videoFront == videoRear){
			videoFront = -1;
			videoRear = -1;
		}
		else{
			videoFront = (videoFront + 1) % MAX_QUEUE_SIZE;
		}
		return p;
	}
}

// Main algorithm for the calculating how many HTTP packets need to be sent out in each iteration
// such that all HTTP packets can be dequeued and that the drop rate is less than the acceptable video drop rate
int calculateMaxHTTPBandWidth(int time, int HTTPPacketsSize, int videoPacketPerIteration){
	int bandWidth = HTTPPacketsSize/time; // calculating the lower bound of the bandwidth of HTTP to be dequeued
	int minVideoBW = videoPacketPerIteration * 0.9; // calculating minimum video packets to be sent out in each iteration
	// calculating the upper bound of HTTP packets to be dequeued with constrainsts
	while((bandWidth % 80 != 0) || (bandWidth % 400 != 0) || (bandWidth <= minVideoBW)){
		bandWidth++;
	} 
	return bandWidth;
}

int main(){
	insertHTTPPackets(); // inserting the HTTP packets
	int HTTPPacketsRemaining = 512000;
	int calculatedMaxHTTPBandWidth = calculateMaxHTTPBandWidth(15,512000,32000); // calculating the optimal bandwidth
	printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n","Time","HTTPQ","VideoQ","HTTPSent","VideoSent","HTTP Drop","Video Drop");
	printf("%s\t%d\t%d\t%d\t\t%d\t\t%f\t%f\n","0*",HTTPPacketsRemaining,0,0,0,0.0,0.0);
	for(int i = 0; i <= 15; i++){
		int totalBandWidth = 64000;
		int httpPacketsTransmitted = 0;
		int videoPacketsTransmitted = 0;
		int httpPacketsDropped = 0;
		int videoPacketsDropped = 0;
		insertVideoPackets(i);
		int maxHTTPBandWidth = calculatedMaxHTTPBandWidth;
		// first dequeue the HTTP packets till the optimal bandwidth for HTTP packets
		while(maxHTTPBandWidth && !httpQueueEmpty()){
			PACKET p = popHTTPPacket();
			maxHTTPBandWidth -= 80;
			httpPacketsTransmitted++;
			HTTPPacketsRemaining -= 80;
		}
		int videoBandWidth = totalBandWidth - calculatedMaxHTTPBandWidth + maxHTTPBandWidth;
		// then dequeue the video packets for the remainder of the bandwidth available
		while(videoBandWidth && !videoQueueEmpty()){
			PACKET p = popVideoPacket();
			videoBandWidth -= 400;
			videoPacketsTransmitted++;
		}
		// Drop the rest of the packets as they will become stale in the next second
		while(!videoQueueEmpty()){
			PACKET p = popVideoPacket();
			videoPacketsDropped++;
		}
		printf("%d\t%d\t%d\t%d\t\t%d\t\t%f\t%f\n",i,HTTPPacketsRemaining,32000,httpPacketsTransmitted*80,videoPacketsTransmitted*400,0.0,(float)videoPacketsDropped/(videoPacketsDropped+videoPacketsTransmitted)*100);
	}
	return 0;
}

/* Answer to the problem
Time	HTTPQ	VideoQ	HTTPSent	VideoSent	HTTP Drop	Video Drop
0*	512000		0	0		0		0.000000	0.000000
0	477600		32000	34400		29600		0.000000	7.500000
1	443200		32000	34400		29600		0.000000	7.500000
2	408800		32000	34400		29600		0.000000	7.500000
3	374400		32000	34400		29600		0.000000	7.500000
4	340000		32000	34400		29600		0.000000	7.500000
5	305600		32000	34400		29600		0.000000	7.500000
6	271200		32000	34400		29600		0.000000	7.500000
7	236800		32000	34400		29600		0.000000	7.500000
8	202400		32000	34400		29600		0.000000	7.500000
9	168000		32000	34400		29600		0.000000	7.500000
10	133600		32000	34400		29600		0.000000	7.500000
11	99200		32000	34400		29600		0.000000	7.500000
12	64800		32000	34400		29600		0.000000	7.500000
13	30400		32000	34400		29600		0.000000	7.500000
14	0			32000	30400		32000		0.000000	0.000000
15	0			32000	0			32000		0.000000	0.000000

The router will always dequeue the HTTP packets first (like a greedy algorithm) till a calculated value and then dequeue video packets
for the remainder of the available bandwidth. The calculated value is such that the video drop rate is less than 10%. The type of queues are
2 circular buffers - one for HTTP & one for video.
*/
