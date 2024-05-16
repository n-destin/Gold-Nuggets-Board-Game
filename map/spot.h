#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct spot spot_t;


spot_t* spot_new();
bool spot_insert(spot_t* spot, char c);
char spot_item(spot_t* spot);
void spot_delete(spot_t* spot);
bool get_visibility(spot_t* spot);
void set_visibility(spot_t* spot, bool visibility);