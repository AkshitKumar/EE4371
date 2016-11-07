#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MAX_QUEUE_SIZE 10000

typedef struct PACKET{
	int type;
	double tarrival;
	int size;
}PACKET;

PACKET packets[MAX_QUEUE_SIZE];
int filesize = 0;

void readInputFile(char** argv){
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL){
		printf("File didn't open properly\n");
		exit(1);
	}
	double tarrival;
	char type[128];
	int i = 0;
	while(fscanf(fp,"%lf %s",&tarrival,type)){
		if(strncmp(type,"http",4) == 0){
			PACKET p;
			p.type = 0;
			p.tarrival = tarrival;
			p.size = 80;
			packets[i++] = p;
		}
		else{
			PACKET p;
			p.type = 1;
			p.tarrival = tarrival;
			p.size = 400;
			packets[i++] = p;
		}
		filesize++;
	}
}

int main(int argc,char** argv){
	if(argc != 2){
		printf("Usage ./a.out <filename>\n");
		exit(1);
	}
	readInputFile(argv);
	for(int i = 0; i < filesize;i++){
		printf("%lf %d %d\n",packets[i].tarrival,packets[i].type,packets[i].size);
	}
	return 0;
}