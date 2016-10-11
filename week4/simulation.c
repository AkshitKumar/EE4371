/*
	Name : Akshit Kumar
	Roll Number : EE14B127
	Assignment : To compare a priority queue using heaps and binary search trees
	and output the drop rate and time spent in maintaining priority queue using BSTs
	and heaps.
*/

// Including the required header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

// Defining the maximum queue size after which the packets will be dropped
#define MAXIMUM_QUEUE_SIZE 127

// Definition of PACKET data type as a struct
typedef struct PACKET{
	int id; // packet id
	int t0; // arrival time of the packet
	int L; // length of the packet
	char *contents; // contents of the packet
}PACKET;

// Function returning the next time the packet arrive
int nextTime(float rateParameter){
	return (int)(-logf(1.0f - (float)rand()/((float)RAND_MAX + 1.0))/ rateParameter);
}

// Heap Implementation
PACKET heapQueue[MAXIMUM_QUEUE_SIZE]; // declaring array for heap implementation
int heapsize = 0; // initialise heap size to 0

// Function to return the parent index of a node in heap
int parent(int i){
	return ceil((i >> 1) - 1);
}

// Function to swap two packets in the heap
void swap(PACKET packet1, PACKET packet2){
	PACKET temp = packet1;
	packet1 = packet2;
	packet2 = packet1;
}

// Helper function to enqueue a new node into the heap
void decreaseKey(int i, PACKET packet){
	if(packet.L > heapQueue[i].L){
		printf("Error\n");
		return;
	}
	heapQueue[i] = packet;
	while(i > 0 && heapQueue[parent(i)].L > heapQueue[i].L){
		swap(heapQueue[i],heapQueue[parent(i)]);
		i = parent(i);
	}
}

// Function to min heapify - maintain the min heap priority queue with each incoming packet
void minHeapify(int i){
	int largest;
	int l = (i << 1) + 1;
	if(l < heapsize && heapQueue[l].L < heapQueue[i].L)
		largest = l;
	else
		largest = i;
	int r = ((i+1) << 1);
	if(r < heapsize && heapQueue[r].L < heapQueue[largest].L)
		largest = r;
	if(largest != i){
		swap(heapQueue[i],heapQueue[largest]);
		minHeapify(largest);
	}
}

// Function to return the packet with the least size
PACKET heapMinimum(){
	return heapQueue[0];
}

// Function to add new packet to the heap priority queue
void heapEnqueue(PACKET packet){
	++heapsize;
	heapQueue[heapsize-1].L = INT_MAX;
	decreaseKey(heapsize-1,packet);
}


// Function to dequeue a packet when it is ready to be transmitted
// Deallocates memory allocated to the packet
// Returns the packet to determine it's size
PACKET heapDequeue(){
	if(heapsize < 0){
		printf("Heap Underflow\n");
	}
	PACKET min = heapQueue[0];
	free(min.contents);
	min.contents = NULL;
	heapQueue[0] = heapQueue[heapsize-1];
	--heapsize;
	minHeapify(0);
	return min;
}


// BST Implementation
typedef struct BSTNode{
    PACKET packet;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

// Function to create a new node for an incoming packet which is to be queued
struct BSTNode* newNode(PACKET packet){
    BSTNode *temp = (BSTNode*) malloc(sizeof(BSTNode));
    temp->packet = packet;
    temp->left = temp->right = NULL;
    return temp;
}

// Function to enqueue the new packet into the BST
struct BSTNode* _bst_enqueue(struct BSTNode* node, PACKET packet){
    if(node == NULL) return newNode(packet);
    if(packet.L < node->packet.L){
        node->left = _bst_enqueue(node->left,packet);
    }
    else if(packet.L >= node->packet.L){
        node->right = _bst_enqueue(node->right,packet);
    }
    return node;
}

// Function to find the node with the minimum packet size
struct BSTNode* _min_value_packet(struct BSTNode* node){
    BSTNode* current = node;
    while(current->left != NULL){
        current = current->left;
    }
    return current;
}

// Function to dequeue the packet with the minimum size
struct BSTNode* _bst_dequeue(struct BSTNode* root, int key){
    if(root == NULL) return root;
    if(key < root->packet.L){
        root->left = _bst_dequeue(root->left,key);
    }
    else if(key > root->packet.L){
        root->right = _bst_dequeue(root->right,key);
    }
    else{
        if(root->left == NULL){
            BSTNode *temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL){
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        struct BSTNode* temp = _min_value_packet(root->right);
        root->packet = temp->packet;
        root->right = _bst_dequeue(root->right,temp->packet.L);
    }
    return root;
}


// Helper Function 
// Function to get a random packet length for the incoming packet
int packetLength(int minLength, int maxLength){
	double randomLength = rand()/(1.0 + RAND_MAX);
	int range = maxLength - minLength + 1;
	return (int)((randomLength * range) + minLength);
}

// Function to generate a memory block to simulate an incoming packet
PACKET generatePacket(int t0, int id){
	PACKET newPacket;
	int packetSize = packetLength(80,1550);
	newPacket.L = packetSize;
	newPacket.t0 = t0;
	newPacket.contents = (char *) malloc(packetSize + 1);
	newPacket.id = id;
	return newPacket;
}

// Function to simulate network node using heaps
void simulateHeapQueue(float rateParameter, float outputRate, int times){
	// Initialize parameters for the simulation
	int queueCount = 0;
	bool outputBlocked = false; // setting output channel to open
	int time = 0; // setting next time count to 0
	int dequeueTime = 0; // setting dequeue time of the packets to 0
	int packetsComingIn = 0; // calculating the incoming packets
	int droppedPackets = 0; // initializing drop packets to 0
	bool canReceive = true; // enabling receie
	int t0 = 0;
	for(int i = 0; i < times; i++){
		// if the time for next arrival is there, canReceive is true and function block
		// is executed
		if(canReceive){
			packetsComingIn++; // increment incoming packets
			// if queuecount is less than max queue size
			// packet is generated and added to the queue
			// else
			// the packet is dropped ie dropped packet count is increased
			if(queueCount < MAXIMUM_QUEUE_SIZE){
				PACKET newPacket = generatePacket(t0,i);
				heapEnqueue(newPacket);
				++queueCount;
			}
			else{
				droppedPackets++;
			}
			// getting the time for next arrival of the packet
			time = nextTime(rateParameter);
			t0 += time;
		}
		// If the output channel is not blocked and queueCount is not zero
		// the packet is ready to be dequeued
		// the dequeue time is calculated and set to dequeueTime 
		// the queueCount is decreased
		if(!outputBlocked && queueCount){
			PACKET extractedPacket = heapDequeue();
			int extractedPacketLength = extractedPacket.L;
			dequeueTime = (int) (extractedPacketLength / outputRate);
			dequeueTime = dequeueTime > 0 ? dequeueTime : 1;
			queueCount--;
		}
		// if the time for next packet arrival is less than dequeueTime
		// the output channel is kept blocked and no new packet can be dequeued
		// can Receive is set to true 
		// dequeueTime is decreased by the next time of the packet
		if(time <= dequeueTime){
			outputBlocked = true;
			dequeueTime = dequeueTime - time;
			canReceive = true;
		}
		// else
		// output channel is set open and queued packets can be now be dequeued again
		// next time packet is decreased by the dequeue Time
		// canReceive is set to false because no packet would be coming in the 
		// next iteration.
		else{
			outputBlocked = false;
			time = time - dequeueTime;
			canReceive = false;
		}
	}
	printf("Drop rate in case of Heaps: %f\n",((float)droppedPackets)/packetsComingIn);
}

// Function to simulate network node using a BST
void simulateBSTQueue(float rateParameter, float outputRate, int times){
	// Initialize parameters for the simulation
	int queueCount = 0;
	bool outputBlocked = false; // setting output channel to open
	int time = 0; // setting next time count to 0
	int dequeueTime = 0; // setting dequeue time of the packets to 0
	int packetsComingIn = 0; // calculating the incoming packets
	int droppedPackets = 0; // initializing drop packets to 0
	bool canReceive = true; // enabling receie
	int t0 = 0;
	struct BSTNode* root = NULL;
	for(int i = 0; i < times; i++){
		// if the time for next arrival is there, canReceive is true and function block
		// is executed
		if(canReceive){
			packetsComingIn++;// increment incoming packets
			// if queuecount is less than max queue size
			// packet is generated and added to the queue
			// else
			// the packet is dropped ie dropped packet count is increased
			if(queueCount < MAXIMUM_QUEUE_SIZE){
				PACKET newPacket = generatePacket(t0,i);
				root = _bst_enqueue(root,newPacket);
				++queueCount;
			}
			else{
				droppedPackets++;
			}
			// getting the time for next arrival of the packet
			time = nextTime(rateParameter);
		}
		// If the output channel is not blocked and queueCount is not zero
		// the packet is ready to be dequeued
		// the dequeue time is calculated and set to dequeueTime 
		// the queueCount is decreased
		if(!outputBlocked && queueCount){
			struct BSTNode* minPacket = _min_value_packet(root);
			root = _bst_dequeue(root,(minPacket->packet).L);
			int extractedPacketLength = (minPacket->packet).L;
			dequeueTime = (int) (extractedPacketLength / outputRate);
			dequeueTime = dequeueTime > 0 ? dequeueTime : 1;
			queueCount--;
		}
		// if the time for next packet arrival is less than dequeueTime
		// the output channel is kept blocked and no new packet can be dequeued
		// can Receive is set to true 
		// dequeueTime is decreased by the next time of the packet
		if(time <= dequeueTime){
			outputBlocked = true;
			dequeueTime = dequeueTime - time;
			canReceive = true;
		}
		// else
		// output channel is set open and queued packets can be now be dequeued again
		// next time packet is decreased by the dequeue Time
		// canReceive is set to false because no packet would be coming in the 
		// next iteration.
		else{
			outputBlocked = false;
			time = time - dequeueTime;
			canReceive = false;
		}
	}
	printf("Drop rate in case of BSTs: %f\n",((float)droppedPackets)/packetsComingIn);
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage ./a.out <times>\n");
		exit(1);
	}
	int times = atoi(argv[1]); // getting the times for the command line
	int seed = time(NULL); // getting the seed for rand function
	srand(seed); // seeding the rand function
	// Setting the input and output rates
	float rateParameter = 0.1;
	float outputRate = 80;
	clock_t heapBegin,heapEnd,BSTBegin,BSTEnd;
	heapBegin = clock();
	simulateHeapQueue(rateParameter,outputRate,times);
	heapEnd = clock();
	BSTBegin = clock();
	simulateBSTQueue(rateParameter,outputRate,times);
	BSTEnd = clock();
	printf("Time Spent in Heap : %f\n",(double) (heapEnd - heapBegin)/CLOCKS_PER_SEC);
	printf("Time Spent in BST : %f\n",(double) (BSTEnd - BSTBegin)/CLOCKS_PER_SEC );
	return 0;
}

/* Input - Output on Running the simulation different times
./a.out 100
Drop rate in case of Heaps: 0.000000
Drop rate in case of BSTs: 0.000000
Time Spent in Heap : 0.000148
Time Spent in BST : 0.000033

./a.out 1000
Drop rate in case of Heaps: 0.000000
Drop rate in case of BSTs: 0.000000
Time Spent in Heap : 0.000341
Time Spent in BST : 0.000282

./a.out 10000
Drop rate in case of Heaps: 0.023870
Drop rate in case of BSTs: 0.000000
Time Spent in Heap : 0.002423
Time Spent in BST : 0.002158

./a.out 100000
Drop rate in case of Heaps: 0.016267
Drop rate in case of BSTs: 0.015256
Time Spent in Heap : 0.017955
Time Spent in BST : 0.019376

./a.out 1000000
Drop rate in case of Heaps: 0.019057
Drop rate in case of BSTs: 0.018845
Time Spent in Heap : 0.152207
Time Spent in BST : 0.195127

./a.out 10000000
Drop rate in case of Heaps: 0.020934
Drop rate in case of BSTs: 0.019517
Time Spent in Heap : 1.463579
Time Spent in BST : 2.011110

Solution to the questions :
On average the heaps are faster than BSTs to implement priority queue. For small number
of iterations, BSTs are very fast (up till 1000 iterations) but the number of iterations 
increase, the heaps perform increasing well compared to BSTs. Reasons for heaps performing 
better than BSTs could be :
* Heaps are implemented using arrays while BSTs are implemented using pointers. Locality of
reference helps in the case of heaps
* The BSTs might be becoming unbalanced resulting in worst case times of O(n) for inserting
compared to heaps consistently taking O(log n) for insertion and deletion

*/