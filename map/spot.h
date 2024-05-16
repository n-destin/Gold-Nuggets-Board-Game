#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../tse-Mr-Axinn/libcs50/hashtable.h"

typedef struct spot spot_t;


spot_t* spot_new(hashtable_t* possible_characters);
bool spot_insert(spot_t* spot, char c);
char spot_item(spot_t* spot);
void spot_delete(spot_t* spot);