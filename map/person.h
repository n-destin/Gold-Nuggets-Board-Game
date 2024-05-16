/* 
 * person.h - header file for CS50 person module
 *
 * A person is an object held within a map and corresponds to a player in the game nuggets. 
 * A person has an amount of gold, as well as a name and position on a map.
 * 
 * Izzy Axinn, May 2024
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**************** global types ****************/
typedef struct person person_t;

/**************** functions ****************/

/**************** person_new ****************/
/*
* Allocates memory for a spot. Sets name to 'c' and sets gold to 0.
* Caller is responsible for calling person_delete
*/
person_t* person_new(char c);

/**************** person_addGold ****************/
/*
* Adds 1 gold to person's count
*/
void person_addGold(person_t* person);

/**************** person_getGold ****************/
/*
* Returns person's gold count
*/
int person_getGold(person_t* person);

/**************** person_getName ****************/
/*
* Returns person's gold count
*/
char person_getName(person_t* person);

/**************** person_getGold ****************/
/*
* Returns person's name
*/
void person_setPos(person_t* person, int new_pos);

/**************** person_getPos ****************/
/*
* Returns person's position
*/
int person_getPos(person_t* person);

/**************** person_delete ****************/
/*
* Delete person
*/
void person_delete(person_t* person);

/**************** person_setName ****************/
/*
* Sets person's name
*/
void person_setName(person_t* person, char c);



