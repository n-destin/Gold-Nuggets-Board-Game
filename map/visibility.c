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
static bool isOnLine(int x, int y, int x1, int y1, int x2, int y2);
static bool sideOfLine(int xA, int yA, int xB, int yB, int x1, int y1, int x2, int y2);

/* This method is called on a person after each move and given the person and map as parameters
 * It will go through the entire map and set every point as visible/invisible as needed
 */
void visibility(person_t* person, map_t* map){
    int position = person_getPos(person);
    spot_t** grid = get_grid(map);
    int rows = get_rows(map);
    int columns = get_columns(map);

    // We are going to loop through all the spots in the grid and check if it is visible by the player
    int totalSpots = rows * columns;
    for(int i=0; i<totalSpots; i++){
        spot_t* spot = grid[i];
        bool isGold = checkGold(spot);
        //Blank spaces don't matter so we skip those spots, are always technically 'visible'
        if( !(spot_item(spot) == ' ') ){ // || ((get_visibility(spot) ) && !isGold) ) {
           int* visible_array = get_visible(person); // the array of visible points
           bool visible = checkVisibility(position, i, grid, rows, columns);
           if(visible || visible_array[i] == 1){
               if(isGold && !visible){
                set_invisible_gold(spot, true);
               }
               else if(isGold){
                set_invisible_gold(spot, false);
               }
               set_visibility(spot, true);
               update_visible_array(person, i);
               
           }
           //if it is gold, but not visibile anymore, then we set it to not be visible
           else if (isGold){
               set_visibility(spot, false);
           }else{
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
    int otherY = y + yIncrementer;
    /* We need to keep track of the points that the line is between
     * Since slope <1 we know it will hit a point before crossing, so we need to track this
     */
    for (int x= x1; x < x2; x++){

        int currIndex = convertXY(x, y, rows, columns);
        int otherIndex = convertXY(x, otherY, rows, columns);
        // Let us double check to make sure that the other index isn't out of bounds
        if(otherIndex < 0 || otherIndex >= rows*columns){
            otherIndex = currIndex;
        }

        //Firstly, we check if the point is on the line. If so, then we only check that point to see if it is a barrier
        if(isOnLine(x, y, x1, y1, x2, y2)){
            otherIndex = currIndex;
        }

        spot_t* currSpot = grid[currIndex];
        spot_t* otherSpot = grid[otherIndex];
        //We check if both the currSpot and otherSpot are barriers, with the exception of it still being on the initial starting axes
        if( spotBarrier(currSpot) && spotBarrier(otherSpot) && !(y == y1 && x== x1) ){
            //suppose we start at a barrier, we might still be stuck on the wall due to midpoint method
            //This accounts for this issue
            return false;
        }

        /* Now this is where it starts to get quite complicated
         * Since we are rounding to the closest point, we never actually know when we truly cross a y-value since it is estimated
         * However, we need to know when we cross a y-value, as if both x-points between at that value are barriers the line shouldn't work
         * Thus we check if two adjacent x-points are on different sides of the line, then we know we are crossing a y-value
         */

        if( ((x+1) < columns) &&  sideOfLine(x, y, x+1 , y, x1, y1, x2, y2)){
            //Thus we are crossing a y-threshold right now
            spot_t* otherX = grid[currIndex + 1];
            if(spotBarrier(otherX) && spotBarrier(currSpot)){
                return false;
            }
        }
        //Also check for the point to the left

        if( ((x-1) >= 0) &&  sideOfLine(x, y, x-1 , y, x1, y1, x2, y2)){
            //Thus we are crossing a y-threshold right now
            spot_t* otherX = grid[currIndex - 1];
            if(spotBarrier(otherX) && spotBarrier(currSpot)){
                return false;
            }
        }

        // Every time the determinant flips from positive to negative, it means that we have flipped points
        // If we go to the next point, then we need to keep the other point as the old one
        if (determinant > 0){

            otherY = y;
            y = y + yIncrementer;
    

            // If we do increment up, we need to subtract to account for the new movement up
            determinant = determinant + 2 * (yDir-xDir);
        }
        else{
            determinant = determinant + 2 * yDir;
        }
        //Finally, we need to check if we actually hit a point. We then flip the otherY
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
    //Case where on the same veritcal line
    if(x1 == x2){
        for(int i= y1 + 1; i < y2; i++){
            int lineIndex = convertXY(x1, i, rows, columns);
            spot_t* sp = grid[lineIndex];
            if(spotBarrier(sp)){
                return false;
            }
        }
        return true;
    }
    if (xDir < 0){
        xIncrementer = -1;
        xDir = -1 * xDir;
    }
    int determinant = (2 * xDir) - yDir;
    int x = x1;
    int otherX = x + xIncrementer;
    for (int y = y1; y < y2  ; y++ ){
        //Check the point to see if its valid, if not return false
        int currIndex = convertXY(x, y, rows, columns);

        int otherIndex = convertXY(otherX, y, rows, columns);
        //If other is out of bounds, we just check the current twice cause why not
        if(otherIndex < 0 || otherIndex >= rows*columns){
            otherIndex = currIndex;
        }

        //Check if point is on line, if so then that means we only care about current point

        if(isOnLine(x, y, x1, y1, x2, y2)){
            otherIndex = currIndex;
        }
        spot_t* currSpot = grid[currIndex];
        spot_t* otherSpot = grid[otherIndex];


        //We check if the spot and other spot is a barrier, with the exception of it still being the starting point
        if( spotBarrier(currSpot) && spotBarrier(otherSpot) &&  !(y == y1 && x== x1)){
            return false;
        }

         /* Now this is where it starts to get quite complicated
         * Since we are rounding to the closest point, we never actually know when we truly cross a x-value since it is estimated
         * However, we need to know when we cross a x-value, as if both y-points between at that value are barriers the line shouldn't work
         * Thus we check if two adjacent 7-points are on different sides of the line, then we know we are crossing a x-value
         */

        if( ((y+1) < rows) &&  sideOfLine(x, y+1, x , y, x1, y1, x2, y2)){
            //Thus we are crossing a x-threshold right now
            int tempY = convertXY(x, y+1,rows, columns);
            spot_t* otherY = grid[tempY];
            if(spotBarrier(otherY) && spotBarrier(currSpot)){
                return false;
            }
        }
        //Also check for the point below
        if( ((y-1) > 0)  && sideOfLine(x, y-1, x , y, x1, y1, x2, y2)){
            //Thus we are crossing a x-threshold right now
            int tempY = convertXY(x, y-1, rows, columns);
            spot_t* otherY = grid[tempY];
            if(spotBarrier(otherY) && spotBarrier(currSpot)){
                return false;
            }
        }


        // This means we are closer to one y-value up and increment the y for the next spot
        if (determinant > 0){
            otherX = x;
            x = x + xIncrementer;
            // If we do increment up, we need to subtract to account for the new movement up
            determinant = determinant + 2 * (xDir-yDir);
        }
        else{
            determinant = determinant + 2 * xDir;
        }
    }
    // if we made it without hitting a barrier then we can make it visible
    return true;
}
// Checks if a given spot is a barrier and returns true if so
static bool spotBarrier(spot_t* spot){
    if( (spot_item(spot) == '-') || (spot_item(spot) == '|') || (spot_item(spot) == '+') || (spot_item(spot) == ' ') || (spot_item(spot) == '#') ){
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
//Checks if the point is on the given line
static bool isOnLine(int x, int y, int x1, int y1, int x2, int y2){
    double dx= x2-x1;
    double dy= y2-y1;
    double leftSide = y-y1;
    double rightSide = (dy/dx)*(x-x1);
    if(leftSide == rightSide){
        return true;
    }
    return false;
}

/* Checks if two points are on either side of a line
 * This is used to know when we actually cross an x/y value(not the estimate) to check for barriers
 * More description of use can be found in the shallow/steep line methods
 */
static bool sideOfLine(int xA, int yA, int xB, int yB, int x1, int y1, int x2, int y2){
    double dx= x2-x1;
    double dy= y2-y1;
    double point1Val = yA -y1 - ( (dy/dx)*(xA-x1) );
    double point2Val = yB -y1 - ( (dy/dx)*(xB-x1) );
    if( point1Val > 0 && point2Val < 0){
        return true;
    }
    if( point1Val < 0 && point2Val > 0){
        return true;
    }
    //If either one of these points is zero, it means the line goes through it and we will eventually check it
    else{
        return false;
    }
}

/* Unit testing for the visibility module. 
 * I commented it out but it demonstrates how this can be used
 * Since we don't have movement yet, I just added a bunch of people to the large map and checked their visibility
 */
/* int main(int argc, char *argv[]){
    map_t* map = map_new("/thayerfs/home/f006hjn/cs50-dev/nuggets-Mr-Axinn/maps/big.txt");
    person_t* person1 = insert_person(map, '@');
    visibility(person1, map);
    map_print(map, stdout);
    //move_person(map, person1, 'l');
    person_t* person2 = insert_person(map, '?');
    visibility(person2, map);
    person_t* person3 = insert_person(map, '^');
    visibility(person3, map);
    person_t* person4 = insert_person(map, 'w');
    visibility(person4, map);
    person_t* person5 = insert_person(map, 'p');
    visibility(person5, map);
    person_t* person6 = insert_person(map, 'q');
    visibility(person6, map);
    map_print(map, stdout);

    map_delete(map);

} 
*/