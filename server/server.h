#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h> // for sockaddr_in
#include <stdbool.h>    // for bool type

// Constants defining gameplay parameters
#define MaxNameLength 50   // Maximum number of characters in playerName
#define MaxPlayers 26      // Maximum number of players allowed in the game
#define GoldTotal 250      // Total amount of gold available in the game
#define GoldMinNumPiles 10 // Minimum number of gold piles
#define GoldMaxNumPiles 30 // Maximum number of gold piles

// Forward declarations from other headers for structures used
typedef struct map map_t;
typedef struct person person_t;

// Type definition for storing network addresses
typedef struct sockaddr_in addr_t;

typedef struct game game_t;

// Function Prototypes

/**
 * Initializes the game with the map provided and sets initial game parameters.
 * @param file_pathname The path to the file containing the map layout.
 */
void initialize_game(char* file_pathname);

/**
* Continuously handles messages from clients connected to the server.
*/
void handle_client_messages();

/**
 * Sends a summary of the game to all players and quits the game.
 * @param game A pointer to the game structure containing game state information.
 */
void send_summary_and_quit(game_t* game);

/**
 * Handles individual messages received from clients and updates the game state accordingly.
 * @param arg A void pointer to the game state, used for passing multiple data types.
 * @param from The address structure of the message sender.
 * @param message The message content received from a client.
 * @return Returns true if the message was handled successfully, otherwise false.
 */
bool handle_message(void* arg, const addr_t from, const char* message);

/**
 * Sets up the network communications necessary for the server to receive client messages.
 */
void setup_network();

/**
 * Helper function to check if a string contains non-space characters.
 * @param str The string to be checked.
 * @return Returns 1 if non-space characters are found, otherwise 0.
 */
static int contains_non_space(const char *str);

#endif // SERVER_H
