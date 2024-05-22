#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "spot.h"
#include "map.h"
#include "person.h"
#include <unistd.h> 

// spot.h

#define GoldTotal 250      // amount of gold in the game
#define GoldMinNumPiles 10 // minimum number of gold piles
#define GoldMaxNumPiles 30 // maximum number of gold piles


typedef struct map{ //Contains the rows and columns of the grid, and a grid of objects and a grid of players
    int rows;
    int columns;
    spot_t** grid;
    person_t** players; 
} map_t;

map_t* map_new(char* path){
    map_t* map = calloc(1, sizeof(map_t)); //Allocates memory for map
    if(map == NULL){
        fprintf(stderr, "Memory allocation failure");
        exit(1);
    }
    FILE* in = NULL;  
    in = fopen(path, "r"); //Opens file
    if (in == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        exit(1);
    }
    fprintf(stderr, "Found file %s\n", path);
    map_validate(map, in); //Find rows and columns of file
    fclose(in); //Close in
    in = fopen(path, "r"); //Reopen 'in'
    map_load(map, in); //Load 'in' map
    fclose(in); //Close 'in'
    return map;
}

void map_validate(map_t* map, FILE* in){
    char* line = NULL;
    int numRows = 0; 
    int numColumns = 0;
    while((line = file_readLine(in)) != NULL){ //Read through each line of file
        int line_size = (int)strlen(line); //Find size of line
        if(line_size > numColumns){ //Determining maximum line size (equals number of columns)
            numColumns = line_size;
        }
        numRows++; 
        free(line);
    }
    map->grid = calloc((numColumns*numRows), sizeof(spot_t*)); //Create memory for grid
    map->players = calloc((numColumns*numRows), sizeof(person_t*)); //Create memory for players
    for (int i = 0; i < (map->rows*map->columns); i++) { //Going through array and setting value to NULL
        map->players[i] = NULL; 
        map->grid[i] = NULL;
    }
    if(map->players == NULL || map->grid == NULL){
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }
    map->columns = numColumns;
    map->rows = numRows;
    free(line);
}

void map_load(map_t* map, FILE* in){ //Load in map data
    char c;
    char* line = NULL;
    int current_row = 0; 
    while((line = file_readLine(in)) != NULL){  //Go through entire file
        int i = 0;
        while ((c = line[i]) != '\0') { //Parse character out of line
            spot_t* spot = spot_new(); //Create new spot
            c = line[i]; //Current character in line
            if(!spot_insert(spot, c)){ //Test spot insert
                fprintf(stderr, "Error inserting item %c", c);
                exit(1);
            }
            map->grid[i+(current_row*map->columns)] = spot; //Setting current position on grid to spot
            i++;
        }
        current_row++;
        free(line);
    }
    free(line);
}

void map_print(map_t* map, FILE* out){ 
    for(int row = 0; row < map->rows; row++){ //Go through rows
        for(int column = 0; column < map->columns; column++){ //Go through columns
            if(map->grid[column + (row*map->columns)] == NULL){  //If current position is null do new line
                fprintf(out, "%c", '\n');
                break;
            }
            else if(map->players[column + (row*map->columns)] != NULL){ //Found a player
                person_t* person = map->players[column + (row*map->columns)]; //Get player there
                fprintf(out, "%c", person_getLetter(person)); //Print player (all players are visible but could be changed)
            }else{
                spot_t* spot = map->grid[column + (row*map->columns)]; //Get the spot
                if(get_visibility(spot)){ //Check spot visibility
                    // modify this to account for visible gold.
                    fprintf(out, "%c", spot_item(spot));
                }
                else{ //Print space
                    fprintf(out, "%c", ' ');
                }
            }
        }
        fprintf(out, "%c", '\n');
    }
}

void map_delete(map_t* map){
    for(int i = 0; i < (map->columns*map->rows); i++){ //Go through entire grid
        if(map->grid[i] != NULL){
            spot_delete(map->grid[i]); //Delete spot
        }
        if(map->players[i] != NULL){
            person_delete(map->players[i]); //Delete player
        }
    }
    free(map->grid);
    free(map->players);
    free(map);
}

bool move_person(map_t* map, person_t* person, char direction){
    int current_pos = person_getPos(person); //Current position
    int row = (int) (current_pos / map->columns); //Find current row
    int column = current_pos - (row*map->columns); //Find current column
    int new_pos = 0;
    if(direction == 'w'){ //Up
        if(row == 0){
            return false;
        }
        new_pos = current_pos - map->columns; 
    }
    else if(direction == 'a'){ //Left
        if(column == 0){
            return false;
        }
        new_pos = current_pos - 1;
    }
    else if(direction == 's'){ //Down
        if(row == map->rows){ 
            return false;
        }
        new_pos = current_pos + map->columns;
    }
    else if(direction == 'd'){ //Right
        if(column == map->columns){ 
            return false;
        }
        new_pos = current_pos + 1;
    }
    if(map->players[new_pos] != NULL){ //If there is a player in the way
        char temp_name = person_getLetter(person); //Create temp
        person_setName(person, person_getLetter(map->players[new_pos])); //Swap positions
        person_setName(map->players[new_pos], temp_name); //Swap positions
    }
    else if(spot_item(map->grid[new_pos]) == '.' || spot_item(map->grid[new_pos]) == '#' || spot_item(map->grid[new_pos]) == '*'){ //Valid movment square
        person_setPos(person, new_pos);  //Move person
        map->players[current_pos] = NULL; 
        map->players[new_pos] = person;
        if(spot_item(map->grid[new_pos]) == '*'){ //Found gold
            person_addGold(person); //Add gold to player count
            spot_insert(map->grid[new_pos], '.'); //Inserts '.' in its place 
        }
    }
    else{
        return false;
    }
    return true;
}

set_t* get_freeSpace(map_t * map, int* num_spaces){
    set_t* toReturn = set_new();
    for(int i = 0; i < (map->columns*map->rows); i++){ //Go through all grid spots
        if((spot_item(map->grid[i]) == '.') && map->players[i] == NULL && (spot_get_gold(map->grid[i])== 0)){ //No players here and '.' present and no gold
            char key[20];
            sprintf(key, "%d", *num_spaces);
            int* value = malloc(sizeof(int));
            if (value != NULL) { 
                *value = i;
                set_insert(toReturn, key, value); //Adds possible key to the set with current index
            } else {
                fprintf(stderr, "Memory failure");
            }
            (*num_spaces)++;
        }
    }

    return toReturn;
}

person_t* insert_person(map_t* map, char c, char* name){ //The idea is to insert all possible position into a set and extract one randomly
    set_t* indexes;
    int num_spaces = 0;
    int final_index = 0;
    indexes = get_freeSpace(map, &num_spaces);
    printf("Here: %d", num_spaces);
    if(num_spaces == 0){
        return NULL;
    }
    int random_number = rand() % (num_spaces); //Generate random number from 0 to num_spaces (possible values in set)
    char random_string[20];
    sprintf(random_string, "%d", random_number); //Turn random number to string
    int* temp = set_find(indexes, random_string); //Finds that string in the set
    final_index = *temp; //Sets the index associated with that value
    person_t* person = person_new(c, name); //Create a new person with name 'c' and name 'name'
    person_setPos(person, final_index); //Set position in person struct
    map->players[final_index] = person; //Set position in map
    set_delete(indexes, namedelete); //Deletes set 
    return person;
}

void namedelete(void* item) //Deletes a name as helper function for hashtable
{
  if (item != NULL) {
    free(item);   
  }
}

void gold_initialize(map_t* map)
{
    int space_count = 0;
    set_t* indices = get_freeSpace(map, &space_count);
    if(space_count == 0){
        fprintf(stderr, "No more spaces for initializing gold");
        exit(1);
    }
    int random_piles = rand() % (GoldMaxNumPiles + 1 - GoldMinNumPiles) + GoldMinNumPiles;
    int gold_remaining = GoldTotal - random_piles; // each pile must have at least one gold
    for(int pile = 0; pile<random_piles; pile++){
        int random_index = rand() % space_count;
        int random_gold = rand() % gold_remaining;
        gold_remaining -= random_gold;
        char random_number_string[20];
        sprintf(random_number_string, "%d", random_index);
        int* position = set_find(indices, random_number_string);
        spot_t * spot = map->grid[*position];
        spot_add_gold(spot, random_gold);
        spot_set_item(spot, '*');
    }
}


// clone the current map. To be called whenever inserting a new player
map_t* clone_map(map_t* current_map)
{
    if (current_map == NULL) {
        return NULL;
    }
    
    map_t* new_map = malloc(sizeof(map_t));
    if (new_map == NULL) {
        fprintf(stderr, "Memory allocation failure");
        exit(1);
    }

    new_map->rows = current_map->rows;
    new_map->columns = current_map->columns;

    new_map->grid = malloc(current_map->rows * current_map->columns * sizeof(spot_t*));
    new_map->players = malloc(current_map->rows * current_map->columns * sizeof(person_t*));

    if (new_map->grid == NULL || new_map->players == NULL) {
        fprintf(stderr, "Memory allocation failure");
        exit(1);
    }

    // Clone grid
    for (int i = 0; i < current_map->rows * current_map->columns; i++) {
        if (current_map->grid[i] != NULL) {
            new_map->grid[i] = spot_clone(current_map->grid[i], i, 0); // Assuming spot_clone is a function that correctly clones a spot
        } else {
            exit(1);
        }
    }
    // Clone players
    for (int i = 0; i < current_map->rows * current_map->columns; i++) {
        if (current_map->players[i] != NULL) {
            new_map->players[i] = person_clone(current_map->players[i]); // Assuming person_clone is a function that correctly clones a person
        } else {
            new_map->players[i] = NULL;
        }
    }
    return new_map;
}

person_t** get_players(map_t* map){
    return map->players;
}

int get_rows(map_t * map){
    return map->rows;
}

int get_columns(map_t* map){
    return map->columns;
}

// void update_visibility(spot_t** grid, int reference){

// }


// void check_visibility(spot_t** grid, int reference){

// }