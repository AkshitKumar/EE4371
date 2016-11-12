/*
    Name : Akshit Kumar
    Roll No. : EE14B127
*/
// Including the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

// Defining the structure to hold the vertices 
typedef struct Point{
    int x_coord;
    int y_coord;
}Point;

// Defining the structure to hold the min cost of triangulating a sub polygon and the min index
typedef struct Data{
    double min_cost;
    int index;
}Data;

// Defining the vertices
Point vertices[5] = {{0,0},{1,0},{2,1},{1,2},{0,2}};
// Table to hold the min cost required to triangulate a set of vertices
Data t[5][5];
int n = 5;

// Function to calculate the cost of joining vertice v1,vertice v2 and vertice v3
double cost(Point vi,Point vj,Point vk){
    return sqrt(pow(vi.x_coord - vj.x_coord,2) + pow(vi.y_coord - vj.y_coord,2))
         + sqrt(pow(vj.x_coord - vk.x_coord,2) + pow(vj.y_coord - vk.y_coord,2))
         + sqrt(pow(vi.x_coord - vk.x_coord,2) + pow(vi.y_coord - vk.y_coord,2));
}

// function to calculate the minimum costs for triangulate the vertices 0 to n-1
double triangulate(Point vertices[5],int n){
    if(n < 3)
        return 0;
    for(int gap = 0; gap < n; gap++){
        for(int i = 0, j = gap; j < n; i++,j++){
            if(j < i+2){
                t[i][j].min_cost = 0.0;
            }
            else{
                t[i][j].min_cost = INT_MAX;
                for(int k = i+1; k < j; k++){
                    double val = t[i][k].min_cost + t[k][j].min_cost + cost(vertices[i],vertices[j],vertices[k]);
                    if(t[i][j].min_cost > val){
                        t[i][j].min_cost = val;
                        t[i][j].index = k;
                    }
                }
            }
        }
    }
    return t[0][n-1].min_cost;
}
// To print the mincost table
void printCost(){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            printf("%f ",t[i][j].min_cost);
        }
        printf("\n");
    }
}

// print which node should be connected to which
void printTriangulation(int i, int j){
    if(j < i+2){
        return;
    }
    int k = t[i][j].index;
    printf("Connect the following : (%d,%d) (%d,%d) (%d,%d)\n",vertices[i].x_coord,vertices[i].y_coord,vertices[k].x_coord,vertices[k].y_coord,vertices[j].x_coord,vertices[j].y_coord);
    printTriangulation(i,k);
    printTriangulation(k,j);
}

/*
To plot the points on a graph, a matlab script has been written which is present in the zip file. The result is saved in plot.jpg file
*/

int main(){
    int n = sizeof(vertices)/sizeof(vertices[0]);
    printf("The minimum cost for triangulating the polygon given by the vertices :%f\n",triangulate(vertices,n));
    printf("To get the minimum triangulation, connect the following point :\n");
    printTriangulation(0,n-1);
    printf("Printing the cost table:");
    printCost();
    return 0;
}

