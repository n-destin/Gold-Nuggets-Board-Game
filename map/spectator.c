#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#define MaxNameLength 50   // max number of chars in playerName



typedef struct spectator{ // spectator has a name, position, and a quantity of gold
    char name[MaxNameLength];
    addr_t port;
} spectator_t;

spectator_t* spectator_new(char* name){
    spectator_t* spectator = malloc(sizeof(spectator_t));
    if (spectator == NULL) { //Checks for memory allocation
        fprintf(stderr, "Memory allocation error");
        exit(1);          
    }
    addr_t port = message_noAddr(message_init());
    spectator->name = name;
    spectator->port = port;
    return spectator;
}


void spectator_setName(spectator_t* spectator, char* new_name){
    spectator->name = new_name;
}

char* spectator_getName(spectator_t* spectator){
    return spectator->name;
}

void spectator_delete(spectator_t* spectator){
    free(spectator);
}