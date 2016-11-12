/*
	Name : Akshit Kumar
	Roll No. : EE14B127
*/
// Including the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/*
    Numbers used for the maps
    * 1 : Clear cell
    * 2 : Obstacle
    * 3 : visited
    * 4 : On List of Consideration
    * 5 : start
    * 6 : destination
*/

// Defining a structure to hold the coordinate of each cell in the grid
typedef struct Coordinates{
    int x_index; 
    int y_index;
}Coordinates;

// Initializing the maz as given in the question according to the scheme described above
int maze[10][10] = {
     {5,1,1,1,1,1,1,1,1,1},
     {1,2,2,2,2,1,2,2,2,1},
     {1,2,1,2,1,1,1,1,2,2},
     {1,1,1,2,1,2,1,1,2,6},
     {1,2,2,2,1,2,2,2,2,1},
     {1,2,1,1,1,2,1,1,2,1},
     {1,2,1,2,1,2,1,1,1,1},
     {1,2,1,2,1,1,1,1,2,1},
     {1,2,1,2,2,2,1,2,2,1},
     {1,1,1,1,1,1,1,1,1,1}
 };

// Creating the a duplicate maze for the purposes of printing it out
char dup_maze[10][10] = {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

// Assigning the start and end corrdinates
Coordinates start_coord = {0,0};
Coordinates dest_coord = {3,9};

// Converting the 2D indices into a number for the purposes of giving a linear index
int createLinearIndex(Coordinates coord){
    return (coord.x_index * 10) + coord.y_index;
}

// Converting the linear index into a 2D index
Coordinates createCoordinates(int linearIndex){
    Coordinates coord;
    coord.y_index = linearIndex % 10;
    coord.x_index = linearIndex / 10;
    return coord;
}

// Initializing the distance array to maximum
int distanceArray[10][10] = {
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX}
};

// Holds the shortest distance to that grid cell
int distanceArray_copy[10][10] = {
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX},
    {INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX,INT_MAX}
};

// Initializing the parent array to 0 which holds the linear index of the parent grid from which it comes
int parent[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

int numExpanded = 0;

// Defining a structure to hold the coordinate of the point and min distance to that coordinate
typedef struct Info{
    Coordinates coord;
    int min_distance;
}Info;

// Function to return the minimum distance from the start along with the index of the point in the grid
Info minimumDistanceFromStart(int distanceArray[10][10]){
    Info info;
    info.coord.x_index = 0;
    info.coord.y_index = 0;
    info.min_distance = distanceArray[0][0];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(distanceArray[i][j] < info.min_distance){
                info.coord.x_index = i;
                info.coord.y_index = j;
                info.min_distance = distanceArray[i][j];
            }
        }
    }
    return info;
}

// Function to display the table 
void displayDistanceCopyArray(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(distanceArray_copy[i][j] == INT_MAX){
                printf("*\t");
            }
            else{
                printf("%d\t",distanceArray_copy[i][j]);
            }
        }
        printf("\n");
    }
}

// route array holds the linear indicies of the grid point which from the shortest path
int route[100];

int main(){
    bool found = false; // setting found to false
    distanceArray[start_coord.x_index][start_coord.y_index] = 0; // the distance of the start point to 0
    int min_distance;
    Coordinates current;
    // search while shortest path is not found
    while(!found){
    	// get the minimum distance from start along with the index
        min_distance = minimumDistanceFromStart(distanceArray).min_distance;
        current = minimumDistanceFromStart(distanceArray).coord;
        // if the index is not to the destination index then don't break out of the while loop
        if((current.x_index == dest_coord.x_index && current.y_index == dest_coord.y_index) || min_distance == INT_MAX){
            found = true;
            break;
        }
        // mark the current node as visited
        maze[current.x_index][current.y_index] = 3;
        // make the distance to the current node as inf
        distanceArray[current.x_index][current.y_index] = INT_MAX;
        int i = current.x_index;
        int j = current.y_index;
        // if the neighbours of the current node have not been visited or are obstacles, put them on list of consideration
        if(i > 0 && i <= 9){
            if(maze[i-1][j] != 2 && maze[i-1][j] != 3 && maze[i-1][j] != 5){
                maze[i-1][j] = 4;
                parent[i-1][j] = createLinearIndex(current);
                distanceArray[i-1][j] = min_distance + 1;
                distanceArray_copy[i-1][j] = min_distance + 1;
            }
        }
        if(i >= 0 && i < 9){
            if(maze[i+1][j] != 2 && maze[i+1][j] != 3 && maze[i+1][j] != 5){
                maze[i+1][j] = 4;
                parent[i+1][j] = createLinearIndex(current);
                distanceArray[i+1][j] = min_distance + 1;
                distanceArray_copy[i+1][j] = min_distance + 1;
            }
        }
        if(j > 0 && j <= 9){
            if(maze[i][j-1] != 2 && maze[i][j-1] != 3 && maze[i][j-1] != 5){
                maze[i][j-1] = 4;
                parent[i][j-1] = createLinearIndex(current);
                distanceArray[i][j-1] = min_distance + 1;
                distanceArray_copy[i][j-1] = min_distance + 1;
            }
        }
        if(j >= 0 && j < 9){
            if(maze[i][j+1] != 2 && maze[i][j+1] != 3 && maze[i][j+1] != 5){
                maze[i][j+1] = 4;
                parent[i][j+1] = createLinearIndex(current);
                distanceArray[i][j+1] = min_distance + 1;
                distanceArray_copy[i][j+1] = min_distance + 1;
            }
        }
        numExpanded += 1;
    }
	// Finding the route of the shortest path using the parent matrix
    int i = 0;
    if(distanceArray[dest_coord.x_index][dest_coord.y_index] == INT_MAX){

    }
    else{
        route[i] = createLinearIndex(dest_coord); // Get the starting index
        while(parent[createCoordinates(route[i]).x_index][createCoordinates(route[i]).y_index] != 0){
            i++;
            route[i] = parent[createCoordinates(route[i-1]).x_index][createCoordinates(route[i-1]).y_index];
        }
    }


    // Print the cost table
   	printf("Printing the cost table\n");
    displayDistanceCopyArray();
    
    // Marking the obstacle points
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(maze[i][j] == 2){
                dup_maze[i][j] ='x';
            }
        }
    }

    // Marking the shortest path
    for(int j = 0; j <= i; j++){
        dup_maze[createCoordinates(route[j]).x_index][createCoordinates(route[j]).y_index] = '*';
    }

    // Marking the start and end coordinates in the grid
    dup_maze[start_coord.x_index][start_coord.y_index] = 'S';
    dup_maze[dest_coord.x_index][dest_coord.y_index] = 'E';
    // Printing out the grid with shortest path marked
    printf("Printing out the grid with the shortest path marked\n");
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("|%c",dup_maze[i][j]);
        }
        printf("|\n");
    }
    return 0;
}
