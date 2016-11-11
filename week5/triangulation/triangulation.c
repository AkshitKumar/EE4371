#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

typedef struct Point{
    int x_coord;
    int y_coord;
}Point;

typedef struct Package{
    double min_cost;
    int index;
}Package;

Point vertices[5] = {{0,0},{1,0},{2,1},{1,2},{0,2}};

Package t[5][5];
int n = 5;

double cost(Point vi,Point vj,Point vk){
    return sqrt(pow(vi.x_coord - vj.x_coord,2) + pow(vi.y_coord - vj.y_coord,2))
         + sqrt(pow(vj.x_coord - vk.x_coord,2) + pow(vj.y_coord - vk.y_coord,2))
         + sqrt(pow(vi.x_coord - vk.x_coord,2) + pow(vi.y_coord - vk.y_coord,2));
}

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

void printIndex(){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            printf("%d ",t[i][j].index);
        }
        printf("\n");
    }
}

void printTriangulation(int i, int j){
    if(j < i+2){
        return;
    }
    int k = t[i][j].index;
    printf("%d %d %d\n",i,k,j);
    printTriangulation(i,k);
    printTriangulation(k,j);
}

int main(){
    int n = sizeof(vertices)/sizeof(vertices[0]);
    printf("%f\n",triangulate(vertices,n));
    printTriangulation(0,n-1);
    printIndex();
    return 0;
}
