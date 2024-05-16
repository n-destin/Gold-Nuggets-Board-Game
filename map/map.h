#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "person.h"


typedef struct map map_t;

map_t* map_new(char* path);
void map_validate(map_t* map, FILE* in);
void map_load(map_t* map, FILE* in);
void map_print(map_t* map, FILE* out);
void map_delete(map_t* map);
void set_characters();
void namedelete(void* item);
person_t* insert_person(map_t* map, char c);
bool move_person(map_t* map, person_t* person, char direction);
