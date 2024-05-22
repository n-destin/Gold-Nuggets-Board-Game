#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"
#include "person.h"
#include <unistd.h> 

int main(const int argc, char* argv[]) {
    if (argc == 2) {  
        char* map_path = argv[1];
        map_t* map = map_new(map_path); 
        long seed = (argc == 3) ? atol(argv[2]) : getpid();
        srand(seed);
        // map_print(map, stdout);
        person_t* a = insert_person(map, 'a', "someone");
        person_t* b = insert_person(map, 'b', "someone");
        person_t* c = insert_person(map, 'c', "someone");
        person_t* d = insert_person(map, 'd', "someone");
        map_print(map, stdout);
        move_person(map, a, 'h');
        move_person(map, b, 'l');
        move_person(map, c, 'j');
        move_person(map, d, 'k');
        move_person(map, a, 'y');
        move_person(map, b, 'u');
        move_person(map, c, 'b');
        move_person(map, d, 'n');
        move_person(map, a, 'a');
        move_person(map, b, 'a');
        move_person(map, c, 'a');
        move_person(map, d, 'a');
        gold_initialize(map);
        map_print(map, stdout);
        map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}
