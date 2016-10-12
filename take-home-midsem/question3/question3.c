#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_QUEUE_SIZE 10000

typedef struct PACKET{
	int type;
	float arrivalTime;
	int size;
}PACKET;

PACKET httpQueue[MAX_QUEUE_SIZE];
PACKET videoQueue[MAX_QUEUE_SIZE];
int httpFront = -1, httpRear = -1;
int videoFront = -1, videoRear = -1;

int videoPacketsInserted = 0;
int httpPacketsInserted = 0;

bool httpQueueFull(){
	if((httpFront == httpRear+1) || (httpFront == 0 && httpRear == MAX_QUEUE_SIZE-1)) 
		return true;
	return false; 
}

bool httpQueueEmpty(){
	if(httpFront == -1)
		return true;
	return false;
}

bool videoQueueFull(){
	if((videoFront == videoRear + 1) || (videoFront == 0 && videoRear == MAX_QUEUE_SIZE-1))
		return true;
	return false;
}

bool videoQueueEmpty(){
	if(videoFront == -1)
		return true;
	return false;
}

PACKET generateHTTPPacket(float i){
	PACKET newPacket;
	newPacket.type = 0;
	newPacket.arrivalTime = -16 + i * (0.0025) ;
	newPacket.size = 80;
	return newPacket;
}

PACKET generateVideoPacket(float i,int videoPacketTotalSize){
	PACKET newVideoPacket;
	newVideoPacket.type = 1; // Setting type to 1 for the video packets
	newVideoPacket.arrivalTime = (float)i + ((32000 - videoPacketTotalSize)/32000); // Setting arrival time of the packet 
	newVideoPacket.size = 400; // Setting size to 400 as defined in the question
	return newVideoPacket; // returning the new packet generated
}

void insertHTTPPackets(){
	int httpTotalPacketSize = 512000;
	int i = 0;
	while(httpTotalPacketSize){
		PACKET p = generateHTTPPacket(i);
		if(httpQueueFull()){
			printf("HTTP Queue Overflow\n");
			exit(1);
		}
		else{
			if(httpFront == -1) httpFront = 0;
			httpRear = (httpRear + 1) % MAX_QUEUE_SIZE;
			httpQueue[httpRear] = p;
			httpPacketsInserted++;
		}
		httpTotalPacketSize -= 80;
	}
}

void insertVideoPackets(int i){
	int videoTotalPacketSize = 32000;
	while(videoTotalPacketSize){
		PACKET p = generateVideoPacket(i,videoTotalPacketSize);
		if(videoQueueFull()){
			printf("Video Queue Overflow\n");
			exit(1);
		}
		else{
			if(videoFront == -1) videoFront = 0;
			videoRear = (videoRear + 1) % MAX_QUEUE_SIZE;
			videoQueue[videoRear] = p;
			videoPacketsInserted++;
		}
		videoTotalPacketSize -= 400;
	}
}

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

int calculateMaxHTTPBandWidth(int time, int HTTPPacketsSize, int videoPacketPerIteration){
	int bandWidth = HTTPPacketsSize/time;
	int minVideoBW = videoPacketPerIteration * 0.9;
	while((bandWidth % 80 != 0) || (bandWidth % 400 != 0) || (bandWidth <= minVideoBW)){
		bandWidth++;
	} 
	return bandWidth;
}

int main(){
	insertHTTPPackets();
	int HTTPPacketsRemaining = 512000;
	int calculatedMaxHTTPBandWidth = calculateMaxHTTPBandWidth(15,512000,32000);
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
		while(maxHTTPBandWidth && !httpQueueEmpty()){
			PACKET p = popHTTPPacket();
			maxHTTPBandWidth -= 80;
			httpPacketsTransmitted++;
			HTTPPacketsRemaining -= 80;
		}
		int videoBandWidth = totalBandWidth - calculatedMaxHTTPBandWidth + maxHTTPBandWidth;
		while(videoBandWidth && !videoQueueEmpty()){
			PACKET p = popVideoPacket();
			videoBandWidth -= 400;
			videoPacketsTransmitted++;
		}
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

*/
