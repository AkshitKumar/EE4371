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
int knapsack_elements[2000];

int get_limit(char line[]){
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

int get_weights(char line[]){
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

int get_knapsack_elements(int W,int n){
	int line = W;
	int i = n;
	int num_elements = 0;
	while(i > 0){
		if(dp[i][line].weight > dp[i-1][line].weight){
			knapsack_elements[num_elements++] = weights[i];
			line = line - weights[i];
			i--;
		}
		else{
			i--;
		}
	}
	return num_elements;
}

void print_dp_matrix(int W, int n){
	for(int i = 0; i <= n; i++){
		for(int j = 0; j <= W; j++){
			printf("%d ", dp[i][j].weight);
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

void print_knapsack_elements(int N){
	for(int i = 0; i < N ; i++){
		printf("%d ",knapsack_elements[i]);
	}
	printf("\n");
}

int main(int argc,char **argv){
	if(argc != 2){
		printf("Usage ./a.out <filename>\n");
		exit(1);
	}
	FILE *file = fopen(argv[1],"r");
	if(file == NULL){
		printf("Unable to open file\n");
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
				W = get_limit(line);
			}
			else if(line_number % 2 == 0){
				n = get_weights(line);
				line_number -= 2;
			}	
			if(line_number == 0){
				printf("DP Solution : %d\n", dp_knapsack_solution(W,n));
				print_knapsack_elements(get_knapsack_elements(W,n));
			}
		}
	}
	return 0;
}
