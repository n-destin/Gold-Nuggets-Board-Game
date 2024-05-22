/*
 * Header file for the visibility program
 * Samuel Alkire
 * May 20th, 2024
 * Notes from IZZys code
 *  We have an array of spots called grid
 *  Each person has a location, which is an int within that grid
 *  Will take in a player, a map, and use math to draw lines from the person position
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"
#include "person.h"

void visibility(person_t* person, map_t* map);
/*
 * We will loop through all of the points in our grid, drawing lines to the player
 * If they intersect any walls then they won't be set to visible(unless they already are)
 * For gold, they will be only set to visible if they are in line with the player, otherwise are false
 * Since the grid can't be huge, this method shouldn't take too long despite being slightly inneficient
 * Returns true if the object should be set to visible
 */
static bool checkVisibility(int playerPos, int spotPos, spot_t** grid, int rows, int columns);

/* If the change in x is greater, meaning the slope is between -1 and 1, we iterate over the x-values and check proximity with y-values
 * The determinant is used to determine if we are closer to the current y value or the one one above/below
 * If it is negative we are closer to current and check that spot, otherwise we increment to the next spot
 * Overall this can be thought of as us determining which side of the midpoint between spots that the line falls on
 * Returns true if the line doesn't intersect any barriers or blank space
 */
static bool shallowLine(int x1, int y1, int x2, int y2, spot_t** grid, int rows, int columns);

/* If the change in y is greater, meaning the abs(slope) > 1, we instead iterate over the y-values for this algorithm
 * The logic is otherwise the same as the shallowline method but flipped
 */
static bool steepLine(int x1, int y1, int x2, int y2, spot_t** grid, int rows, int columns);

// Checks if a given spot is a barrier and returns true if so
static bool spotBarrier(spot_t* spot);

// Checks if a given spot is gold
static bool checkGold(spot_t* spot);

//Returns the 1D coordinate for 2D coordinates
static int convertXY(int xCoord, int yCoord, int rows, int columns);








