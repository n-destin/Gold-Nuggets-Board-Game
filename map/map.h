/* 
 * map.h - header file for CS50 map module
 *
 * A map is a holds the information for the nuggest game
 * including where objects are, where people are, and the various properties
 * of these tings
 * 
 * Izzy Axinn, May 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "person.h"
#include "spot.h"

/**************** global types ****************/
typedef struct map map_t;
typedef struct sockaddr_in addr_t;


/**************** functions ****************/

/**************** map_new ****************/
/*
* Allocates memory for a map and loads it into map data structure
* 
* Caller is responsible for map delete
*/
map_t* map_new(char* path);

map_t* clone_map(map_t* current_map);

/**************** map_validate ****************/
/*
* A helper function for map_new, finding dimensions of map
*/
void map_validate(map_t* map, FILE* in);


/**************** map_load ****************/
/*
* A helper function for map_new, loading data into map
*/
void map_load(map_t* map, FILE* in);


/**************** map_print ****************/
/*
* Prints the map in its current state 
*/
void map_print(map_t* map, FILE* out);


/**************** map_delete ****************/
/*
* Deletes map 
*/
void map_delete(map_t* map);


/**************** namedelete ****************/
/*
* Helper function for map_delete
*/
void namedelete(void* item);

person_t** get_players(map_t* map);

/**************** insert_person ****************/
/*
* Inserts a newly created person with name 'c' into map at a random valid location
*/
person_t* insert_person(map_t* map, char c, char* name, addr_t address);

/**************** move_person ****************/
/*
* Moves a person to a valid square and returns true. If square is invalid returns false.
* The direction char may be 'w', 'a', 's', or 'd' with corresponding directions. Correctly implements player swapping.
* If person steps on gold, will update gold count. Map is updated automatically
*/
bool move_person(map_t* map, person_t* person, char direction);


/**************** gold_initialize ****************/
// Initializes the gold onto the map
void gold_initialize(map_t* map);

/**************** get_rows ****************/
// Returns the number of rows on the map
int get_rows(map_t * map);

/**************** get_columns****************/
// Return the number of columns on the map
int get_columns(map_t* map);

/**************** grid_to_string_player ****************/
/* Converts the grid into a string for what the player sees
 * Uses visible to keep track of what should/should not be displayed
 *
 */
char* grid_to_string_player(map_t* map, char letter);

/**************** grid_to_string_spectator ****************/
/* Converts the grid into a string for what a spectator should see
 * This is the entire map, since a spectator should be able to see everything
 */
char* grid_to_string_spectator(map_t* map);

/**************** set_person ****************/
// Creates a clone of the person which is stored in our array of people
void set_person(map_t* map, person_t* person);

/**************** get_grid****************/
// Returns the grid associated with a given map
spot_t** get_grid(map_t* map);