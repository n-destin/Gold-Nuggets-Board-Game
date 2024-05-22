#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "../support/message.h" 
#include "../support/log.h"
#include "../map/map.h"
#include "../map/person.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"

#define MaxNameLength 50   // max number of chars in playerName
#define MaxPlayers 26      // maximum number of players
#define GoldTotal 250      // amount of gold in the game
#define GoldMinNumPiles 10 // minimum number of gold piles
#define GoldMaxNumPiles 30 // maximum number of gold piles



typedef struct {
    map_t* map; // this is the base map
    hashtable_t* maps;
    int num_players;
    int num_spectators; // binary
    int total_gold;
    int remaining_gold;
    addr_t spectator_address;
} game_t;

game_t game;

typedef struct sockaddr_in addr_t;

void initialize_game();
void handle_client_messages();
void send_summary_and_quit();
bool handle_message(void* arg, const addr_t from, const char* message);
void setup_network();

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <mapfile> [seed]\n", argv[0]);
        exit(1);
    }
    char* map_filepathname = argv[1];
    long seed = (argc == 3) ? atol(argv[2]) : getpid();
    if (seed <= 0 && argc == 3) {
        fprintf(stderr, "Seed must be a positive integer\n");
        exit(1);
    }
    // map_t * map = map_new(map_filepathname);
    srand(seed);
    initialize_game(map_filepathname);
    setup_network(game);
    handle_client_messages();
    // send_summary_and_quit(game);
    return 0;
}

void initialize_game(char* file_pathname) {
    game.num_players = 0;
    game.maps = hashtable_new(MaxPlayers);
    game.num_spectators = 0;
    game.total_gold = GoldTotal;
    game.remaining_gold = GoldTotal;
    game.spectator_address = message_noAddr(); // intialize it to no address  
    game.map = map_new(file_pathname);
    gold_initialize(game.map);
}

void handle_client_messages() {
    printf("Handling client messages\n");
    message_loop(&game, 0, NULL, NULL, handle_message);
}

void send_summary_and_quit(game_t * game) {
    printf("Game over, sending summary and quitting\n");

    char summary[1024] = "GAME OVER:\n";
    person_t** players = get_players(game->map);
    for (int i = 0; i < (get_rows(game->map) * get_columns(game->map)); i++) {
        char playerSummary[128];
        snprintf(playerSummary, sizeof(playerSummary), "%c %d %s\n", person_getLetter(players[i]), person_getGold(players[i]), person_getName(players[i]));
        strncat(summary, playerSummary, sizeof(summary) - strlen(summary) - 1);
    }

    // Send QUIT message with summary to all clients
    // for (int i = 0; i < game.numPlayers; i++) {
    //     message_send()
    // }

    // if (game.numSpectators > 0) {
    //     message_send(game.spectatorAddr, "QUIT");
    // }

    printf("%s", summary);
}

void broadcast(char* action, game_t* game, person_t* sender)
{
    person_t** players = get_players(game->map);
    if(strcmp(action, "gridisplay ") == 0){
        for(int index = 0; index < (get_rows(game->map) * get_columns(game->map)); index++){
            person_t * person = players[index];
            if (person != NULL){
                map_t* map = hashtable_find(game->maps, message_stringAddr(person_getAddr(person)));
                char* string_map = grid_to_string(map);
                char send_display[strlen(string_map) + 10];
                char send_grid[50];
                sprintf(send_display, "DISPLAY\n%s", string_map);
                sprintf(send_grid, "GRID %d %d %d ", 0, person_getGold(person), game->remaining_gold);
                message_send(person_getAddr(person), send_grid);
                message_send(person_getAddr(person), send_display);
            }
        }
    }else{
        for(int index = 0; index<(get_columns(game->map) * get_rows(game->map)); index++){
            person_t* person = players[index];
            if(person != NULL){
                printf("How about %s \n\n", person_getName(person));
                map_t* map = hashtable_find(game->maps, message_stringAddr(person_getAddr(person)));
                if(sender == NULL){
                    printf("sender is null");
                }
                if(map == NULL){
                    printf("map is null");
                }
                if(strncmp(action, "insert ", 7) == 0){
                    printf(" brooooo  I reached here");
                    set_person(map, sender);
                }else if(strncmp(action, "move ", 5) == 0){
                    char direction = action[5];
                    // move_person(map, sender, direction);
                }
            }
        }
    }
}

void setup_network(game_t* game) {
    char port_string[120];
    int port = message_init(stderr);
    if (port == 0) {
        fprintf(stderr, "Failed to initialize messaging system\n");
        exit(1);
    }
    sprintf(port_string, "%d", port);
    // broadcast(port_string, game);
}

person_t* find_sender(addr_t from, game_t* game){
    person_t ** players = get_players(game->map);
    for(int index = 0; index < (get_columns(game->map) * get_rows(game->map)); index++){
        person_t* person = players[index];
        if(person != NULL){
            if(message_eqAddr(person_getAddr(players[index]), from)){
                return person;
            }
        }
    }
    return NULL;
}


bool handle_message(void* arg, const addr_t from, const char* message) {
    game_t* game = (game_t*)arg; // change state -> game. consistency in naming
    person_t* sender = find_sender(from, game);
    if(sender == NULL){
        printf("Sender is null");
    }
    // Handle different types of messages from clients
    if (strncmp(message, "PLAY ", 5) == 0) {
        if (game->num_players < MaxPlayers) {
            // printf("I reached here \n ");
            char name[25];
            char character = 'A' + game->num_players; // getting a chracters
            printf("New char: %c \n", character);
            strncpy(name, message + 5, MaxNameLength - 1); 
            person_t* new_player = insert_person(game->map, character, name, from);
            map_t* new_map = clone_map(game->map); // for the new player
            // printf("and here");
            if(!hashtable_insert(game->maps, message_stringAddr(person_getAddr(new_player)), new_map)){ // inserting a a new map to the hashtable of maps
                fprintf(stderr, "unable to append the map in the hashtabel of maps");
                return NULL;
            }
            char action[120];
            sprintf(action, "insert ");
            printf("I reached here new: %s\n", action);
            broadcast(action, game, new_player); // broadcast insertion of a player
            // Respond with player letter and initial map
            char response[256];
            sprintf(response, "OK %c\n", character);
            message_send(from, response);
            game->num_players++;

        } else {
            message_send(from, "GAME FULL");
        }
    } else if (strncmp(message, "SPECTATE ", 9) == 0) {
        if (game->num_spectators != 0) {
            message_send(game->spectator_address, "QUIT");
        }
        game->spectator_address = from;
        char response[256];
        sprintf(response, "WELCOME SPECTATOR\n");
        message_send(from, response);
    } else if (strncmp(message, "KEY ", 4) == 0) {
        printf("I reached in movements \n");
        printf("message: %s \n", message);
        // Handle player movement
        char direction = message[4]; 
        // printf("direction: %c \n", direction);
        // if(sender == NULL){
        //     broadcast("QUIT", game);
        // }
        // mov the person on the main map
        if(sender == NULL){
            printf("the sender is nullagain \n");
        }
        // bool moved = move_person(game->map, sender, direction);
        // map_print(game->map, stdout);
        // printf("moved: %d \n", moved);
        // move the person on all other maps.
        char action[120];
        sprintf(action, "move %c", direction);
        broadcast(action, game, sender);
        } else {
            message_send(from, "INVALID PLAYER");
        }
        // constantly broadcast the GRID, and DISPLAY to all the players
        broadcast("gridisplay ", game, sender);
    return false; 
}
