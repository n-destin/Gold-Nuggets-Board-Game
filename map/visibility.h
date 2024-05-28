/*
 * Header file for the visibility program
 * Samuel Alkire
 * May 28th, 2024
 * Visibility Module for the Nuggets Project.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"
#include "person.h"

/* Basic Pseudo Code for the entire visibility process:
 *  We loop through all of the points on the grid:
 *      If the point is not a blank space than we check if it is visible by:
 *          Calculate the slope from the point to the player
 *          If the slope is between -1 and 1:
 *              If the player is further right
 *                  Draw a shallow line from the spot to the player
 *                  Return false if it intersects anything, and true otherwise
 *              else
 *                  Draw a shallow line from the player to spot
 *                  Return false if it intersects anything, and true otherwise
 *          If the abs(slope) is greater than 1:
 *              If the player is below the spot
 *                  Draw a steep line from the spot to the player
 *                  Return false if it intersects anything, and true otherwise
 *              else
 *                  Draw a steep line from the player to spot
 *                  Return false if it intersects anything, and true otherwise
 *      If the spot is visible
 *          Make that spot visible
 *      if the spot is gold and also not visible
 *          Make the gold no longer visible
 */

/**************** Visibility ****************/

/* This is the method that will be called to actually use the visibility module
 * When called on a player and map, this method will go through all of the spots and check if they are visible
 * This should be called each time a player is inserted into the map or it moves a space
 */

void visibility(person_t* person, map_t* map);

/******Since all of the methods are static, aside from this visibility one, there descriptions can be found within the visibility.c file*/








