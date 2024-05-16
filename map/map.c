#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../../tse-Mr-Axinn/libcs50/file.h"
#include "../../tse-Mr-Axinn/libcs50/hashtable.h"
#include "../../tse-Mr-Axinn/libcs50/set.h"
#include "spot.h"
#include "map.h"

typedef struct map{
    int rows;
    int columns;
    spot_t** grid; 
} map_t;

hashtable_t* possible_characters = NULL; 

map_t* map_new(char* path){
    map_t* map = malloc(sizeof(map_t));
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
    map->grid = malloc((numColumns*numRows) * sizeof(spot_t*));
    map->columns = numColumns;
    map->rows = numRows;
    free(line);
}

void map_load(map_t* map, FILE* in){
    char c;
    int map_size = (map->rows * map->columns);
    if (map->grid == NULL) {
        fprintf(stderr, "Memory allocation failure");
        free(map);
        exit(1);
    }
    for (int i = 0; i < map_size; i++) {
        map->grid[i] = NULL;
    }
    
    char* line = NULL;
    int current_row = 0; 
    while((line = file_readLine(in)) != NULL){ 
        int i = 0;
        while ((c = line[i]) != '\0') {
            spot_t* spot = spot_new(possible_characters);
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
            spot_t* spot = map->grid[column + (row*map->columns)];
            fprintf(out, "%c", spot_item(spot));
        }
        fprintf(out, "%c", '\n');
    }
}

void map_delete(map_t* map){
    for(int i = 0; i < (map->columns*map->rows); i++){
        if(map->grid[i] != NULL){
            spot_delete(map->grid[i]);
        }
    }
    free(map->grid);
    free(map);
    hashtable_delete(possible_characters, namedelete);
}

void set_characters(){
    possible_characters = hashtable_new(7);
    char characters[7] = {'.', '#', '*', ' ', '|', '+', '-'};
    for (size_t i = 0; i < 7; i++) {
        char* key = &characters[i];
        hashtable_insert(possible_characters, key, "");
    }
}

bool insert_person(map_t* map, char c){
    set_t* indexes = set_new();
    int num_spaces = 0;
    int final_index = 0;
    for(int i = 0; i < (map->columns*map->rows); i++){
        if(spot_item(map->grid[i]) == '.'){
            char key[20];
            sprintf(key, "%d", num_spaces);
            //printf("index: %s ", index);
            int* tem = malloc(sizeof(int)); // Allocate memory for an integer
            if (tem != NULL) { // Check if malloc was successful
                *tem = i; // Assign the value of i to the memory location pointed to by tem
                set_insert(indexes, key, tem); // Insert tem into the set
            } else {
                fprintf(stderr, "Memory failure");
            }
            set_insert(indexes, key, tem);
            num_spaces++;
        }
    }
    if(num_spaces == 0){
        return false;
    }
    
    srand(time(NULL));
    int random_number = rand() % (num_spaces+1);
    char random_string[20];
    int* temp = set_find(indexes, random_string);
    
    final_index = *temp;
    
    spot_t* person = spot_new(possible_characters);
    spot_insert(person, c);
    map->grid[final_index] = person;
    set_delete(indexes, namedelete);
    return true;
}

void namedelete(void* item)
{
  if (item != NULL) {
    free(item);   
  }
}