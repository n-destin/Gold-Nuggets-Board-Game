#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../../tse-Mr-Axinn/libcs50/file.h"
#include "../../tse-Mr-Axinn/libcs50/hashtable.h"
#include "../../tse-Mr-Axinn/libcs50/set.h"
#include "spot.h"
#include "map.h"
#include "person.h"

typedef struct map{
    int rows;
    int columns;
    spot_t** grid;
    person_t** players; 
} map_t;

map_t* map_new(char* path){
    map_t* map = calloc(1, sizeof(map_t));
    if(map == NULL){
        fprintf(stderr, "Memory allocation failure");
        exit(1);
    }
    FILE* in = NULL;  
    in = fopen(path, "r"); 
    if (in == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        exit(1);
    }
    fprintf(stderr, "Found file %s\n", path);
    map_validate(map, in);
    fclose(in);  
    in = fopen(path, "r");
    map_load(map, in);
    fclose(in);
    return map;
}

void map_validate(map_t* map, FILE* in){
    char* line = NULL;
    int numRows = 0;
    int numColumns = 0;
    while((line = file_readLine(in)) != NULL){ 
        int line_size = (int)strlen(line);
        if(line_size > numColumns){
            numColumns = line_size;
        }
        numRows++; 
        free(line);
    }
    if(numColumns < 3 || numRows < 3){
        fprintf(stderr, "Map doesn't have correct amount of rows/colums");
    }
    map->grid = calloc((numColumns*numRows), sizeof(spot_t*));
    map->players = calloc((numColumns*numRows), sizeof(person_t*));
    for (int i = 0; i < (map->rows*map->columns); i++) {
        map->players[i] = NULL;
    }
    if(map->players == NULL || map->grid == NULL){
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }
    map->columns = numColumns;
    map->rows = numRows;
    free(line);
}

void map_load(map_t* map, FILE* in){
    char c;
    int map_size = (map->rows * map->columns);
    for (int i = 0; i < map_size; i++) {
        map->grid[i] = NULL;
    }
    
    char* line = NULL;
    int current_row = 0; 
    while((line = file_readLine(in)) != NULL){ 
        int i = 0;
        while ((c = line[i]) != '\0') {
            spot_t* spot = spot_new();
            c = line[i];
            if(!spot_insert(spot, c)){
                fprintf(stderr, "Error inserting item %c", c);
                exit(1);
            }
            map->grid[i+(current_row*map->columns)] = spot; 
            i++;
        }
        current_row++;
        free(line);
    }
    free(line);
}

void map_print(map_t* map, FILE* out){
    
    for(int row = 0; row < map->rows; row++){
        for(int column = 0; column < map->columns; column++){
            if(map->grid[column + (row*map->columns)] == NULL){
                fprintf(out, "%c", '\n');
                break;
            }
            else if(map->players[column + (row*map->columns)] != NULL){
                person_t* person = map->players[column + (row*map->columns)];
                fprintf(out, "%c", person_getName(person));
            }else{
                spot_t* spot = map->grid[column + (row*map->columns)];
                fprintf(out, "%c", spot_item(spot));
            }
        }
        fprintf(out, "%c", '\n');
    }
}

void map_delete(map_t* map){
    for(int i = 0; i < (map->columns*map->rows); i++){
        if(map->grid[i] != NULL){
            spot_delete(map->grid[i]);
        }
        if(map->players[i] != NULL){
            person_delete(map->players[i]);
        }
    }
    free(map->grid);
    free(map->players);
    free(map);
}

bool move_person(map_t* map, person_t* person, char direction){
    int current_pos = person_getPos(person);
    int row = (int) (current_pos / map->columns);
    int column = current_pos - (row*map->columns);
    int new_pos = 0;
    if(direction == 'w'){
        if(row == 0){
            return false;
        }
        new_pos = current_pos - map->columns;
    }
    else if(direction == 'a'){
        if(column == 0){
            return false;
        }
        new_pos = current_pos - 1;
    }
    else if(direction == 's'){
        if(row == map->rows){ 
            return false;
        }
        new_pos = current_pos + map->columns;
    }
    else if(direction == 'd'){
        if(column == map->columns){ 
            return false;
        }
        new_pos = current_pos + 1;
    }
    if(map->players[new_pos] != NULL){
        map->players[current_pos] = map->players[new_pos];
        map->players[new_pos] = person; 
    }
    else if(spot_item(map->grid[new_pos]) == '.' || spot_item(map->grid[new_pos]) == '#' || spot_item(map->grid[new_pos]) == '*'){
        person_setPos(person, new_pos);
        map->players[current_pos] = NULL; 
        map->players[new_pos] = person;
        if(spot_item(map->grid[new_pos]) == '*'){
            person_addGold(person);
        }
    }
    else{
        return false;
    }
    return true;
}

person_t* insert_person(map_t* map, char c){
    set_t* indexes = set_new();
    int num_spaces = 0;
    int final_index = 0;
    for(int i = 0; i < (map->columns*map->rows); i++){
        if((spot_item(map->grid[i]) == '.') && map->players[i] == NULL){
            char key[20];
            sprintf(key, "%d", num_spaces);
            
            int* value = malloc(sizeof(int)); 
            if (value != NULL) { 
                *value = i;
                set_insert(indexes, key, value); 
            } else {
                fprintf(stderr, "Memory failure");
            }
            set_insert(indexes, key, value);
            num_spaces++;
        }
    }
    if(num_spaces == 0){
        return NULL;
    }
    
    srand(time(NULL));
    int random_number = rand() % (num_spaces+1);
    char random_string[20];
    sprintf(random_string, "%d", random_number);
    int* temp = set_find(indexes, random_string);
    final_index = *temp;
    
    person_t* person = person_new(c);
    person_setPos(person, final_index);
    map->players[final_index] = person;
    set_delete(indexes, namedelete);
    return person;
}

void namedelete(void* item)
{
  if (item != NULL) {
    free(item);   
  }
}