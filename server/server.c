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
#include "../map/visibility.h"
#include <ctype.h>
#include "server.h"

#define MaxNameLength 50   // max number of chars in playerName
#define MaxPlayers 26      // maximum number of players
#define GoldTotal 250      // amount of gold in the game
#define GoldMinNumPiles 10 // minimum number of gold piles
#define GoldMaxNumPiles 30 // maximum number of gold piles



typedef struct game {
    map_t* map; // this is the base map
    int num_players;
    int num_spectators; // binary
    int total_gold;
    int remaining_gold;
    addr_t spectator_address;
} game_t;

game_t game;

typedef struct sockaddr_in addr_t;

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
    srand(seed); // seeding
    initialize_game(map_filepathname); // creating an initial state of the game
    setup_network(game); // sets up the server port
    handle_client_messages();
    return 0;
}

// initialized the initial state of the game
void initialize_game(char* file_pathname) {
    game.num_players = 0;
    game.num_spectators = 0;
    game.total_gold = GoldTotal;
    game.remaining_gold = GoldTotal;
    game.spectator_address = message_noAddr(); // intialize it to no address  
    game.map = map_new(file_pathname);
    gold_initialize(game.map);
}

// calls message_loop for recieveing client messages
void handle_client_messages() {
    fprintf(stderr, "Handling client messages\n");
    message_loop(&game, 0, NULL, NULL, handle_message); // wait for the client's messages
}

// sends the summary to players and spectators at the end of the game 
void send_summary_and_quit(game_t * game) {
    char summary[1024] = "QUIT GAME OVER:\n";
    person_t** players = get_players(game->map);
    for (int i = 0; i < (get_rows(game->map) * get_columns(game->map)); i++) {
        if(players[i] != NULL){
        char playerSummary[128];
            snprintf(playerSummary, sizeof(playerSummary), "%c %d %s\n", person_getLetter(players[i]), person_getGold(players[i]), person_getName(players[i]));
            strncat(summary, playerSummary, sizeof(summary) - strlen(summary) - 1);
        }
    }
    // send to players
    for (int i = 0; i < (get_rows(game->map) * get_columns(game->map)); i++) {
        if(players[i] != NULL){
            message_send(person_getAddr(players[i]), summary);
        }
    }
    // send to the spectator
    if (game->num_spectators > 0) {
        message_send(game->spectator_address, summary);
    }
    printf("%s", summary);
}

//sends updates to all the players and spectator 
void broadcast(game_t* game)
{
    char* string_map = NULL;
    person_t** players = get_players(game->map);
    int gold_collected = 0;
    for(int index = 0; index < (get_rows(game->map) * get_columns(game->map)); index++){ // calculate the total gold collected so far
        person_t * person = players[index];
        if (person != NULL){
            gold_collected += person_getGold(person); // get each person's gold
        }
    }
    game->remaining_gold = 250 - gold_collected;
    for(int index = 0; index < (get_rows(game->map) * get_columns(game->map)); index++){
        person_t * person = players[index];
        if (person != NULL){
            visibility(person, game->map); // modify the map visibility before sending
            string_map = grid_to_string_player(game->map, person_getLetter(person));
            char send_gold[50];
            char send_display[strlen(string_map) + 10];
            sprintf(send_display, "DISPLAY\n%s", string_map);
            sprintf(send_gold, "GOLD %d %d %d ", 0, person_getGold(person), game->remaining_gold);
            // send messages to both players and spectator
            message_send(person_getAddr(person), send_gold);
            message_send(person_getAddr(person), send_display);
            free(string_map);
        }
    }
    if(message_isAddr(game->spectator_address)){
        char send_gold_spectator[50];
        string_map = grid_to_string_spectator(game->map);
        char send_display_spectator[strlen(string_map) + 10];  // get the string rep of the map
        sprintf(send_display_spectator, "DISPLAY\n%s", string_map); // create a string map for spectator
        sprintf(send_gold_spectator, "GOLD %d", game->remaining_gold);
        // send messages to both client and spectator
        message_send(game->spectator_address, send_gold_spectator);
        message_send(game->spectator_address, send_display_spectator);
        free(string_map);
    }
}


// initializes the listening port for the server
void setup_network(game_t game) {
    char port_string[120];
    int port = message_init(stderr);
    if (port == 0) {
        fprintf(stderr, "Failed to initialize messaging system\n");
        exit(1);
    }
    sprintf(port_string, "%d", port); // get the port_string
}

// finds the person who sent the message.
person_t* find_sender(addr_t from, game_t* game){
    person_t ** players = get_players(game->map);
    for(int index = 0; index < (get_columns(game->map) * get_rows(game->map)); index++){
        person_t* person = players[index];
        if(person != NULL){
            if(message_eqAddr(person_getAddr(players[index]), from)){ // get the person
                return person;
            }
        }
    }
    return NULL;
}

// checks if a string had no space
static int contains_non_space(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 1; 
        }
        str++;
    }
    return 0; 
}

// handles the clients messages 
bool handle_message(void* arg, const addr_t from, const char* message) {
    game_t* game = (game_t*)arg; // change state -> game. consistency in naming
    person_t* sender = find_sender(from, game);
    if (strncmp(message, "PLAY ", 5) == 0) {
        if (game->num_players < MaxPlayers) { // check if we are not above the limit

            char name[25];
            char character = 'A' + game->num_players; // getting a chracters
            strncpy(name, message + 5, MaxNameLength - 1); 
            if(!contains_non_space(name)){
                message_send(from, "QUIT Sorry - you must provide player's name.");
            }
            person_t* new_player = insert_person(game->map, character, name, from); // create a new person
            if(new_player == NULL){
                message_send(from, "QUIT Sorry there is no more space in the grid");
            }
            char gridMessage[100]; 
            sprintf(gridMessage, "GRID %d %d", get_rows(game->map), get_columns(game->map)); // creates the grid message
            message_send(from, gridMessage);
            if(new_player == NULL){
                fprintf(stderr, "Something went wrong when insertfing a player");
                exit(1);
            }
            char response[256];
            sprintf(response, "OK %c\n", character);
            message_send(from, response); // welcomes the new player
            game->num_players++;

        } else {
            message_send(from, "QUIT Game is full: no more players can join.");
        }
    } else if (strncmp(message, "SPECTATE", 8) == 0) {  // handles joining of spetators
        if (game->num_spectators != 0) {
            message_send(game->spectator_address, "QUIT You have been replaced by a new spectator.");
        }
        game->spectator_address = from;
        game->num_spectators = 1;
    } else if (strncmp(message, "KEY ", 4) == 0) { //handles key messages. (movements and quitting)
        char direction = message[4]; 
        if(direction == 'Q'){ 
            if (message_eqAddr(from, game->spectator_address)){ // when the the spectator quits
                message_send(from, "QUIT Thanks for watching!");
                game->num_spectators = 0;
            }else{
                message_send(from, "QUIT Thanks for playing!"); // when the player quits

            } 
        }
        else{
            if(isupper((unsigned char)direction)){
                direction = tolower(direction);
                while(move_person(game->map, sender, direction)){ // move the person if it is capital
                    visibility(sender, game->map);
                }
            }
            else{
                move_person(game->map, sender, direction); // move once
            }
        }
    } else {
        fprintf(stderr, "Logged '%s.\n", message);
    }
    broadcast(game);

    if(game->remaining_gold == 0){
        // send the quit message to everyone and summary and then change everything
        send_summary_and_quit(game);
        map_delete(game->map);
        return true;
    }
    return false; 
}
