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
        map_print(map, stdout);
        move_person(map, a, 'h');
        move_person(map, b, 'l');
        move_person(map, c, 'j');
        move_person(map, d, 'k');
        move_person(map, a, 'y');
        move_person(map, b, 'u');
        move_person(map, c, 'b');
        move_person(map, d, 'n');
        map_print(map, stdout);
        
        map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}
