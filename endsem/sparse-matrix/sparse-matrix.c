#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

typedef struct Node{
	int row;
	int col;
	double val;
	struct Node* next;
}Node;

struct Node* rows[500] = {NULL};
struct Node* cols[500] = {NULL};

int x[500];

void insert_node_row_wise(Node *head, int row, int col, double val){
	Node *new_node;
	new_node = (Node *) malloc(sizeof(Node));
	new_node->row = row;
	new_node->col = col;
	new_node->val = val;
	new_node->next = NULL;
	if(rows[row] == NULL){	
		rows[row] = new_node;
	}
	else{
		if(col < rows[row]->col){
			new_node->next = rows[row];
			rows[row] = new_node;
		}
		else{
		Node *temp,*prev;
		temp = rows[row];
		while(temp != NULL && temp->col < col){
			prev = temp;
			temp = temp->next;
		}
		new_node->next = temp;
		prev->next = new_node;
		}
	}
}


void insert_node_col_wise(Node *head, int row, int col, double val){
	Node *new_node;
	new_node = (Node *) malloc(sizeof(Node));
	new_node->row = row;
	new_node->col = col;
	new_node->val = val;
	new_node->next = NULL;
	if(cols[col] == NULL){
		cols[col] = new_node;
	}
	else{
		if(row < cols[col]->row){
			new_node->next = cols[col];
			cols[col] = new_node;
		}
		else{
		Node *temp,*prev;
		temp = cols[col];
		while(temp != NULL && temp->row < row){
			prev = temp;
			temp = temp->next;
		}
		new_node->next = temp;
		prev->next = new_node;
		}
	}
}

void multiply_a_into_x(){
	double result[500] = {0.00};
	for(int i = 0; i < 500; i++){
		Node*temp = rows[i];
		while(temp != NULL){
			result[i] = result[i] + (temp->val * (double) x[temp->col]);
			temp = temp->next;
		}
	}
	FILE *file = fopen("output2a.dat","w");
	fprintf(file,"Solution of A.x is :\n");
	for(int i = 0; i < 500; i++){
		printf("%lf\n",result[i]);
		fprintf(file,"%lf\n",result[i]);
	}
	fclose(file);	
}


void multiply_x_transpose_into_a(){
	double result[500] = {0.00};
	for(int i = 0; i < 500; i++){
		Node* temp = cols[i];
		while(temp != NULL){
			result[i] = result[i] + (temp->val * (double)x[temp->row]);
			temp = temp->next;
		}
	}
	FILE *file = fopen("output2b.dat","w");
	fprintf(file,"Solution of x'.A is : \n");
	for(int i = 0; i < 500; i++){
		printf("%lf ",result[i]);
		fprintf(file,"%lf ",result[i]);
	}
	printf("\n");
	fclose(file);	
}

void print_row(Node *head){
	Node *temp = head;
	while(temp != NULL){
		printf("%d %d %lf\n",temp->row,temp->col,temp->val);
		temp = temp->next;
	}
}


int main(int argc,char** argv){
	if(argc != 2){
		printf("Usage ./a.out <filename>\n");
		exit(1);
	}
	FILE *file = fopen(argv[1],"r");
	if(file == NULL){
		printf("Unable to open file\n");
		exit(1);
	}
	int i = 0;
	char line[256];
	while(fgets(line,sizeof(line),file) != NULL){
		char *pos;
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';
		if(line[0] == '#' || line[0] == '\n'){
			continue;
		}
		else{
			int row,col,x_val;
			double val;
			if(sscanf(line,"%d %d %lf",&row,&col,&val) == 3){
				insert_node_row_wise(rows[row],row,col,val);
				insert_node_col_wise(cols[col],row,col,val);
			}
			else if(sscanf(line,"%d",&x_val) == 1){
				x[i++] = x_val;
			}
		}
	}
	multiply_a_into_x();
	multiply_x_transpose_into_a();
	return 0;
}