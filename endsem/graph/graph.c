#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

#define MAXSIZE 1000

typedef struct Node{
	char name[20];
	int age_of_death;
}Node;

typedef struct Connections{
	int parent_index;
	char name[20];
	int parent_age;
	int children_index[10];
	int num_children;
}Connections;

typedef struct Edge{
	char parent_name[20];
	char child_name[20];
	int parent_age;
}Edge;

int stack[MAXSIZE];
int top = -1;

int isempty(){
	if(top == -1){
		return 1;
	}
	else{
		return 0;
	}
}

int isfull(){
	if(top == MAXSIZE){
		return 1;
	}
	else{
		return 0;
	}
}

int peek(){
	return stack[top];
}

int pop(){
	int data;
	if(!isempty()){
		data = stack[top];
		top = top - 1;
	}else{
		printf("Stack empty");
	}
	return data;
}

void push(int data){
	if(!isfull()){
		top = top + 1;
		stack[top] = data;
	}else{
		printf("Stack overflow");
	}
}

Node nodes[100];
Edge edges[100];
Connections connections[100];

int node_count = 0;
int edge_count = 0;

void make_node(char line[256]){
	char name[20];
	char age[20];
	int age_of_death;
	sscanf(line,"%s %s",name,age);
	if(strcmp(age,"-") == 0){
		age_of_death = INT_MAX;
	}
	else{
		sscanf(age,"%d",&age_of_death);
	}
	strcpy(nodes[node_count].name,name);
	nodes[node_count++].age_of_death = age_of_death;
}

void make_edge(char line[256]){
	char parent_name[20];
	char child_name[20];
	int parent_age;
	sscanf(line,"%s %s %d",parent_name,child_name,&parent_age);
	strcpy(edges[edge_count].parent_name,parent_name);
	strcpy(edges[edge_count].child_name,child_name);
	edges[edge_count++].parent_age = parent_age;
}

int find_index_by_name(char name[20]){
	int i = 0;
	while(i < node_count){
		if(strcmp(nodes[i++].name,name) == 0)
			break;
	}
	return i-1;
}

void make_connections(){
	for(int i = 0; i < node_count; i++){
		strcpy(connections[i].name,nodes[i].name);
		connections[i].parent_index = -1;
		connections[i].num_children = 0;
	}
	// Make parent connections
	for(int i = 0; i < node_count; i++){
		int t = 0;
		for(int j = 0; j < edge_count; j++){
			if(strcmp(connections[i].name,edges[j].parent_name) == 0){
				connections[i].num_children++;
				connections[i].children_index[t++] = find_index_by_name(edges[j].child_name);
			}
		}
	}
	// Make children connections
	for(int i = 0; i < node_count; i++){
		for(int j = 0; j < edge_count; j++){
			if(strcmp(connections[i].name,edges[j].child_name) == 0){
				connections[i].parent_index = find_index_by_name(edges[j].parent_name);
				connections[i].parent_age = edges[j].parent_age;
			}
		}
	}
}

int find_ancestor(){
	int ans;
	for(int i = 0; i < node_count; i++){
		if(connections[i].parent_index == -1 && connections[i].num_children != 0){
			ans = i;
			break;
		}
	}
	return ans;
}

int count_descendants(int index){
	push(index);
	int gen_count = 0;
	bool visited[100] = {false};
	while(top != -1){
		int ele = pop();
		visited[ele] = true;
		for(int i = 0; i < connections[ele].num_children;i++){
			if(visited[connections[ele].children_index[i]] != true){
				push(connections[ele].children_index[i]);
			}
		}
	}
	int count = 0;
	for(int i = 0; i < node_count; i++){
		if(visited[i])
			count++;
	}
	return count - 1;
}

void print_descendants(){
	printf("Printing the number of descendants\n");
	for(int i = 0; i < node_count; i++){
		printf("%s : %d\n",connections[i].name,count_descendants(i));
	}
}


int find_great_grand_father_age_at_birth(int index){
	int age = connections[index].parent_age;
	index = connections[index].parent_index;
	age += connections[index].parent_age;
	index = connections[index].parent_index;
	age += connections[index].parent_age;
	return age;
}

void find_great_grand_children(int index){
	int level1[100] = {-1};
	int level2[100] = {-1};
	int level3[100] = {-1};
	int great_grand_children[100];
	int level1_count = 0;
	int level2_count = 0;
	int level3_count = 0;
	int ggc_count = 0;
	for(int i = 0; i < connections[index].num_children; i++){
		level1[level1_count++] = connections[index].children_index[i];
	}
	for(int j = 0; j < level1_count; j++){
		for(int i = 0 ; i < connections[level1[j]].num_children;i++){
			level2[level2_count++] = connections[level1[j]].children_index[i];
		}
	}
	for(int k = 0; k < level2_count; k++){
		for(int l = 0; l < connections[level2[k]].num_children;l++){
			level3[level3_count++] = connections[level2[k]].children_index[l];
		}
	}
	printf("Great Grand-Children of %s : ", nodes[index].name);
	if(level3_count != 0){
		for(int t = 0; t < level3_count;t++){
			printf("%s ",nodes[level3[t]].name);
		}
	}
	else{
		printf("NIL");
	}
	if(level3_count != 0){
		for(int t = 0; t < level3_count;t++){
			if(find_great_grand_father_age_at_birth(level3[t]) <= nodes[index].age_of_death){
				great_grand_children[ggc_count++] = level3[t];
			}
		}
		printf("\n%s lived long enough to see : ",nodes[index].name);
		if(ggc_count > 0){
			for(int i = 0; i < ggc_count; i++){
				printf("%s ",nodes[great_grand_children[i]].name);
			}
		}
		else{
			printf("NIL");
		}
		//printf("\n");
	}
	printf("\n");
}

void print_great_grand_children(){
	for(int i = 0; i < node_count;i++){
		find_great_grand_children(i);
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
	char line[128];
	while(fgets(line,sizeof(line),file) != NULL){
		if(line[0] == '#' || line[0] == '\n'){
			continue;
		}
		else{
			char *pos;
			if ((pos=strchr(line, '\n')) != NULL)
				*pos = '\0';
			char str1[20];
			char str2[20];
			char str3[20];
			if(sscanf(line,"%s %s %s",str1,str2,str3) == 2){
				make_node(line);
			}
			else if(sscanf(line,"%s %s %s",str1,str2,str3) == 3){
				make_edge(line);
			}
		}
	}
	/*
	for(int i = 0; i < node_count;i++){
		printf("%s %d\n",nodes[i].name,nodes[i].age_of_death);
	}
	for(int j = 0; j < edge_count;j++){
		printf("%s %s %d\n",edges[j].parent_name,edges[j].child_name,edges[j].parent_age);
	}
	for(int i = 0; i < node_count; i++){
		for(int j = 0; j < connections[i].num_children; j++){
			printf("%s ",nodes[connections[i].children_index[j]].name);
		}
		printf("\n");
	}
	*/
	make_connections();
	printf("Original Ancestor : %s\n",nodes[find_ancestor()].name);
	print_descendants();
	print_great_grand_children();
	return 0;
}