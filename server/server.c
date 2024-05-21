#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "../support/message.h" 
#include "../support/log.h"
#include "../map/map.h"
#include "../map/person.h"

#define MaxNameLength 50   // max number of chars in playerName
#define MaxPlayers 26      // maximum number of players
#define GoldTotal 250      // amount of gold in the game
#define GoldMinNumPiles 10 // minimum number of gold piles
#define GoldMaxNumPiles 30 // maximum number of gold piles


// each players's map has different visibility
typedef struct player_map {
    slot_t** grid;
    person_t * owner;
    slots_t** golds_visible; // the slots of the golds visible from the positions
} player_map_t;

typedef struct {
    map_t* map; // this is the base map
    player_map_t* players_maps[MAX_PLAYERS];
    int num_players;
    int num_spectators; // binary
    int total_gold;
    int remaining_gold;
    addr_t spectator_adress;
} game_t;

game_t game;

void initialize_game(map_t * map);
void handle_client_messages();
void send_summary_and_quit();
bool handle_message(void* arg, const addr_t from, const char* message);

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
    map_t * map = map_new(map_filepathname);
    srand(seed);
    setup_network();
    initialize_game();
    handle_client_messages();
    send_summary_and_quit();
    return 0;
}

void initialize_game(map_t* map) {
    game.num_players = 0;
    game.num_spectators = 0;
    game.total_gold = GoldTotal;
    game.remaining_gold = GoldTotal;
    game.spectator_adress = message_noAddr(); // intialize it to no adress  
    game.map = map_new();
    gold_initialize(game.map, seed);
    
    
}

void handle_client_messages() {
    printf("Handling client messages\n");
    message_loop(&game, 0, NULL, NULL, handle_message);
}

void send_summary_and_quit() {
    printf("Game over, sending summary and quitting\n");

    char summary[1024] = "GAME OVER:\n";
    for (int i = 0; i < game.numPlayers; i++) {
        char playerSummary[128];
        snprintf(playerSummary, sizeof(playerSummary), "%c %d %s\n", 
                 'A' + i, game.players[i].purse, game.players[i].name);
        strncat(summary, playerSummary, sizeof(summary) - strlen(summary) - 1);
    }

    // Send QUIT message with summary to all clients
    for (int i = 0; i < game.numPlayers; i++) {
        // Placeholder: send message to each player
    }
    if (game.numSpectators > 0) {
        message_send(game.spectatorAddr, "QUIT");
    }

    printf("%s", summary);
}

void setup_network(game_t* game) {
    int port = message_init(stderr);
    if (port == 0) {
        fprintf(stderr, "Failed to initialize messaging system\n");
        exit(1);
    }
    broadcast(port, game);
}

void broadcast(char* message, game_t* game)
{
    for(int index = 0; index<game->num_players; index++){
        message_send(map->players[index]->adress, message);
    }
}

int find_sender(void* const addr_t from, game_t* game){
    for(int index = 0; index<game.num_players; index++){
        if(message_eqAddr(game->maps[index]->person->adress, from)){
            return index;
        }
    }
    return -1;
}

bool handle_message(void* arg, const addr_t from, const char* message, const char* map_filepathname) {
    game_t* state = (game_t*)arg; // change state -> game. consistency in naming
    int map_index  = find_sender(from, game); // index of the map of the sender
    // Handle different types of messages from clients
    if (strncmp(message, "JOIN ", 5) == 0) {
        if (state->num_players < MAX_PLAYERS) {
            person_t* new_player = person_new(); 
            player_map_t* new_map = malloc(player_map_t);
            
            new_map->player = new_player;
            strncpy(newPlayer->name, message + 5, MaxNameLength - 1);
            newPlayer->person->gold = 0;
            state->numPlayers++;
            // Respond with player letter and initial map
            char response[256];
            snprintf(response, sizeof(response), "WELCOME %c\n%s", 'A' + state->numPlayers - 1, message);
            message_send(from, response);
        } else {
            message_send(from, "GAME FULL");
        }
    } else if (strncmp(message, "SPECTATE ", 9) == 0) {
        if (state->numSpectators == 0) {
            state->spectatorAddr = from;
            state->numSpectators = 1;
            // Respond with initial map
            char response[256];
            snprintf(response, sizeof(response), "MAP\n%s", message);
            message_send(from, response);
        } else {
            message_send(state->spectatorAddr, "QUIT");
            state->spectatorAddr = from;
            // Respond with initial map
            char response[256];
            snprintf(response, sizeof(response), "MAP\n%s", message);
            message_send(from, response);
        }
    } else if (strncmp(message, "MOVE ", 5) == 0) {
        // Handle player movement
        int playerIndex = message[5] - 'A';
        if (playerIndex >= 0 && playerIndex < state->numPlayers) {
            player_t* player = &state->players[playerIndex];
            char direction = message[6];
            if (move_person(state->map, player, direction)) {
                // Update map and notify all players and spectators
                for (int i = 0; i < state->numPlayers; i++) {
                    // Placeholder: send updated map to each player
                }
                if (state->numSpectators > 0) {
                    // Placeholder: send updated map to spectator
                }
            } else {
                message_send(from, "INVALID MOVE");
            }
        } else {
            message_send(from, "INVALID PLAYER");
        }
    } else if (strncmp(message, "QUIT", 4) == 0) {
        int playerIndex = message[5] - 'A';
        if (playerIndex >= 0 && playerIndex < state->numPlayers) {
            // Remove player from the game
            state->players[playerIndex].name[0] = '\0';
            // Notify other players and spectators
            for (int i = 0; i < state->numPlayers; i++) {
                // send updated map to each player
            }
            if (state->numSpectators > 0) {
                // send updated map to spectator
            }
        } else {
            message_send(from, "INVALID PLAYER");
        }
    }

    return false; 
}
