/*
    Name : Akshit Kumar
    Roll Number : EE14B127
    Assignment : To compare a priority queue implemented using heaps and Binary
    Search Trees by simulating a network node
*/
// Including required header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <inttypes.h>

// defining the maximum queue size after which the packets will be dropped
#define MAXIMUM_QUEUE_SIZE 127

// Definition of PACKET data type as a struct
typedef struct PACKET{
    int id; // unique identifier of the Packet
    int t0; // arrival time of the packet
    int L; // length of the packet
    char *contents; // contents of the packet
}PACKET;

// Heap Implementation
PACKET heapQueuePackets[MAXIMUM_QUEUE_SIZE]; // declaring array for heap Implementation

int heapsize = 0; // initialize heap size to 0

// Function to swap two packets in the heap
void _swap_heap_packets(PACKET packet1,PACKET packet2){
    PACKET temp = packet1;
    packet1 = packet2;
    packet2 = temp;
}

// Function to return the parent index of a node in heap
int _parent(int i){
    return ceil((i >> 1) - 1);
}

// Helper Function to enqueue a new node into the heap
void _decreaseKey(int i, PACKET packet){
    if(packet.L > heapQueuePackets[i].L){
        printf("Error\n");
        return;
    }
    heapQueuePackets[i] = packet;
    while(i > 0 && heapQueuePackets[_parent(i)].L > heapQueuePackets[i].L){
        _swap_heap_packets(heapQueuePackets[i],heapQueuePackets[_parent(i)]);
        i = _parent(i);
    }
}

// Function to min heapify - maintain the min heap priority queue with each incoming packet
void _min_heapify(int i){
    int largest;
    int l = (i << 1) + 1;
    if(l < heapsize && heapQueuePackets[l].L < heapQueuePackets[i].L){
        largest = l;
    }
    else{
        largest = l;
    }
    int r = (i+1) << 1;
    if(r < heapsize && heapQueuePackets[r].L < heapQueuePackets[largest].L){
        largest = r;
    }
    if(largest != i){
        _swap_heap_packets(heapQueuePackets[i],heapQueuePackets[largest]);
        _min_heapify(largest); // recursive call to correct the subtrees as well
    }
}

// Function to add new packet to the priority queue
void _heap_enqueue(PACKET packet){
    ++heapsize;
    heapQueuePackets[heapsize-1].L = INT_MAX;
    _decreaseKey(heapsize-1,packet);
}

// Function to find the minimum packet size in the heap
int _heap_minimum_packet_length(){
    return heapQueuePackets[0].L;
}

// Function to dequeue a packet when it is ready to be transmitted
// Deallocates memory allocated to the packet
// Returns the packet to determine it's size
PACKET _heap_dequeue(){
    if(heapsize < 0){
        printf("Heap Underflow\n");
    }
    PACKET min = heapQueuePackets[0];
    free(min.contents);
    min.contents = NULL;
    heapQueuePackets[0] = heapQueuePackets[heapsize-1];
    --heapsize;
    _min_heapify(0);
    return min;
}

// Binary Search Tree Implementation

// Definition of BSTNode structure
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

// Helper Function to print the status of the queue on each iteration
// Used for debugging purposes
void inorder(struct BSTNode *root){
    if(root != NULL){
        inorder(root->left);
        printf("%d %d\n",root->packet.L, root->packet.id);
        inorder(root->right);
    }
}

// Functions for the arrival of packets
int nextTime(float rateParameter){
    return (int)(-logf(1.0f - (float)(rand()/(float)(RAND_MAX+1.0)))/rateParameter);
}

// Function to get a random packet length for the incoming packet
int _packet_length(int min_length, int max_length){
    double random_length = rand()/(1.0 + RAND_MAX);
    int range = max_length - min_length + 1;
    return random_length * range + min_length;
}

// Function to generate a memory block to simulate an incoming packet
PACKET generate_incoming_packet(int t0,int id){
    PACKET new_packet;
    int packet_size = _packet_length(80,1550);
    new_packet.L = packet_size;
    new_packet.t0 = t0;
    new_packet.contents = (char*) malloc(packet_size+1);
    new_packet.id = id;
    return new_packet;
}

// Function to simulate network node using heaps
float simulateHeapPriorityQueue(long long int times,float rateParameter,float output_rate){
    int queue_count = 0;
    struct timespec start,end; // Clocking the time to run the simulation
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int t0 = 0;
    int id = 0;
    long int packet_size_sum = 0;
    long int num_packet_queued = 0;
    for(int i = 0; i < times ; i++){
        int next_time = nextTime(rateParameter); // get the time for arrival of next packet
        t0 += next_time; // add to the arrival times of the packet
        id++;
        while(queue_count){
            int time_for_dequeueing = output_rate * next_time; // calculate time for dequeing
            if(_heap_minimum_packet_length() <= time_for_dequeueing){
                PACKET packet_transmitted = _heap_dequeue(); // deque the packet
                time_for_dequeueing -= packet_transmitted.L; // decrease time available for dequeing the packet
                queue_count--; // update packet count in queue
            }
            else{
                break;
            }
        }
        // check if space available in buffer
        if(queue_count < MAXIMUM_QUEUE_SIZE){
            PACKET new_packet = generate_incoming_packet(t0,id); // generate a packet of random size
            packet_size_sum += new_packet.L;
            _heap_enqueue(new_packet); // add the packet to the queue
            queue_count++; // update the packet count in queue
            num_packet_queued++;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW,&end);
    // calculate the time for running the iterative loop
    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("Average input rate for Heaps: %f\n",rateParameter * (packet_size_sum/num_packet_queued) );
    printf("Time using heaps : %f seconds\n",((float) delta_us)/1000000.0 );
    return (float) delta_us/1000000.0;
}

// Function to simulate network node using BSTs
float simulateBSTPriorityQueue(long long int times,float rateParameter,float output_rate){
    int bst_queue_count = 0;
    struct timespec bst_start, bst_end; // Clocking the time to run the simulation
    clock_gettime(CLOCK_MONOTONIC_RAW,&bst_start);
    int bst_t0 = 0;
    int bst_id = 0;
    long int packet_size_sum = 0;
    long int num_packet_queued = 0;
    struct BSTNode* root = NULL;
    for(int i = 0; i < times; i++){
        int next_time = nextTime(rateParameter);
        bst_t0 += next_time;
        bst_id++;
        while(bst_queue_count){
            int time_for_dequeueing = output_rate * next_time;
            struct BSTNode* min_packet = _min_value_packet(root);
            if((min_packet->packet).L <= time_for_dequeueing){
                root = _bst_dequeue(root,(min_packet->packet).L);
                time_for_dequeueing -= (min_packet->packet).L;
                bst_queue_count--;
            }
            else{
                break;
            }
        }
        if(bst_queue_count < MAXIMUM_QUEUE_SIZE){
            PACKET new_packet = generate_incoming_packet(bst_t0,bst_id);
            root = _bst_enqueue(root,new_packet);
            packet_size_sum += new_packet.L;
            bst_queue_count++;
            num_packet_queued++;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW,&bst_end);
    uint64_t bst_delta_us = (bst_end.tv_sec - bst_start.tv_sec) * 1000000 + (bst_end.tv_nsec - bst_start.tv_nsec) / 1000;
    printf("Average input rate for BSTs : %f\n",rateParameter * (packet_size_sum/num_packet_queued) );
    printf("Time using BSTs : %f seconds\n",((float) bst_delta_us)/1000000.0 );
    return (float) bst_delta_us/1000000.0;
}

int main(){
    float rateParameter = 0.1;
    float output_rate = 1000.0;
    long long int number_iterations = 1e7;
    printf("-------------------------------------------------------------------\n");
    printf("Running simulation using heaps for %lld times\n",number_iterations);
    simulateHeapPriorityQueue(number_iterations,rateParameter,output_rate);
    printf("-------------------------------------------------------------------\n");
    printf("Running simulation using BSTs for %lld times\n",number_iterations);
    simulateBSTPriorityQueue(number_iterations,rateParameter,number_iterations);
    return 0;
}
/*
Input Parameters

Input 1:
rateParameter = 0.1
output_rate = 1000
number of iterations = 1e1
Output 1:
-------------------------------------------------------------------
Running simulation using heaps for 10 times
Average input rate for Heaps: 97.599998
Time using heaps : 0.000055 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 10 times
Average input rate for BSTs : 84.099998
Time using BSTs : 0.000026 seconds

Input 2:
rateParameter = 0.1
output_rate = 1000
number of iterations = 1e2
Output 2:
-------------------------------------------------------------------
Running simulation using heaps for 100 times
Average input rate for Heaps: 90.400002
Time using heaps : 0.000153 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 100 times
Average input rate for BSTs : 74.800003
Time using BSTs : 0.000228 seconds

Input 3:
rateParameter = 0.1
output_rate = 1000
number of iterations = 1e3
Output 3:
-------------------------------------------------------------------
Running simulation using heaps for 1000 times
Average input rate for Heaps: 81.200005
Time using heaps : 0.001144 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 1000 times
Average input rate for BSTs : 81.000000
Time using BSTs : 0.002082 seconds

Input 4:
rateParameter = 0.1
output_rate = 1000
number of iterations = 1e4
Output 4:
-------------------------------------------------------------------
Running simulation using heaps for 10000 times
Average input rate for Heaps: 81.000000
Time using heaps : 0.016065 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 10000 times
Average input rate for BSTs : 81.700005
Time using BSTs : 0.025079 seconds

Input 5:
rateParameter = 0.1
output_rate = 1000
number of iterations = 1e5
Output 5:
-------------------------------------------------------------------
Running simulation using heaps for 100000 times
Average input rate for Heaps: 81.400002
Time using heaps : 0.029540 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 100000 times
Average input rate for BSTs : 81.599998
Time using BSTs : 0.042666 seconds

Input 6:
rateParameter = 0.1
output_rate = 1000
number of iterations : 1e6
Output 6:
-------------------------------------------------------------------
Running simulation using heaps for 1000000 times
Average input rate for Heaps: 81.400002
Time using heaps : 0.198460 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 1000000 times
Average input rate for BSTs : 81.400002
Time using BSTs : 0.384036 seconds

Input 7:
rateParameter = 0.1
output_rate = 1000
number of iterations : 1e7
Output 7:
-------------------------------------------------------------------
Running simulation using heaps for 10000000 times
Average input rate for Heaps: 81.400002
Time using heaps : 1.694899 seconds
-------------------------------------------------------------------
Running simulation using BSTs for 10000000 times
Average input rate for BSTs : 81.500000
Time using BSTs : 3.804684 seconds

Solutions to the questions:
1.  On average the input data rate is 81.4 bytes/second for rateParameter = 0.1 which is
    in close agreement to the theoretical value of 81.5 bytes/second since packet sizes are
    uniformly distributed.
2.  rateParameter*(average packet size) is less than output rate
3.  On an average the heaps are faster than BSTs to implement priority queues. For very small
    number of iterations, BSTs are very fast but as the number of iterations increase, heaps
    perform increasing well compared to BSTs. Reasons for heaps performing better than BSTs could be:
    * Heaps are implemented using arrays while BSTs are implemented using pointers. Locality of reference
    helps in the case of heaps.
    * The BSTs might be becoming unbalanced resulting in worst case times of O(n) for insertion and deletion
    compared to heaps consistently taking O(log n) for insertion and deletion.
*/
