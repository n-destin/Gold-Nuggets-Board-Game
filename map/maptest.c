#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"
#include "person.h"
#include <unistd.h> 
#include "../support/message.h"

int main(const int argc, char* argv[]) {
    if (argc == 2) {  
        char* map_path = argv[1];
        
        map_t* map = map_new(map_path); 
        long seed = (argc == 3) ? atol(argv[2]) : getpid();
        srand(seed);
        addr_t dummy = message_noAddr();
        // map_print(map, stdout);

        person_t* a = insert_person(map, 'a', "someone", dummy);
        person_t* b = insert_person(map, 'b', "someone", dummy);
        person_t* c = insert_person(map, 'c', "someone", dummy);
        person_t* d = insert_person(map, 'd', "someone", dummy);
        gold_initialize(map);
        map_print(map, stdout);
        
        move_person(map, a, 'h');
        move_person(map, b, 'l');
        move_person(map, c, 'j');
        move_person(map, d, 'k');
        move_person(map, a, 'y');
        move_person(map, b, 'u');
        move_person(map, c, 'b');
        move_person(map, d, 'n');
        fprintf(stdout, "Person has: %d gold\n", person_getGold(a));
        fprintf(stdout, "Person has: %d gold\n", person_getGold(b));
        fprintf(stdout, "Person has: %d gold\n", person_getGold(c));
        fprintf(stdout, "Person has: %d gold\n", person_getGold(d));
        
        map_print(map, stdout);
        map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}
