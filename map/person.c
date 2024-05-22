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
    char name[MaxNameLength];
    addr_t adress;
} person_t;

person_t* person_new(char c, char* name){
    person_t* person = malloc(sizeof(person_t));
    char port_string[16];
    if (person == NULL) { //Checks for memory allocation
        fprintf(stderr, "Memory allocation error");
        exit(1);          
    } 
    addr_t adress = message_noAddr();
    int port = message_init(stderr);
    sprintf(port_string, "%d", port);
    message_setAddr(name, port_string, &adress);

    if(!message_isAddr(adress)){
        fprintf(stderr, "Unablen to initialize adress");
        exit(1);
    }
    person->letter = c;
    person->adress = adress;
    strcpy(person->name, name);
    person->gold = 0;
    return person;
}

void person_addGold(person_t* person){
    person->gold = person->gold + 1;
}

addr_t person_getAddr(person_t* person){
    return person->adress;
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
    clone->adress = original->adress; // Assuming addr_t can be copied directly

    return clone;
}