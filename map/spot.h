/* 
 * spot.h - header file for CS50 spot module
 *
 * A spot is a place a place on a map which can hold a person and object. 
 * A spot may be visible or not visible, though the person on it is always visible
 * 
 * Izzy Axinn, May 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**************** global types ****************/
typedef struct spot spot_t;

/**************** functions ****************/

/**************** spot_new ****************/
/*
* Allocates memory for a spot
* Caller is responsible for calling spot_delete
*/
spot_t* spot_new();

/**************** spot_insert ****************/
/*
* Sets item of spot to char 'c'
*/
bool spot_insert(spot_t* spot, char c);

/**************** spot_item ****************/
/*
* Returns the item held by the spot
*/
char spot_item(spot_t* spot);

/**************** spot_delete ****************/
/*
* Deletes the spot
*/
void spot_delete(spot_t* spot);

/**************** spot_delete ****************/
/*
* get visibility of spot
*/
bool get_visibility(spot_t* spot);

/**************** spot_delete ****************/
/*
* set visibility of spot
*/
void set_visibility(spot_t* spot, bool visibility);

int spot_get_gold(spot_t * spot);

void spot_add_gold(spot_t* spot, int gold);

void spot_set_item(spot_t* spot, char item);

// cloning a spot
spot_t* spot_clone(spot_t* current_spot, int index, int position) ;