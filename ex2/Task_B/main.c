#include <stdio.h>
#include "array.h"


int main(int argc, char* argv[]){
    long dim = 1000*1000*1000;
    Array arr = array_new(5);

    for(long i = 0; i < dim; i++){
        array_insertBack(&arr, i);
    }
    array_destroy(arr);
    return 0;
}
