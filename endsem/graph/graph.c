#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

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
	*/
	make_connections();
	printf("%s\n",nodes[find_ancestor()].name);
	return 0;
}