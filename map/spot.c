#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "person.h"

typedef struct spot{
    char item;
    char person;
} spot_t;


spot_t* spot_new(){
    spot_t* spot = malloc(sizeof(spot_t));
    if (spot == NULL) {
        fprintf(stderr, "Memory allocation error");
        exit(1);          
    } 
    return spot;
}

bool spot_insert(spot_t* spot, char c){
    if(spot == NULL){
        return false;
    }
    spot->item = c;
    return true;
}

char spot_item(spot_t* spot){
    return spot->item;
}

void spot_delete(spot_t* spot){
    free(spot);
}