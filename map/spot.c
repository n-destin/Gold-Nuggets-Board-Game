#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "person.h"

typedef struct spot{ //Contains item, person, and a visibility
    char item;
    char person; // the person on the slot
    bool visible;
    int gold;
} spot_t;

spot_t* spot_new(){
    spot_t* spot = malloc(sizeof(spot_t));
    if (spot == NULL) { //Checks for memory allocation
        fprintf(stderr, "Memory allocation error");
        exit(1);          
    } 
    spot->gold = 0;
    spot->visible = true;
    return spot;
}

bool spot_insert(spot_t* spot, char c){
    if(spot == NULL){ //Makes sure possible psot
        return false;
    }
    spot->item = c;
    return true;
}

void set_visibility(spot_t* spot, bool visibility){
    spot->visible = visibility;
}

bool get_visibility(spot_t* spot){
    return spot->visible;
}

char spot_item(spot_t* spot){
    return spot->item;
}

void spot_delete(spot_t* spot){
    free(spot);
}

void spot_set_item(spot_t* spot, char item){
    spot->item = item;
}

void spot_add_gold(spot_t* spot, int gold){
    spot->gold = gold;
}

int spot_get_gold(spot_t* spot){
    return spot->gold;
}