#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "message.h" 
#include "log.h"
#include "map.h"
#include "person.h"

#define MaxNameLength 50   // max number of chars in playerName
#define MaxPlayers 26      // maximum number of players
#define GoldTotal 250      // amount of gold in the game
#define GoldMinNumPiles 10 // minimum number of gold piles
#define GoldMaxNumPiles 30 // maximum number of gold piles


typedef struct {
    map_t* map;
    player_t players[MAX_PLAYERS];
    int numPlayers;
    int numSpectators;
    int totalGold;
    int goldRemaining;
    addr_t spectatorAddr;
} game_state_t;

game_state_t gameState;

void initialize_game(map_t * map);
void handle_client_messages();
void send_summary_and_quit();
void setup_network();
bool handle_message(void* arg, const addr_t from, const char* message);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <mapfile> [seed]\n", argv[0]);
        exit(1);
    }

    map_filepathname = argv[1];

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
    gameState.numPlayers = 0;
    gameState.numSpectators = 0;
    gameState.totalGold = GoldTotal;
    gameState.goldRemaining = GoldTotal;
    gameState.spectatorAddr = message_noAddr();

    // Drop gold piles randomly on the map
    int numPiles = GoldMinNumPiles + rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1);
    for (int i = 0; i < numPiles; i++) {
        
        // Placeholder for actual map and gold placement logic
    }

}

void handle_client_messages() {
    printf("Handling client messages\n");
    message_loop(&gameState, 0, NULL, NULL, handle_message);
}

void send_summary_and_quit() {
    printf("Game over, sending summary and quitting\n");

    char summary[1024] = "GAME OVER:\n";
    for (int i = 0; i < gameState.numPlayers; i++) {
        char playerSummary[128];
        snprintf(playerSummary, sizeof(playerSummary), "%c %d %s\n", 
                 'A' + i, gameState.players[i].purse, gameState.players[i].name);
        strncat(summary, playerSummary, sizeof(summary) - strlen(summary) - 1);
    }

    // Send QUIT message with summary to all clients
    for (int i = 0; i < gameState.numPlayers; i++) {
        // Placeholder: send message to each player
    }
    if (gameState.numSpectators > 0) {
        message_send(gameState.spectatorAddr, "QUIT");
    }

    printf("%s", summary);
}

void setup_network() {
    FILE *logFP = stderr;
    int port = message_init(logFP);
    if (port == 0) {
        fprintf(stderr, "Failed to initialize messaging system\n");
        exit(1);
    }
    message_send()
}

bool handle_message(void* arg, const addr_t from, const char* message) {
    game_state_t* state = (game_state_t*)arg;

    // Handle different types of messages from clients
    if (strncmp(message, "JOIN ", 5) == 0) {
        if (state->numPlayers < MAX_PLAYERS) {
            person_t* newPlayer = person_new();
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
                // Placeholder: send updated map to each player
            }
            if (state->numSpectators > 0) {
                // Placeholder: send updated map to spectator
            }
        } else {
            message_send(from, "INVALID PLAYER");
        }
    }

    return false; // Continue handling messages
}
