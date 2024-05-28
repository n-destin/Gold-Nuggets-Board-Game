/*
* Unit test for map: test basic map functions, gold, moving, and printing of map
* Izzy Axinn - 2024
*/


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
        
        map_t* map = map_new(map_path); //Create new map
        long seed = (argc == 3) ? atol(argv[2]) : getpid();
        srand(seed);
        addr_t dummy = message_noAddr();
        

        //Inserting people
        person_t* a = insert_person(map, 'a', "someone", dummy);
        person_t* b = insert_person(map, 'b', "someone", dummy);
        person_t* c = insert_person(map, 'c', "someone", dummy);
        person_t* d = insert_person(map, 'd', "someone", dummy);

        //Initialize gold
        gold_initialize(map); 


        map_print(map, stdout); //Map in current state
        
        //Moving players
        move_person(map, a, 'h');
        move_person(map, a, 'h');
        move_person(map, a, 'h');
        move_person(map, a, 'h');
        move_person(map, b, 'l');
        move_person(map, b, 'l');
        move_person(map, b, 'l');
        move_person(map, c, 'h');
        move_person(map, d, 'l');

        //Testing their gold
        fprintf(stdout, "Person has: %d gold\n", person_getGold(a));
        fprintf(stdout, "Person has: %d gold\n", person_getGold(b));
        fprintf(stdout, "Person has: %d gold\n", person_getGold(c));
        fprintf(stdout, "Person has: %d gold\n", person_getGold(d));
        
        //Printing map and deleting
        map_print(map, stdout);
        map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}
