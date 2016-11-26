// Including the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define max(a,b) (a > b ? a : b)

typedef struct Cost{
	int weight;
	int count;
}Cost;

Cost dp[2000][2000];

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
	int i = 1;
	while(token != NULL){
		sscanf(token,"%d",&num);
		weights[i++] = num;
		token = strtok(NULL," ");
	}
	return i-1;
}

int dp_knapsack_solution(int W, int n){
	for(int j = 0; j <= W; j++){
		dp[0][j].weight = 0;
		dp[0][j].count = 0;
	}
	for(int i = 1; i <= n ;i++){
		for(int j = 0; j <= W; j++){
			if(weights[i] > j){
				dp[i][j] = dp[i-1][j];
			}
			else{
				if(dp[i-1][j-weights[i]].weight + weights[i] > dp[i-1][j].weight && dp[i-1][j-weights[i]].weight + weights[i] < (float)j - log(dp[i-1][j-weights[i]].count + 1)){
					dp[i][j].weight = dp[i-1][j-weights[i]].weight + weights[i];
					dp[i][j].count = dp[i-1][j-weights[i]].count + 1;
				}
				else{
					dp[i][j] = dp[i-1][j];
				}
			}
		}
	}
	return dp[n][W].weight;
}

void print_dp_matrix(int W, int n){
	for(int i = 0; i <= n; i++){
		for(int j = 0; j <= W; j++){
			printf("%d ", dp[i][j].count);
		}
		printf("\n");
	}
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
	int W;
	int n;
	while(fgets(line,sizeof line,file) != NULL){
		if(line[0] == '#' || line[0] == '\n'){
			continue;
		}
		else{
			line_number++;
			if(line_number % 2 != 0){
				W = getLimit(line);
			}
			else if(line_number % 2 == 0){
				n = getWeights(line);
				line_number -= 2;
			}	
			if(line_number == 0){
				printf("DP Solution : %d\n", dp_knapsack_solution(W,n));
			}
		}
	}
	return 0;
}
