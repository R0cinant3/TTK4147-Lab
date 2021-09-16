#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]){
    long xy_size = 1000*1000*1000; // 8 GB (sizeof(long) = 8 bytes)
    long x_dim = 10000;
    long y_dim = xy_size/x_dim;
    long** matrix = malloc(y_dim*sizeof(long*));
    printf("Size of int: %ld", sizeof(int));
    for(long y = 0; y < y_dim; y++){
        matrix[y] = malloc(x_dim*sizeof(long));
        memset(matrix[y], 0, x_dim*sizeof(long));
    }
    printf("Allocation complete (press any key to continue...)\n");
    getchar();
    return 0;
}
