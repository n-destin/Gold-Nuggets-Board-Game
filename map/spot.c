#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../tse-Mr-Axinn/libcs50/hashtable.h"
#include "spot.h"

typedef struct spot{
    char item;
} spot_t;

hashtable_t* available_characters; 

spot_t* spot_new(hashtable_t* possible_characters){
    available_characters = possible_characters;
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