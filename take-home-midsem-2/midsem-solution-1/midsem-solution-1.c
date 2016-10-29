#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MAX_QUEUE_SIZE 10000

typedef struct PACKET{
    int type;
    double t_arrival;
    int size;
}PACKET;

PACKET packets[MAX_QUEUE_SIZE];
int file_size = 0;

PACKET generate_packet(int type, double t_arrival, int size){
    PACKET packet;
    packet.type = type;
    packet.t_arrival = t_arrival;
    packet.size = size;
    return packet;
}

void read_packets_from_input_file(char** argv){
    FILE *file = fopen(argv[1],"r");
    if(file == NULL){
        printf("Unable to open file\n");
        exit(1);
    }
    char line[128];
    int i = 0;
    int type,size;
    double t_arrival;
    while(fgets(line,sizeof line,file) != NULL){
        bool isTime = true;
        char *pos;
        if ((pos=strchr(line,'\n')) != NULL)
            *pos = '\0';
        char *token;
        token = strtok(line," ");
        while (token != NULL) {
            if(isTime){
                sscanf(token,"%lf",&t_arrival);
            }
            else{
                if(strcmp(token,"http") == 0){
                    type = 0;
                    size = 80;
                }
                else if(strcmp(token,"video") == 0){
                    type = 1;
                    size = 400;
                }
            }
            isTime = false;
            token = strtok(NULL," ");
        }
        PACKET packet = generate_packet(type,t_arrival,size);
        packets[i++] = packet;
        file_size++;
    }
}

PACKET queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

bool queue_full(){
    if((front == rear + 1) || (front == 0 && rear == MAX_QUEUE_SIZE - 1)){
        return true;
    }
    return false;
}

bool queue_empty(){
    if(front == -1){
        return true;
    }
    return false;
}

void enqueue_packet(PACKET packet){
    if(queue_full())
        printf("Queue overflow\n");
    else{
        if(rear == -1){
            rear = 0;
            front = 0;
        }
        else if(rear == MAX_QUEUE_SIZE - 1)
            rear = 0;
        else
            rear++;
        queue[rear] = packet;
    }
}

PACKET dequeue_packet(){
    PACKET packet;
    if(queue_empty())
        printf("Queue underflow\n");
    else{
        packet = queue[front];
        if(front == rear){
            front = -1;
            rear = -1;
        }
        else if(front == MAX_QUEUE_SIZE - 1)
            front = 0;
        else
            front++;
    }
    return packet;
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

void drop_stale_packets(){
    do{
        PACKET packet = queue[front];
        is_dropped = false;
        if(packet.type == 0){
            if(fabs(current_time - packet.t_arrival) > 15.000000){
                is_dropped = true;
                packet = dequeue_packet();
                count_http_drop++;
                printf("%lf\t%s\t%s\n",current_time,"http","DROP");
                http_dropped[(int)current_time]++;
                t_next_dequeue = ((double)queue[front].size)/64000.0;
            }
        }
        else{
            if(fabs(current_time - packet.t_arrival) > 1.000000){
                is_dropped = true;
                packet = dequeue_packet();
                count_video_drop++;
                printf("%lf\t%s\t%s\n",current_time,"video","DROP");
                video_dropped[(int)current_time]++;
                t_next_dequeue = ((double)queue[front].size)/64000.0;
            }
        }
    }while(is_dropped && !queue_empty());
}

void send_packet(){
    current_time += t_next_dequeue;
    packet = dequeue_packet();
    if(packet.type == 0){
        printf("%lf\t%s\t%s\n",current_time,"http","SENT");
        http_sent[(int)current_time]++;
    }
    else{
        printf("%lf\t%s\t%s\n",current_time,"video","SENT");
        video_sent[(int)current_time]++;
    }
    t_next_enqueue -= t_next_dequeue;
    if(!queue_empty()){
        t_next_dequeue = ((double)queue[front].size)/64000.0;
    }
}

void receive_packet(){
    current_time += t_next_enqueue;
    t_next_dequeue -= t_next_enqueue;
    if(packets[i].type == 0){
        enqueue_packet(packets[i]);
        count_http_inserted++;
        http_inserted[(int) current_time]++;
    }
    else{
        enqueue_packet(packets[i]);
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
    read_packets_from_input_file(argv);
    t_next_enqueue = packets[i].t_arrival;
    while (i <= file_size) {
        if(queue_empty()){
            receive_packet();
            t_next_dequeue = ((double)queue[front].size)/64000.0;
        }
        drop_stale_packets();
        if(t_next_dequeue < t_next_enqueue && !queue_empty()){
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
    return 0;
}
