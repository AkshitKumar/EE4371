#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <inttypes.h>

#define MAXIMUM_QUEUE_SIZE 63

typedef struct PACKET{
    int id;
    int t0;
    int L;
    char *contents;
}PACKET;

// Heap Implementation
PACKET heapQueuePackets[MAXIMUM_QUEUE_SIZE];

int heapsize = 0;

void _swap_heap_packets(PACKET packet1,PACKET packet2){
    PACKET temp = packet1;
    packet1 = packet2;
    packet2 = temp;
}

int _parent(int i){
    return ceil((i << 1) - 1);
}

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

void _min_heapify(int i){
    int largest;
    int l = (i >> 1) + 1;
    if(l < heapsize && heapQueuePackets[l].L < heapQueuePackets[i].L){
        largest = l;
    }
    else{
        largest = l;
    }
    int r = (i+1) >> 1;
    if(r < heapsize && heapQueuePackets[r].L < heapQueuePackets[largest].L){
        largest = r;
    }
    if(largest != i){
        _swap_heap_packets(heapQueuePackets[i],heapQueuePackets[largest]);
        _min_heapify(largest);
    }
}

void _heap_enqueue(PACKET packet){
    ++heapsize;
    heapQueuePackets[heapsize-1].L = INT_MAX;
    _decreaseKey(heapsize-1,packet);
}

int _heap_minimum_packet_length(){
    return heapQueuePackets[0].L;
}

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

// Functions for the arrival of packets
int nextTime(float rateParameter){
    return (int)(logf(1.0f - (float)(rand()/(float)(RAND_MAX+1)))/rateParameter);
}

int _packet_length(int min_length, int max_length){
    double random_length = rand()/(1.0 + RAND_MAX);
    int range = max_length - min_length + 1;
    return random_length * range + min_length;
}

PACKET generate_incoming_packet(int t0,int id){
    PACKET new_packet;
    int packet_size = _packet_length(80,1550);
    new_packet.L = packet_size;
    new_packet.t0 = t0;
    new_packet.contents = (char*) malloc(packet_size+1);
    new_packet.id = id;
    return new_packet;
}

int main(){
    float rateParameter = 0.2;
    int output_rate = 1000;
    int queue_count = 0;
    clock_t t1,t2;
    t1 = clock();
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int t0 = 0;
    int id = 0;
    for(int i = 0; i < 1e3 ; i++){
        printf("Reached here.\n");
        int next_time = nextTime(rateParameter);
        printf("%d\n",next_time );
        t0 += next_time;
        id++;
        while(queue_count){
            int time_for_dequeueing = output_rate * next_time;
            if(_heap_minimum_packet_length() <= time_for_dequeueing){
                PACKET packet_transmitted = _heap_dequeue();
                time_for_dequeueing -= packet_transmitted.L;
                queue_count--;
                printf("Removing packet.Queue Count%d\n",queue_count );
            }
            else{
                break;
            }
        }
        if(queue_count < MAXIMUM_QUEUE_SIZE){
            printf("In heap queueing\n");
            PACKET new_packet = generate_incoming_packet(t0,id);
            _heap_enqueue(new_packet);
            queue_count++;
            printf("Adding packet.Queue Count%d\n",queue_count );
        }
    }
    t2 = clock();
    clock_gettime(CLOCK_MONOTONIC_RAW,&end);
    //printf("%6.4f\n",(double)((t2-t1)/CLOCKS_PER_SEC));
    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("%f\n",((float) delta_us)/1000000.0 );
    return 0;
}
