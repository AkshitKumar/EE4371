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
}PACKET;

// Instantiating an array of structure PACKET to hold all the packets read from the input file ie midterm-input.dat
PACKET packets[MAX_QUEUE_SIZE];
int file_size = 0; // Initialising the file size to 0

/*
 Function to generate a new packet
 @param type : type of the packet
 @param t_arrival : arrival time of the packet
 @param size : size of the packet
 */
PACKET generate_packet(int type, double t_arrival, int size){
    PACKET packet;
    packet.type = type;
    packet.t_arrival = t_arrival;
    packet.size = size;
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

PACKET http_queue[MAX_QUEUE_SIZE];
PACKET video_queue[MAX_QUEUE_SIZE];
int http_front = -1,http_rear = -1;
int video_front = -1, video_rear = -1;

bool http_queue_full(){
    if((http_front == http_rear + 1) || (http_front == 0 && http_rear == MAX_QUEUE_SIZE - 1))
        return true;
    return false;
}

bool video_queue_full(){
    if((video_front == video_rear + 1) || (video_front == 0 && video_rear == MAX_QUEUE_SIZE - 1))
        return true;
    return false;
}

bool http_queue_empty(){
    if(http_front == -1)
        return true;
    return false;
}

bool video_queue_empty(){
    if(video_front == -1)
        return true;
    return false;
}

void enqueue_http_packet(PACKET packet){
    if(http_queue_full())
        printf("HTTP Queue Overflow\n");
    else{
        if(http_rear == -1){
            http_front = 0;
            http_rear = 0;
        }
        else if(http_rear == MAX_QUEUE_SIZE - 1)
            http_rear = 0;
        else
            http_rear++;
        http_queue[http_rear] = packet;
    }
}

void enqueue_video_packet(PACKET packet){
    if(video_queue_full())
        printf("Video Queue Overflow\n");
    else{
        if(video_rear == -1){
            video_front = 0;
            video_rear = 0;
        }
        else if(video_rear == MAX_QUEUE_SIZE - 1)
            video_rear = 0;
        else
            video_rear++;
        video_queue[video_rear] = packet;
    }
}

PACKET dequeue_http_packet(){
    PACKET packet;
    if(http_queue_empty())
        printf("HTTP Queue Underflow");
    else{
        packet = http_queue[http_front];
        if(http_front == http_rear){
            http_front = -1;
            http_rear = -1;
        }
        else{
            http_front = (http_front + 1) % MAX_QUEUE_SIZE;
        }
    }
    return packet;
}

PACKET dequeue_video_packet(){
    PACKET packet;
    if(video_queue_empty())
        printf("HTTP Queue Underflow");
    else{
        packet = video_queue[video_front];
        if(video_front == video_rear){
            video_front = -1;
            video_rear = -1;
        }
        else{
            video_front = (video_front + 1) % MAX_QUEUE_SIZE;
        }
    }
    return packet;
}

double current_time = 0.00000;
double t_next_enqueue;
double t_next_http_dequeue;
double t_next_video_dequeue;
PACKET http_packet;
PACKET video_packet;
FILE *output_file;
bool is_http_dropped = false;
bool is_video_dropped = false;
int i = 0;
int can_drop;
int count_http_drop = 0, count_video_drop = 0;
int count_http_inserted = 0, count_video_inserted = 0;

int http_inserted[16];
int video_inserted[16];
int http_sent[16];
int video_sent[16];
int http_dropped[16];
int video_dropped[16];

void drop_stale_http_packets(){
    if(!http_queue_empty()){
        http_packet = http_queue[http_front];
        do{
            is_http_dropped = false;
            if(fabs(current_time - http_packet.t_arrival) > 15.000000){
                is_http_dropped = true;
                http_packet = dequeue_http_packet();
                count_http_drop++;
                http_dropped[(int)current_time]++;
                printf("%lf\t%s\t%s\n",current_time,"http","DROP");
                if(!http_queue_empty())
                    t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
            }
        }while(is_http_dropped && !http_queue_empty());
    }
}

void drop_stale_video_packets(){
    if(!video_queue_empty()){
        video_packet = video_queue[video_front];
        do{
            is_video_dropped = false;
            if(fabs(current_time - video_packet.t_arrival) > 1.000000){
                is_video_dropped = true;
                video_packet = dequeue_video_packet();
                count_video_drop++;
                video_dropped[(int)current_time]++;
                printf("%lf\t%s\t%s\n",current_time,"video","DROP");
                if(!video_queue_empty())
                    t_next_video_dequeue = ((double)video_queue[video_front].size)/64000.0;
            }
        }while(is_video_dropped && !video_queue_empty());
    }
}

/*
void send_video_packet(){
    current_time += t_next_video_dequeue;
    packet = dequeue_video_packet();
    printf("%lf\t%s\t%s\n",current_time,"video","SENT");
    t_next_enqueue -= t_next_video_dequeue;
    if(!video_queue_empty())
        t_next_video_dequeue = ((double)video_queue[video_front].size)/64000.0;
}

void send_http_packet(){
    current_time += t_next_http_dequeue;
    packet = dequeue_http_packet();
    printf("%lf\t%s\t%s\n",current_time,"http","SENT");
    t_next_enqueue -= t_next_http_dequeue;
    if(!http_queue_empty())
        t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
}

void receive_http_packet(){
    current_time += t_next_enqueue;
    t_next_http_dequeue -= t_next_enqueue;
    enqueue_http_packet(packets[i]);
    count_http_inserted++;
    t_next_enqueue = packets[++i].t_arrival;
    t_next_enqueue -= current_time;
}

void receive_video_packet(){
    current_time += t_next_enqueue;
    t_next_video_dequeue -= t_next_enqueue;
    enqueue_video_packet(packets[i]);
    count_video_inserted++;
    t_next_enqueue = packets[++i].t_arrival;
    t_next_enqueue -= current_time;
}
*/
int main(int argc,char** argv){
    if(argc != 2){
        printf("Usage ./a.out <filename>");
        exit(1);
    }
    read_packets_from_input_file(argv);
    t_next_enqueue = packets[i].t_arrival;
    //printf("%lf",t_next_enqueue);
    while(i < file_size){
        if(video_queue_empty() && http_queue_empty()){
            current_time += t_next_enqueue;
            if(packets[i].type == 0){
                enqueue_http_packet(packets[i]);
                count_http_inserted++;
                http_inserted[(int)current_time]++;
                t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
            }
            else{
                enqueue_video_packet(packets[i]);
                count_video_inserted++;
                video_inserted[(int)current_time]++;
                t_next_video_dequeue = ((double)video_queue[video_front].size)/64000;
            }
            t_next_enqueue = packets[++i].t_arrival;
            t_next_enqueue -= current_time;
        }
        drop_stale_http_packets();
        drop_stale_video_packets();
        /*
        if(!http_queue_empty()){
            http_packet = http_queue[http_front];
            do{
                is_http_dropped = false;
                if(fabs(current_time - http_packet.t_arrival) > 15.000000){
                    is_http_dropped = true;
                    http_packet = dequeue_http_packet();
                    count_http_drop++;
                    printf("%lf\t%s\t%s\n",current_time,"http","DROP");
                    if(!http_queue_empty())
                        t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
                }
            }while(is_http_dropped && !http_queue_empty());
        }
        if(!video_queue_empty()){
            video_packet = video_queue[video_front];
            do{
                is_video_dropped = false;
                if(fabs(current_time - video_packet.t_arrival) > 1.000000){
                    is_video_dropped = true;
                    video_packet = dequeue_video_packet();
                    count_video_drop++;
                    printf("%lf\t%s\t%s\n",current_time,"video","DROP");
                    if(!video_queue_empty())
                        t_next_video_dequeue = ((double)video_queue[video_front].size)/64000.0;
                }
            }while(is_video_dropped && !video_queue_empty());
        }
        */
        if(http_queue_empty() && !video_queue_empty()){
            if(t_next_video_dequeue < t_next_enqueue){
                current_time += t_next_video_dequeue;
                t_next_enqueue -= t_next_video_dequeue;
                video_packet = dequeue_video_packet();
                printf("%lf\t%s\t%s\n",current_time,"video","SENT");
                video_sent[(int)current_time]++;
                if(!video_queue_empty()){
                    t_next_video_dequeue = ((double)video_queue[video_front].size)/64000;
                }
            }
            else if(t_next_enqueue <= t_next_video_dequeue){
                current_time += t_next_enqueue;
                t_next_video_dequeue -= t_next_enqueue;
                if(packets[i].type == 0){
                    enqueue_http_packet(packets[i]);
                    count_http_inserted++;
                    http_inserted[(int)current_time]++;
                    t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
                }
                else{
                    enqueue_video_packet(packets[i]);
                    count_video_inserted++;
                    video_inserted[(int)current_time]++;
                    t_next_video_dequeue = ((double)video_queue[video_front].size)/64000;
                }
                t_next_enqueue = packets[++i].t_arrival;
                t_next_enqueue -= current_time;
            }
        }
        if(!http_queue_empty() && video_queue_empty()){
            if(t_next_http_dequeue < t_next_enqueue){
                current_time += t_next_http_dequeue;
                t_next_enqueue -= t_next_http_dequeue;
                http_packet = dequeue_http_packet();
                http_sent[(int)current_time]++;
                printf("%lf\t%s\t%s\n",current_time,"http","SENT");
                if(!http_queue_empty()){
                    t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
                    //printf("%lf\n",t_next_http_dequeue);
                }
            }
            else if(t_next_enqueue <= t_next_http_dequeue){
                current_time += t_next_enqueue;
                t_next_http_dequeue -= t_next_enqueue;
                if(packets[i].type == 0){
                    enqueue_http_packet(packets[i]);
                    count_http_inserted++;
                    http_inserted[(int)current_time]++;
                    t_next_http_dequeue = ((double)http_queue[http_front].size)/64000.0;
                }
                else{
                    enqueue_video_packet(packets[i]);
                    count_video_inserted++;
                    video_inserted[(int)current_time]++;
                    t_next_video_dequeue = ((double)video_queue[video_front].size)/64000;
                }
                t_next_enqueue = packets[++i].t_arrival;
                t_next_enqueue -= current_time;
            }
        }
        if(!http_queue_empty() && !video_queue_empty()){
            can_drop = count_video_inserted/10 - count_video_drop;
            while(!video_queue_empty() && can_drop){
                video_packet = dequeue_video_packet();
                count_video_drop++;
                printf("%lf\t%s\t%s\n",current_time,"video","DROP");
                video_dropped[(int)current_time]++;
                if(!video_queue_empty()){
                    t_next_video_dequeue = ((double)video_queue[video_front].size)/64000;
                }
                can_drop = count_video_inserted/10 - count_video_drop;
            }
            if(t_next_video_dequeue < t_next_enqueue && !video_queue_empty()){
                current_time += t_next_video_dequeue;
                t_next_enqueue -= t_next_video_dequeue;
                video_packet = dequeue_video_packet();
                printf("%lf\t%s\t%s\n",current_time,"video","SENT");
                video_sent[(int)current_time]++;
                if(!video_queue_empty()){
                    t_next_video_dequeue = ((double)video_queue[video_front].size)/64000;
                }
            }
        }
    }
    printf("%s : %f \n","Video Packet Drop ", (float)count_video_drop/(float)count_video_inserted * 100.0);
    printf("%s : %f \n","HTTP Packet Drop ", (float)count_http_drop/(float)count_http_inserted * 100.0);
    for(int i = 0; i < 16; i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i,http_inserted[i]*80,video_inserted[i]*400,http_sent[i]*80,video_sent[i]*400,http_dropped[i]*80,video_dropped[i]*400);
    }
    return 0;
}



