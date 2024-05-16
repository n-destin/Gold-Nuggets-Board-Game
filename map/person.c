#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


typedef struct person{ //Person has a name, position, and a quantity of gold
    char name;
    int pos;
    int gold;
} person_t;

person_t* person_new(char c){
    person_t* person = malloc(sizeof(person_t));
    if (person == NULL) { //Checks for memory allocation
        fprintf(stderr, "Memory allocation error");
        exit(1);          
    } 
    person->name = c;
    person->gold = 0;
    return person;
}

void person_addGold(person_t* person){
    person->gold = person->gold + 1;
}

int person_getGold(person_t* person){
    return person->gold;
}

void person_setName(person_t* person, char c){
    person->name = c;
}

char person_getName(person_t* person){
    return person->name;
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