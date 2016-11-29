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

typedef struct row_header{
	struct Node* head;
}row_header;

typedef struct column_header{
	struct Node* head;
}column_header;

row_header rows[500];

int x[500];

void insert_node(Node *head, int row, int col, double val){
	if(head == NULL){
		head = (Node *) malloc(sizeof(Node));
		head->row = row;
		head->col = col;
		head->val = val;
		head->next = NULL;
		return;
	}
	Node *new_node;
	new_node = (Node *)malloc(sizeof(Node));
	new_node->row = row;
	new_node->col = col;
	new_node->val = val;
	new_node->next = NULL;
	Node *temp,*prev;
	temp = head->next;
	prev = head;
	while(temp != NULL && temp->col < col){
		prev = temp;
		temp = temp->next;
	}
	new_node->next = temp;
	prev->next = new_node;
}

void print_row(Node *head){
	Node *temp = head;
	while(temp != NULL){
		printf("%lf ",temp->val);
		temp = temp->next;
	}
	printf("\n");
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
		int row;
		int col;
		double val;
		int x_val;
		if(sscanf(line,"%d %d %lf",&row,&col,&val) == 3){
			printf("%d %d %lf\n",row,col,val);
			//insert_node(rows[row].head,row,col,val);
		}
		if(sscanf(line,"%d",&x_val) == 1){
			x[i++] = x_val;
		}
	}
	//print_row(rows[0].head);
	/*
	for(int i = 0; i < 500; i++){
		printf("%d\n",x[i]);
	}
	*/
	return 0;
}