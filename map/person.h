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
typedef struct sockaddr_in addr_t;

/**************** functions ****************/

/**************** person_new ****************/
/*
* Allocates memory for a person. Sets name to 'c' and sets gold to 0.
* Caller is responsible for calling person_delete
*/
person_t* person_new(char c, char* name, addr_t address, int max_positions);

/**************** person_addGold ****************/
/*
* Adds specified amount of gold to person's count
*/
void person_addGold(person_t* person, int gold);

/**************** person_getGold ****************/
/*
* Returns person's gold count
*/
int person_getGold(person_t* person);

char person_getLetter(person_t* person);

/**************** person_getName ****************/
/*
* Returns person's name
*/
char* person_getName(person_t* person);

/**************** person_setPos ****************/
/*
* Set a persons position
*/
void person_setPos(person_t* person, int new_pos);

/**************** person_getPos ****************/
/*
* Returns person's position
*/
int person_getPos(person_t* person);

addr_t person_getAddr(person_t* person);

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


person_t* person_clone( person_t* original);

/**************** get_visible ****************/
/*
 * Returns the array of what is visible/not visible for a given person
 * Within the array, zero means it is visible and otherwise it should be visible to that player
 */
int* get_visible(person_t* person);

/**************** update_visible_array ****************/
 /* Updates what is visible and not visible to the player
 */
void update_visible_array(person_t* person, int index);