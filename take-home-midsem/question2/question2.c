/* 
	Name : Akshit Kumar
	Roll Number : EE14B127
	Question : Implement a Max Heap Priorty Queue to queue Video Packets and HTTP Packets such that Video packets always 
	have higher priority
*/

// Including the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define MAX_QUEUE_SIZE 10000

typedef struct PACKET{
	int type;
	float arrivalTime;
	int size;
	float key;
} PACKET;

int HTTPPacketInserted = 0;
int videoPacketInserted = 0;

PACKET max_heap_queue[MAX_QUEUE_SIZE];

int heapsize = 0;

int parent(int i){
	return ceil((i >> 1) - 1);
}

void swap(PACKET packet1,PACKET packet2){
	PACKET temp = packet1;
	packet1 = packet2;
	packet2 = temp;
}

void max_heapify(int i){
	int largest;
	int left = (i << 1) + 1;
	int right = (i + 1) << 1;
	if(left < heapsize && max_heap_queue[left].key > max_heap_queue[i].key)
		largest = left;
	else
		largest = i;
	if(right < heapsize && max_heap_queue[right].key > max_heap_queue[i].key)
		largest = right;
	if(largest != i){
		swap(max_heap_queue[i],max_heap_queue[largest]);
		max_heapify(largest);
	}
}

void increaseKey(int i, PACKET packet){
	if(packet.key < max_heap_queue[i].key){
		printf("Error\n");
		exit(1);
	}
	max_heap_queue[i] = packet;
	while(i > 0 && max_heap_queue[parent(i)].key < max_heap_queue[i].key){
		swap(max_heap_queue[parent(i)],max_heap_queue[i]);
		i = parent(i);
	}
}

PACKET max_heap_enqueue(PACKET packet){
	++heapsize;
	max_heap_queue[heapsize].key = INT_MIN;
	increaseKey(heapsize-1,packet);
}

PACKET max_heap_dequeue(){
	if(heapsize < 0){
		printf("Heap Underflow\n");
	}
	PACKET max_packet = max_heap_queue[0];
	max_heap_queue[0] = max_heap_queue[heapsize-1];
	--heapsize;
	max_heapify(0);
	return max_packet;
}

PACKET generateHTTPPackets(int i){
	PACKET newHTTPPacket;
	newHTTPPacket.type = 0;
	newHTTPPacket.arrivalTime = 0;
	newHTTPPacket.size = 80;
	newHTTPPacket.key = 6400 - i;
	return newHTTPPacket;
}

PACKET generateVideoPackets(int i, int videoPacketTotalSize, int time){
	PACKET newVideoPacket;
	newVideoPacket.type = 1;
	newVideoPacket.arrivalTime = (float)time + ((32000 - videoPacketTotalSize)/32000);
	newVideoPacket.size = 400;
	newVideoPacket.key = 8000 - i;
	return newVideoPacket;
}

void insertHTTPPackets(){
	int HTTPPacketTotalSize = 512000;
	int i = 0;
	while(HTTPPacketTotalSize){
		PACKET p = generateHTTPPackets(i);
		if(heapsize <= MAX_QUEUE_SIZE){
			max_heap_enqueue(p);
			HTTPPacketTotalSize -= 80;	
			HTTPPacketInserted++;
		}
		i++;
	}
}

void insertVideoPackets(int time){
	int videoPacketTotalSize = 32000;
	int i = 0;
	while(videoPacketTotalSize){
		PACKET p = generateVideoPackets(i,videoPacketTotalSize,time);
		if(heapsize <= MAX_QUEUE_SIZE){
			max_heap_enqueue(p);
			videoPacketTotalSize -= 400;
			videoPacketInserted++;
		}
		i++;
	}
}

int main(){
	int HTTPPacketsDropped = 0;
	int HTTPPacketsTransmitted = 0;
	int VideoPacketsDropped = 0;
	int VideoPacketsTransmitted = 0;
	insertHTTPPackets();
	for(int i = 0; i <= 15; i++){
		int bandWidth = 64000;
		insertVideoPackets(i);
		while(bandWidth && heapsize > 0){
			PACKET p = max_heap_dequeue();
			switch(p.type){
				case 0: if(fabs(i - p.arrivalTime) < 15){
							HTTPPacketsTransmitted++;
							bandWidth -= 80;
						}
						else{
							HTTPPacketsDropped++;
						}
						break;
				case 1: if(fabs(i - p.arrivalTime) <= 1){
							VideoPacketsTransmitted++;
							bandWidth -= 400;
						}
						else{
							VideoPacketsDropped++;
						}
						break;
			}
		}
	}
	printf("HTTP Packets Inserted : %d\n",HTTPPacketInserted);
	printf("Video Packets Inserted : %d\n", videoPacketInserted);
	printf("Percentage of HTTP Packets Dropped : %f\n", (float)HTTPPacketsDropped/(float)HTTPPacketInserted * 100);
	printf("Percentage of Video Packets Dropped : %f\n", (float)VideoPacketsDropped/(float)videoPacketInserted * 100);
	return 0;
}

/*
Answers to the problem
HTTP Packets Inserted : 6400
Video Packets Inserted : 1280
Percentage of HTTP Packets Dropped : 6.250000
Percentage of Video Packets Dropped : 0.000000

Each packet is assigned a unique depending on the type of packet it is :
1. If it is an HTTP packet, it is assigned a key = 6400 - i where i is the arrival time of the packet in integer scale
2. If it is a video packet, it is assigned a key = 8000 - i where i is the arrival time of the packet in integer scale
In each iteration since the key of video packets will always be more than that of the HTTP packets, the video packets will
have higher priority and will always be dequeued first.
Amongst the HTTP packets itself, since the key is inversely proportional to the time of arrival, the packet which arrived 
first is the first one to be dequeued. Similar is the case with the video packets as well. The constant is chosen such that
all the video packets will be dequeued before the HTTP packets are dequeued in each iterations. The least possible value of key will 
be 7920 which is more than the greatest value of HTTP packets which is 6400.
*/