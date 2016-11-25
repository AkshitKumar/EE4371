// Including the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

int W;
int weights[2000];

int getLimit(char line[]){
	char *pos;
	int W;
	if((pos = strchr(line,'/n')) != NULL){
		*pos = '\0';
	}
	char *token;
	token = strtok(line," ");
	while(token != NULL){
		sscanf(token,"%d",&W);
		token = strtok(NULL," ");
	}
	return W;
}

int getWeights(char line[]){
	char *pos;
	if((pos = strchr(line,'\n')) != NULL){
		*pos = '\0';
	}
	char *token;
	token = strtok(line," ");
	int num;
	int i = 0;
	while(token != NULL){
		sscanf(token,"%d",&num);
		weights[i++] = num;
		token = strtok(NULL," ");
	}
	return i;
}

void printWeights(int num_weights){
	for(int i = 0; i < num_weights; i++){
		printf("%d ",weights[i]);
	}
	printf("\n");
}

int main(int argc,char **argv){
	if(argc != 2){
		printf("Usage ./a.out <filename>");
		exit(1);
	}
	FILE *file = fopen(argv[1],"r");
	if(file == NULL){
		printf("Unable to open file");
		exit(1);
	}
	char line[2000];
	char line_number = 0;
	while(fgets(line,sizeof line,file) != NULL){
		if(line[0] == '#' || line[0] == '\n'){
			continue;
		}
		else{
			line_number++;
			if(line_number % 2 != 0){
				printf("Limit is : %d \n",getLimit(line));
			}
			else if(line_number % 2 == 0){
				printWeights(getWeights(line));
			}
		}
	}
}
