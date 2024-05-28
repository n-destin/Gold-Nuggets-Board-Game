#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../support/message.h"


#define MaxNameLength 50   // max number of chars in playerName
#define MaxPlayers 26      // maximum number of players
#define GoldTotal 250      // amount of gold in the game
#define GoldMinNumPiles 10 // minimum number of gold piles
#define GoldMaxNumPiles 30 // maximum number of gold piles


typedef struct person{// Person has a name, position, and a quantity of gold
    char letter;
    int pos;
    int gold;
    int* visible;
    char name[MaxNameLength];
    addr_t address;
} person_t;

person_t* person_new(char c, char* name, addr_t address, int max_position){
    person_t* person = malloc(sizeof(person_t));
    // char port_string[16];
    if (person == NULL) { //Checks for memory allocation
        fprintf(stderr, "Memory allocation error");
        exit(1);          
    }
    person->visible = (int*) malloc(sizeof(int) * max_position);
    for (int i = 0; i < max_position; i++) {
        person->visible[i] = 0;
    }

    person->letter = c;
    person->address = address;
    strcpy(person->name, name);
    person->gold = 0;
    return person;
}

void person_addGold(person_t* person, int gold){
    person->gold = person->gold + gold;
}

addr_t person_getAddr(person_t* person){
    return person->address;
}

int person_getGold(person_t* person){
    return person->gold;
}

void person_setName(person_t* person, char* new_name){
    strcpy(person->name, new_name);
}

void person_setLetter(person_t * person, char new_letter)
{
    person->letter = new_letter;
}

char* person_getName(person_t* person){
    return person->name;
}

char person_getLetter(person_t * person)
{
    return person->letter;
}

void person_setPos(person_t* person, int new_pos){
    person->pos = new_pos;
}

int person_getPos(person_t* person){
    return person->pos;
}

void person_delete(person_t* person){
    printf("got here");
    free(person);
}

// // Cloning a person
person_t* person_clone( person_t* original) {
    person_t* clone = malloc(sizeof(person_t*));
    
    if (clone == NULL) {
        fprintf(stderr, "Memory allocation error");
        exit(1);
    }

    if(original == NULL){
        return NULL;
    }

    clone->letter = original->letter;
    clone->pos = original->pos;
    clone->gold = original->gold;
    strcpy(clone->name, original->name);
    clone->address = original->address; // Assuming addr_t can be copied directly

    return clone;
}


int* get_visible(person_t* person){
    return person->visible;
}

void update_visible_array(person_t* person, int index){
    person->visible[index] = 1;
}