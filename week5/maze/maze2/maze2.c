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

typedef struct Coordinates{
    int x_index;
    int y_index;
}Coordinates;

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

 int maze_copy[10][10] = {
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

/*
 int dup_maze[10][10] = {
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
*/
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

Coordinates start_coord = {0,0};
Coordinates dest_coord = {3,9};
/*
start_coord.x_index = 0;
start_coord.y_index = 0;
dest_coord.x_index = 3;
dest_coord.y_index = 9;
*/
int createLinearIndex(Coordinates coord){
    return (coord.x_index * 10) + coord.y_index;
}

Coordinates createCoordinates(int linearIndex){
    Coordinates coord;
    coord.y_index = linearIndex % 10;
    coord.x_index = linearIndex / 10;
    return coord;
}

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

typedef struct Info{
    Coordinates coord;
    int min_distance;
}Info;

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

void displayDistanceArray(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(distanceArray[i][j] == INT_MAX){
                printf(" * ");
            }
            else{
                printf("%d ",distanceArray[i][j]);
            }
            //printf("%d ",distanceArray[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------\n");
}

void displayDistanceCopyArray(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(distanceArray_copy[i][j] == INT_MAX){
                printf(" * ");
            }
            else{
                printf("%d ",distanceArray_copy[i][j]);
            }
            //printf("%d ",distanceArray[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------\n");
}

int route[100];

int main(){
    bool found = false;
    distanceArray[start_coord.x_index][start_coord.y_index] = 0;
    int min_distance;
    Coordinates current;
    while(!found){
        min_distance = minimumDistanceFromStart(distanceArray).min_distance;
        printf("%d\n",min_distance );
        current = minimumDistanceFromStart(distanceArray).coord;
        if((current.x_index == dest_coord.x_index && current.y_index == dest_coord.y_index) || min_distance == INT_MAX){
            found = true;
            break;
        }
        maze[current.x_index][current.y_index] = 3;
        distanceArray[current.x_index][current.y_index] = INT_MAX;
        int i = current.x_index;
        int j = current.y_index;
        if(i > 0 && i <= 9){
            if(maze[i-1][j] == 2 && maze[i-1][j] != 3 && maze[i-1][j] != 5){
                maze[i-1][j] = 4;
                parent[i-1][j] = createLinearIndex(current);
                distanceArray[i-1][j] = min_distance + 10;
                distanceArray_copy[i-1][j] = min_distance + 10;
            }
            else if(maze[i-1][j] != 2 && maze[i-1][j] != 3 && maze[i-1][j] != 5){
                maze[i-1][j] = 4;
                parent[i-1][j] = createLinearIndex(current);
                distanceArray[i-1][j] = min_distance + 1;
                distanceArray_copy[i-1][j] = min_distance + 1;
            }
        }
        if(i >= 0 && i < 9){
            if(maze[i+1][j] == 2 && maze[i+1][j] != 3 && maze[i+1][j] != 5){
                maze[i+1][j] = 7;
                parent[i+1][j] = createLinearIndex(current);
                distanceArray[i+1][j] = min_distance + 10;
                distanceArray_copy[i+1][j] = min_distance + 10;
            }
            else if(maze[i+1][j] != 2 && maze[i+1][j] != 3 && maze[i+1][j] != 5 && maze[i+1][j] != 7){
                maze[i+1][j] = 4;
                parent[i+1][j] = createLinearIndex(current);
                distanceArray[i+1][j] = min_distance + 1;
                distanceArray_copy[i+1][j] = min_distance + 1;
            }
        }
        if(j > 0 && j <= 9){
            if(maze[i][j-1] == 2 && maze[i][j-1] != 3 && maze[i][j-1] != 5){
                maze[i][j-1] = 7;
                parent[i][j-1] = createLinearIndex(current);
                distanceArray[i][j-1] = min_distance + 10;
                distanceArray_copy[i][j-1] = min_distance + 10;
            }
            else if(maze[i][j-1] != 2 && maze[i][j-1] != 3 && maze[i][j-1] != 5 && maze[i][j-1] != 7){
                maze[i][j-1] = 4;
                parent[i][j-1] = createLinearIndex(current);
                distanceArray[i][j-1] = min_distance + 1;
                distanceArray_copy[i][j-1] = min_distance + 1;
            }
        }
        if(j >= 0 && j < 9){
            if(maze[i][j+1] == 2 && maze[i][j+1] != 3 && maze[i][j+1] != 5){
                maze[i][j+1] = 7;
                parent[i][j+1] = createLinearIndex(current);
                distanceArray[i][j+1] = min_distance + 10;
                distanceArray_copy[i][j+1] = min_distance + 10;
            }
            else if(maze[i][j+1] != 2 && maze[i][j+1] != 3 && maze[i][j+1] != 5  && maze[i][j+1] != 7){
                maze[i][j+1] = 4;
                parent[i][j+1] = createLinearIndex(current);
                distanceArray[i][j+1] = min_distance + 1;
                distanceArray_copy[i][j+1] = min_distance + 1;
            }
        }
        numExpanded += 1;
        //displayDistanceCopyArray();
    }
    displayDistanceCopyArray();
    printf("%d\n",numExpanded);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%d ",parent[i][j]);
        }
        printf("\n");
    }

    int i = 0;
    if(distanceArray[dest_coord.x_index][dest_coord.y_index] == INT_MAX){

    }
    else{
        route[i] = createLinearIndex(dest_coord);
        while(parent[createCoordinates(route[i]).x_index][createCoordinates(route[i]).y_index] != 0){
            i++;
            route[i] = parent[createCoordinates(route[i-1]).x_index][createCoordinates(route[i-1]).y_index];
        }
    }
    //printf("%d\n",i);

    for(int j = 0; j <= i; j++){
        printf("%d ",route[j] );
    }
    printf("\n");
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(maze_copy[i][j] == 2){
                dup_maze[i][j] ='x';
                //printf("%s\n", );
            }
            //printf("%d ",maze[i][j]);
        }
        //printf("\n");
    }

    dup_maze[start_coord.x_index][start_coord.y_index] = 'S';
    dup_maze[dest_coord.x_index][dest_coord.y_index] = 'E';

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("|%c",dup_maze[i][j]);
        }
        printf("|\n");
    }

    for(int j = 0; j <= i; j++){
        dup_maze[createCoordinates(route[j]).x_index][createCoordinates(route[j]).y_index] = '*';
    }

    dup_maze[start_coord.x_index][start_coord.y_index] = 'S';
    dup_maze[dest_coord.x_index][dest_coord.y_index] = 'E';

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("|%c",dup_maze[i][j]);
        }
        printf("|\n");
    }
    return 0;
}
