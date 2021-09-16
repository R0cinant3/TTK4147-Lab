#include <stdio.h>
#include "array.h"


int main(int argc, char* argv[]){
    long dim = 1000;
    Array arr = array_new(2);

    for(long i = 0; i < dim; i++){
        array_insertBack(&arr, i);
    }
    printf("Pointer Start:\t%p\nPointer End:\t%p\n", (void *)arr.data, (void *)arr.data[arr.back-1]);
    array_destroy(arr);
    return 0;
}
