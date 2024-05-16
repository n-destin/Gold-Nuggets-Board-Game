#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"
#include "person.h"

int main(const int argc, char* argv[]) {
    if (argc == 2) {  
        char* map_path = argv[1];
        map_t* map = map_new(map_path);
        map_print(map, stdout);
        person_t* a = insert_person(map, 'a');
        
        
        person_t* b = insert_person(map, 'b');
        person_t* c = insert_person(map, 'c');
        person_t* d = insert_person(map, 'd');
        person_t* e = insert_person(map, 'e');
        person_t* f = insert_person(map, 'f');
        person_t* g = insert_person(map, 'g');
        person_t* h = insert_person(map, 'h');
        person_t* i = insert_person(map, 'i');
        person_t* j = insert_person(map, 'j');
        map_print(map, stdout);
        move_person(map, a, 'a');
        move_person(map, b, 'a');
        move_person(map, c, 'a');
        move_person(map, d, 'a');
        
        map_print(map, stdout);
        
        map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}