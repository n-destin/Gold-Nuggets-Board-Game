#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "spot.h"
#include "map.h"


int main(const int argc, char* argv[]) {
    if (argc == 2) {  
        char* map_path = argv[1];
        map_t* map = map_new(map_path);
        map_print(map, stdout);
        //map_delete(map);
    }
    else {
        fprintf(stderr, "Usage: ./maptest map.txt\n");
        exit(1);
    }
}