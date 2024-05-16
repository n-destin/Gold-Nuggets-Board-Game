#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct person{
    char name;
    int pos;
    int gold;
} person_t;

person_t* person_new(char c){
    person_t* person = malloc(sizeof(person_t));
    person->name = c;
    return person;
}

void person_addGold(person_t* person){
    person->gold = person->gold + 1;
}

int person_getGold(person_t* person){
    return person->gold;
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