#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"


int main(const int argc, char* argv[]) {
    if (argc == 2) {  
        char* map_path = argv[1];
        map_t* map = map_new(map_path);
        printf("here\n");
        map_print(map, stdout);
        printf("here\n");
        insert_person(map, 'a');
        insert_person(map, 'b');
        insert_person(map, 'c');
        printf("here\n");
        map_print(map, stdout);
        map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}