#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct person person_t;

person_t* person_new(char c);

void person_addGold(person_t* person);

int person_getGold(person_t* person);

char person_getName(person_t* person);

void person_setPos(person_t* person, int new_pos);

int person_getPos(person_t* person);
void person_delete(person_t* person);



