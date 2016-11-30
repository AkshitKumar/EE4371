/*
 * Name : Akshit Kumar
 * Roll No : EE14B127
 * Solution to the genelogy question in the take home endsem examination - finds the original ancestor, no. of descendants and people who lived long enough to see their great grandchildren
 */
// Inclusion of necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

// defining the max size of the stack
#define MAXSIZE 1000

// Definition of Node struct - contains the name and age of death of the person
typedef struct Node{
	char name[20];
	int age_of_death;
}Node;

/*
 * Definition of the Connections struct
 * parent_index : stores the index of parent ie makes an edge connection to the parent
 * name[20] : contains the name of the node
 * parent_age : stores the age of the parent when that node was born
 * num_children : stores the number of children of that node
 * children_index[10] : stores the indices of the children ie makes edge connections to the children
 */
typedef struct Connections{
	int parent_index;
	char name[20];
	int parent_age;
	int children_index[10];
	int num_children;
}Connections;

// Definition of the Edge struct - contains the parent name, child name and parent age at time of child's birth
typedef struct Edge{
	char parent_name[20];
	char child_name[20];
	int parent_age;
}Edge;

/* Implementation of stack for performing DFS*/
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

/* Array of structs */
Node nodes[100];
Edge edges[100];
Connections connections[100];

int node_count = 0;
int edge_count = 0;

// Helper function to read a line from the file and make a node
void make_node(char line[256]){
	char name[20];
	char age[20];
	int age_of_death;
	sscanf(line,"%s %s",name,age); // Get the name and age of the person
	if(strcmp(age,"-") == 0){
		age_of_death = INT_MAX; // If the age of death is - , assign is maximum possible value
	}
	else{
		sscanf(age,"%d",&age_of_death);
	}
	strcpy(nodes[node_count].name,name);
	nodes[node_count++].age_of_death = age_of_death;
}

// Helper function to read a line from the file and make an edge
void make_edge(char line[256]){
	char parent_name[20];
	char child_name[20];
	int parent_age;
	sscanf(line,"%s %s %d",parent_name,child_name,&parent_age); // get the name of the parent, child and parent age
	strcpy(edges[edge_count].parent_name,parent_name);
	strcpy(edges[edge_count].child_name,child_name);
	edges[edge_count++].parent_age = parent_age;
}

// Helper function to find the index corresponding to a name
int find_index_by_name(char name[20]){
	int i = 0;
	while(i < node_count){
		if(strcmp(nodes[i++].name,name) == 0)
			break;
	}
	return i-1;
}

// Helper function to make all the connections - ie connect each node to its parent and children
void make_connections(){
	// Initialising the nodes
	for(int i = 0; i < node_count; i++){
		strcpy(connections[i].name,nodes[i].name);
		connections[i].parent_index = -1; // setting parent index as -1 
		connections[i].num_children = 0; // setting the number of children as 0
	}
	// Make parent connections
	// Iterate through all the nodes
	for(int i = 0; i < node_count; i++){
		int t = 0;
		// For each node, iterate through all the edge connections
		// If the node is the first name, then assign its index to all it's children nodes
		for(int j = 0; j < edge_count; j++){
			if(strcmp(connections[i].name,edges[j].parent_name) == 0){
				connections[i].num_children++;
				connections[i].children_index[t++] = find_index_by_name(edges[j].child_name);
			}
		}
	}
	// Make children connections
	// Iterate through all the nodes
	for(int i = 0; i < node_count; i++){
		// For each node, iterate through all the edge connections
		// If the node is second name, get the parent index and parent age
		for(int j = 0; j < edge_count; j++){
			if(strcmp(connections[i].name,edges[j].child_name) == 0){
				connections[i].parent_index = find_index_by_name(edges[j].parent_name);
				connections[i].parent_age = edges[j].parent_age;
			}
		}
	}
}

/*
 * This is the function for finding the original ancestor
 * The algorithm is pretty straight forward.
 * Iterate through the Connections array and find a node which doesn't point to a parent but has non zero children
 * Return that index to find the name of the node
 */
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

/*
 * This is the function for finding the descendants of a given index
 * The algorithm for finding the descendants is a Depth First Search returning the number of visited nodes
 */
int count_descendants(int index){
	// adding the node to the stack
	push(index);
	int gen_count = 0;
	// initialise the visited array
	bool visited[100] = {false};
	// while the stack is not empty
	while(top != -1){
		int ele = pop(); // pop the top most element
		visited[ele] = true; // mark it visited
		// for all the children of that node, which are not visited, add them to the stack
		for(int i = 0; i < connections[ele].num_children;i++){
			if(visited[connections[ele].children_index[i]] != true){
				push(connections[ele].children_index[i]);
			}
		}
	}
	int count = 0;
	// count the number of visited nodes
	for(int i = 0; i < node_count; i++){
		if(visited[i])
			count++;
	}
	// return the number of nodes visited except for itself
	return count - 1;
}

// Function to print all the descendants
void print_descendants(){
	printf("Printing the number of descendants\n");
	for(int i = 0; i < node_count; i++){
		printf("%s : %d\n",connections[i].name,count_descendants(i));
	}
}

// Helper function to find the age of great grand father at the time of the birth of the particular node
int find_great_grand_father_age_at_birth(int index){
	int age = connections[index].parent_age;
	// backtrack to its parent
	index = connections[index].parent_index;
	age += connections[index].parent_age;
	// backtrack to its grandparent
	index = connections[index].parent_index;
	age += connections[index].parent_age;
	return age; // return the age of the great grand father
}

// Function to find the great grand children of each node
/*
 The algorithm used is a brute force search of all the children
 There are three levels - children, grand children and great grand children
 Iterate through all the children and find all the grand children
 Then iterate through all the grand children and find all the great grand children
*/
void find_great_grand_children(int index){
	int level1[100] = {-1};
	int level2[100] = {-1};
	int level3[100] = {-1};
	int great_grand_children[100];
	int level1_count = 0;
	int level2_count = 0;
	int level3_count = 0;
	int ggc_count = 0;
	// iterate through all the children
	for(int i = 0; i < connections[index].num_children; i++){
		level1[level1_count++] = connections[index].children_index[i];
	}
	// iterate through all the grand chilren
	for(int j = 0; j < level1_count; j++){
		for(int i = 0 ; i < connections[level1[j]].num_children;i++){
			level2[level2_count++] = connections[level1[j]].children_index[i];
		}
	}
	// iterate through all the great grand children
	for(int k = 0; k < level2_count; k++){
		for(int l = 0; l < connections[level2[k]].num_children;l++){
			level3[level3_count++] = connections[level2[k]].children_index[l];
		}
	}
	// print the great grand children
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
		// check if there is overlap in the lives of great grand father and their great grand children
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
	}
	printf("\n");
}

// Printing the great grand children for all the nodes
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
	// Reading in the file and making node and edge arrays
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
	make_connections(); // make the connections
	// Printing out the solution to mentioned questions
	printf("Original Ancestor : %s\n",nodes[find_ancestor()].name);
	print_descendants();
	print_great_grand_children();
	return 0;
}