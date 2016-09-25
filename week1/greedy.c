/*
    Name : Akshit Kumar
    Roll Number : EE14B!27
    Assignment : Greedy Graph Coloring Algorithm
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool colored[30]; // array to hold the status of vertice
int incidence_matrix[30][30]; // incidence matrix to determine edges between graphs
int newcolor = 1; // initialise new color to be assigned
int vertice_color[30]; // hold the new color assignment for the vertice
int num_vertices; // number of vertices in the graph
char vertice_names[30][128]; // mapping of vertice number to vertice name
char row[30]; // character array to get the rows of incidence matrix
bool extra[30]; // array to hold possible extra turns

// function to initialise the array of possible extra turns
void initialize_extra(){
    for(int i = 0; i < num_vertices; i++){
        extra[i] = true;
    }
}

// function to initialise the array holding status of vertices
void initialize_colored(){
    for(int i = 0; i <num_vertices; i++){
        colored[i] = false;
    }
}

// function to check if all vertices have been colored or not
bool isAllColoured(){
    bool flag = true;
    for(int i = 0; i < num_vertices; i++){
        if(!colored[i]){
            flag = false;
        }
    }
    return flag;
}

// function to check if a vertice is present in newcolor_stack or not
bool present_in_newcolor_stack(int newcolor_stack[num_vertices],int i){
    for(int p = 0; p < num_vertices; p++){
        if(newcolor_stack[p] == i){
            return true;
            break;
        }
    }
    return false;
}

// Implementation of greedy coloring algorithm
int greedy(){
    bool found;
    int count = 0; // holds the count of the stack
    int newcolor_stack[num_vertices]; // add vertices to the stack
    // initialise the newcolor_stack
    for(int i = 0; i < num_vertices; i++){
        newcolor_stack[i] = -1;
    }
    // iterate through all the vertices and check if not colored
    for(int v = 0; v < num_vertices; v++){
        if(!colored[v]){
            found = false;
            // iterate thorugh stack and look for edge
            for(int w = 0; w < count; w++){
                if(incidence_matrix[v][newcolor_stack[w]]){
                    found = true;
                }
            }
            // if edge is not found, assign a color and mark colored
            if(!found){
                colored[v] = true;
                vertice_color[v] = newcolor;
                newcolor_stack[count] = v;
                count++;
            }
        }
    }
    // finding all possible values for turns
    for(int w = 0; w < count; w++){
        for(int v = 0; v < num_vertices; v++){
            if(incidence_matrix[newcolor_stack[w]][v]){
                extra[v] = false;
            }
        }
    }
    // pruning the values of turns from the one which are already colored
    // gives the array of extra turns possible
    for(int i = 0; i < num_vertices; i++){
        if(extra[i]){
            if(present_in_newcolor_stack(newcolor_stack,i)){
                extra[i] = false;
            }
        }
    }

    return 1;
}

int main(int argc, char **argv){
    if(argc != 2){
        printf("Usage ./prog <filename>\n");
        exit(1);
    }
    FILE *file;
    file = fopen(argv[1],"r");
    if(file == NULL){
        printf("File cannot be opened.\n");
        exit(2);
    }
    // Reading the first number to get the number of vertices
    fscanf(file,"%1d",&num_vertices);
    // Loop to get the vertice Names
    for(int i = 0; i <= num_vertices; i++){
        fgets(vertice_names[i],128,file);
        char *pos;
        if((pos=strchr(vertice_names[i],'\n')) != NULL)
            *pos = '\0';
    }
    // Loop to populate the incidence_matrix
    for(int i = 0; i <= num_vertices; i++){
        fgets(row,30,file);
        char *pos;
        if((pos=strchr(vertice_names[i],'\n')) != NULL)
            *pos = '\0';
        for(int j = 0; j < num_vertices; j++){
            incidence_matrix[i][j] = row[j] - '0';
        }
    };

    // initialize colored and extra boolean arrays
    initialize_colored();
    initialize_extra();

    printf("Color\tTurns\t\tExtras\n");

    int count = 0;
    // Call greedy till all vertices are colored
    while(!isAllColoured()){
        greedy();
        printf("%d\t",newcolor);
        for(int i = 0; i < num_vertices; i++){
            if(vertice_color[i] == newcolor){
                printf("%s,",vertice_names[i+1]);
            }
        }
        printf("\t\t");
        for(int i = 0; i < num_vertices; i++){
            if(extra[i]){
                printf("%s,",vertice_names[i+1]);
            }
        }
        initialize_extra();
        printf("\n");
        newcolor++; // increment the newcolor for new color Assignment
    }
    return 0;
}

/*
Complexity of the algorithm : O(n^2)

Input:
6
AB
AC
AD
BA
BC
BD
000000
000101
000100
011000
000000
010000

Output:
Color	Turns	    Extras
1	AB,AC,AD,BC,
2	BA,BD,		    AB,BC,
*/
