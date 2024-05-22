#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "spot.h"
#include "map.h"
#include "person.h"

void visibility(person_t* person, map_t* map);
static bool checkVisibility(int playerPos, int spotPos, spot_t** grid, int rows, int columns);
static bool shallowLine(int x1, int y1, int x2, int y2, spot_t** grid, int rows, int columns);
static bool steepLine(int x1, int y1, int x2, int y2, spot_t** grid, int rows, int columns);
static bool spotBarrier(spot_t* spot);
static bool checkGold(spot_t* spot);
static int convertXY(int xCoord, int yCoord, int rows, int columns);


/* Unit testing for the visibility module
 */
int main(int argc, char *argv[]){
    map_t* map = map_new("/thayerfs/home/f006hjn/cs50-dev/nuggets-Mr-Axinn/maps/small.txt");
    map_print(map, stdout);
    person_t* person1 = insert_person(map, '@');
    printf("-------------\n");
    visibility(person1, map);
    map_print(map, stdout);
    printf("-------------\n");

}
/* This method is called on a person after each move and given the person and map as parameters
 * It will go through the entire map and set every point as visible/invisible as needed 
 */
void visibility(person_t* person, map_t* map){
    int position = person_getPos(person);
    spot_t** grid = map_getGrid(map);
    int rows = map_getRows(map);
    int columns = map_getColumns(map);

    // We are going to loop through all the spots in the grid and check if it is visible by the player
    int totalSpots = rows * columns;
    for(int i=0; i<totalSpots; i++){
        spot_t* spot = grid[i];
        bool isGold = checkGold(spot);
        //Blank spaces don't matter so we skip those spots, are always technically 'visible'
        // COULD POTENTIALLY improve efficiency by checking if spot is already visible
        if( !(spot_item(spot) == ' ') ){ // || ((get_visibility(spot) ) && !isGold) ) {
            bool visible = checkVisibility(position, i, grid, rows, columns);
            if(visible){
                set_visibility(spot, true);
            }
            //if it is gold, but not visibile anymore, then we set it to not be visible
            else if (isGold){
                set_visibility(spot, false);
            }
        }
    }
}

/*
 * We will loop through all of the points in our grid, drawing lines to the player
 * If they intersect any walls then they won't be set to visible(unless they already are)
 * For gold, they will be only set to visible if they are in line with the player, otherwise are false
 * Since the grid can't be huge, this method shouldn't take too long despite being slightly inneficient
 * Returns true if the object should be set to visible
 */
static bool checkVisibility(int playerPos, int spotPos, spot_t** grid, int rows, int columns){
    //Get X and Y coordinates for the player and spot
    int playerY = (int) (playerPos / columns);
    int playerX = playerPos - (playerY * columns);
    int spotY = (int) (spotPos / columns);
    int spotX = spotPos - (spotY * columns);
    //Now we figure out the slope between the points
    int xDir = playerX - spotX;
    int yDir = playerY - spotY;
    // in order to perform this algorithm, we need to have different method if the abs(slope) > 1
    if(abs(xDir)>abs(yDir)){
        // Additionally, we need the line to go in the positive x-direction
        if(playerX > spotX){
            // shallowLine from spot to player
            return shallowLine(spotX, spotY, playerX, playerY, grid, rows, columns);
        }
        else{
            // shallow line from player to spot
            return shallowLine(playerX, playerY, spotX, spotY, grid, rows, columns);
        }
    }
    else{
        // Line must go in the positive y-direction
        if(playerY > spotY){
            // steep line from spot to player
            return steepLine(spotX, spotY, playerX, playerY, grid, rows, columns);
        }
        else{
            // steep line from player to spot
            return steepLine(playerX, playerY, spotX, spotY, grid, rows, columns);
        }
    } 
}

/* If the change in x is greater, meaning the slope is between -1 and 1, we iterate over the x-values and check proximity with y-values
 * The determinant is used to determine if we are closer to the current y value or the one one above/below
 * If it is negative we are closer to current and check that spot, otherwise we increment to the next spot
 * Overall this can be thought of as us determining which side of the midpoint between spots that the line falls on
 * Returns true if the line doesn't intersect any barriers or blank space
 */
static bool shallowLine(int x1, int y1, int x2, int y2, spot_t** grid, int rows, int columns){
    int xDir = x2 - x1; // will always be positive 
    int yDir = y2 - y1;
    int yIncrementer = 1;
    if (yDir < 0){
        yIncrementer = -1;
        yDir = -1 * yDir;
    }
    int determinant = 2 * yDir - xDir;
    int y = y1;
    for (int x= x1; x < x2; x++){
        //Check the point to see if its valid, if not return false
        int currIndex = convertXY(x, y, rows, columns);
        spot_t* currSpot = grid[currIndex];
        //We check if the spot is a barrier, with the exception of it still being on the initial starting axes
        if( spotBarrier(currSpot) && !(y == y1 && x== x1) ){
            //suppose we start at a barrier, we might still be stuck on the wall due to midpoint method
            //This accounts for this issue
            return false;
        }
        // This means we are closer to one y-value up and increment the y for the next spot
        if (determinant > 0){
            y = y + yIncrementer;
            // If we do increment up, we need to subtract to account for the new movement up
            determinant = determinant + 2 * (yDir-xDir);
        }
        determinant = determinant + 2 * yDir;
    }
    // if we made it without hitting a barrier then we can make it visible
    return true;
}

/* If the change in y is greater, meaning the abs(slope) > 1, we instead iterate over the y-values for this algorithm
 * The logic is otherwise the same as the shallowline method but flipped
 */
static bool steepLine(int x1, int y1, int x2, int y2, spot_t** grid, int rows, int columns){
    int xDir = x2 - x1; 
    int yDir = y2 - y1;  // Will always be positive by ordering of points
    int xIncrementer = 1;
    if (xDir < 0){
        xIncrementer = -1;
        xDir = -1 * xDir;
    }
    int determinant = (2 * xDir) - yDir;
    int x = x1;
    for (int y = y1; y < y2  ; y++ ){
        //Check the point to see if its valid, if not return false
        int currIndex = convertXY(x, y, rows, columns);
        spot_t* currSpot = grid[currIndex];
        //We check if the spot is a barrier, with the exception of it still being the starting point
        if( spotBarrier(currSpot) && !(y == y1 && x== x1)){
            return false;
        }
        // This means we are closer to one y-value up and increment the y for the next spot
        if (determinant > 0){
            x = x + xIncrementer;
            // If we do increment up, we need to subtract to account for the new movement up
            determinant = determinant + 2 * (xDir-yDir);
        }
        determinant = determinant + 2 * xDir;
    }
    // if we made it without hitting a barrier then we can make it visible
    return true;
}
// Checks if a given spot is a barrier and returns true if so
static bool spotBarrier(spot_t* spot){
    if( (spot_item(spot) == '-') || (spot_item(spot) == '|') || (spot_item(spot) == '+') || (spot_item(spot) == ' ') ){
        return true;
    }
    return false;
}
// Checks if a given spot is gold
static bool checkGold(spot_t* spot){
    if ( spot_item(spot) == '*' ){
        return true;
    }
    return false;
}

//Returns the 1D coordinate for 2D coordinates
static int convertXY(int xCoord, int yCoord, int rows, int columns){
    int location = yCoord*columns + xCoord;
    return location;
}
