// Including the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <limits.h>

// Defining the maximum queue size
#define MAX_QUEUE_SIZE 10000

// Defination of the PACKET struct which holds all the relevant details about the packet
typedef struct PACKET{
    int type; // if HTTP packet, type = 0 else type == 1
    double t_arrival; // contains the arrival time of the packet in seconds
    int size; // if HTTP packet, size = 80 else size = 400
    double key; // a unique key assigned to each packet to prioritize packets
}PACKET;

// Instantiating an array of structure PACKET to hold all the packets read from the input file ie midterm-input.dat
PACKET packets[MAX_QUEUE_SIZE];
int file_size = 0; // Initialising the file size to 0

/*
 Function to generate a new packet
 @param type : type of the packet
 @param t_arrival : arrival time of the packet
 @param size : size of the packet
 @param key : unique key assigned to each packet to prioritize packets
 */
PACKET generate_packet(int type, double t_arrival, int size){
    PACKET packet;
    packet.type = type;
    packet.t_arrival = t_arrival;
    packet.size = size;
    packet.key = (double)size - t_arrival; // The key is directly proportional to size of packet and inversely proportional to the time, so this way - video packets always have higher priority over the http packets
    return packet;
}

/*
 Function to read the entire file and store all the packets in an array - packets
 @param argv : Get the file name to read the packets from
 */
void read_packets_from_input_file(char** argv){
    FILE *file = fopen(argv[1],"r"); // open the file specified in command line input
    if(file == NULL){
        printf("Unable to open file\n");
        exit(1);
    }
    char line[128]; // assign a string to read a line from the file
    int i = 0;
    int type,size;
    double t_arrival;
    // read the file til end of file is reached
    while(fgets(line,sizeof line,file) != NULL){
        bool isTime = true;
        // Remove the '\n' character from end of each line
        char *pos;
        if ((pos=strchr(line,'\n')) != NULL)
            *pos = '\0';
        // tokenize the string read by a space
        char *token;
        token = strtok(line," "); // returns a pointer to a list containing words separated by a space
        // iterate till the pointer points to NULL
        while (token != NULL) {
            if(isTime){
                sscanf(token,"%lf",&t_arrival); // read in the arrival time of the packet
            }
            else{
                if(strcmp(token,"http") == 0){
                    type = 0; // set the type of the packet
                    size = 80; // set the size of the packet
                }
                else if(strcmp(token,"video") == 0){
                    type = 1; // set the type of the packet
                    size = 400; // set the size of the packet
                }
            }
            isTime = false;
            token = strtok(NULL," "); // make the pointer point to next word
        }
        PACKET packet = generate_packet(type,t_arrival,size); // generate an instant of the structure
        packets[i++] = packet; // insert that packet into the packets array
        file_size++; // increase the file size by 1 with every line that is read
    }
    fclose(file); // close the file after reading it completely
}

PACKET max_heap_queue[MAX_QUEUE_SIZE];

int heapsize = 0; // Initializing heap size to 0
// Helper function to return the index of parent of a node in binary tree
int parent(int i){
    return ceil((i >> 1) - 1);
}
// Helper function to swap two packets
void swap(PACKET packet1,PACKET packet2){
    PACKET temp = packet1;
    packet1 = packet2;
    packet2 = temp;
}
// Function to max heapify - maintain the max heap priority queue with each incoming packet
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
// Helper function to enqueue a new packet into the max heap
void increaseKey(int i, PACKET packet){
    max_heap_queue[i] = packet;
    while(i > 0 && max_heap_queue[parent(i)].key < max_heap_queue[i].key){
        swap(max_heap_queue[parent(i)],max_heap_queue[i]);
        i = parent(i);
    }
}
// Function to add a new packet to the max heap queue
void max_heap_enqueue(PACKET packet){
    ++heapsize;
    max_heap_queue[heapsize].key = INT_MIN;
    increaseKey(heapsize-1,packet);
}
// Function to dequeue a packet when it is ready to be transmitted
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

double current_time = 0.000000;
double t_next_enqueue;
double t_next_dequeue;
int count_http_drop = 0, count_video_drop = 0;
int count_http_inserted = 0, count_video_inserted = 0;
bool is_dropped = false;
int i = 0;

int http_inserted[16];
int video_inserted[16];
int http_sent[16];
int video_sent[16];
int http_dropped[16];
int video_dropped[16];

PACKET packet;
FILE *output_file;

void drop_stale_packets(){
    do{
        PACKET packet = max_heap_queue[0];
        is_dropped = false;
        if(packet.type == 0){
            if(fabs(current_time - packet.t_arrival) > 15.000000){
                is_dropped = true;
                //packet = dequeue_packet();
                packet = max_heap_dequeue();
                count_http_drop++;
                printf("%lf\t%s\t%s\n",current_time,"http","DROP");
                fprintf(output_file,"%lf\t%s\t%s\n",current_time,"http","DROP");
                http_dropped[(int)current_time]++;
                t_next_dequeue = ((double)max_heap_queue[0].size)/64000.0;
            }
        }
        else{
            if(fabs(current_time - packet.t_arrival) > 1.000000){
                is_dropped = true;
                packet = max_heap_dequeue();
                count_video_drop++;
                printf("%lf\t%s\t%s\n",current_time,"video","DROP");
                fprintf(output_file,"%lf\t%s\t%s\n",current_time,"video","DROP");
                video_dropped[(int)current_time]++;
                t_next_dequeue = ((double)max_heap_queue[0].size)/64000.0;
            }
        }
    }while(is_dropped && !(heapsize == 0));
}

void send_packet(){
    current_time += t_next_dequeue;
    packet = max_heap_dequeue();
    if(packet.type == 0){
        printf("%lf\t%s\t%s\n",current_time,"http","SENT");
        fprintf(output_file,"%lf\t%s\t%s\n",current_time,"http","SENT");
        http_sent[(int)current_time]++;
    }
    else{
        printf("%lf\t%s\t%s\n",current_time,"video","SENT");
        fprintf(output_file,"%lf\t%s\t%s\n",current_time,"video","SENT");
        video_sent[(int)current_time]++;
    }
    t_next_enqueue -= t_next_dequeue;
    if(!(heapsize==0)){
        t_next_dequeue = ((double)max_heap_queue[0].size)/64000.0;
    }
}

void receive_packet(){
    current_time += t_next_enqueue;
    t_next_dequeue -= t_next_enqueue;
    if(packets[i].type == 0){
        max_heap_enqueue(packets[i]);
        count_http_inserted++;
        http_inserted[(int) current_time]++;
    }
    else{
        max_heap_enqueue(packets[i]);
        count_video_inserted++;
        video_inserted[(int) current_time]++;
    }
    t_next_enqueue = packets[++i].t_arrival;
    t_next_enqueue -= current_time;
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Usage ./a.out <filename>\n");
        exit(1);
    }
    output_file = fopen("output-2.dat","w");
    fprintf(output_file,"%s\t\t%s\t%s\n","time","type","action");
    read_packets_from_input_file(argv);
    t_next_enqueue = packets[i].t_arrival;
    while (i <= file_size) {
        if(heapsize == 0){
            receive_packet();
            t_next_dequeue = ((double)max_heap_queue[0].size)/64000.0;
        }
        drop_stale_packets();
        if(t_next_dequeue < t_next_enqueue && !(heapsize == 0)){
            send_packet();
        }
        else if(t_next_enqueue <= t_next_dequeue){
            receive_packet();
        }
    }
    printf("%s : %f \n","Video Packet Drop ", (float)count_video_drop/(float)count_video_inserted * 100.0);
    printf("%s : %f \n","HTTP Packet Drop ", (float)count_http_drop/(float)count_http_inserted * 100.0);
    for(int i = 0; i < 16; i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i,http_inserted[i]*80,video_inserted[i]*400,http_sent[i]*80,video_sent[i]*400,http_dropped[i]*80,video_dropped[i]*400);
    }
    fclose(output_file);
    return 0;
}

