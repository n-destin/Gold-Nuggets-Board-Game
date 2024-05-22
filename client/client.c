/*
client.c
CS50 - Nuggets - Social Security
@SebastianFrazier26
Handles clientside display and initialization
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../map/spot.h"
#include "../map/map.h"
#include "../map/person.h"
#include "../support/message.h"
#include "../support/log.h"

// References to required functions
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
void spectate(char* serverHost, char* serverPort, addr_t serverAddress);
void play(char* hostname, char* port, char* player_name, addr_t serverAddress);

/* Main function to handle parsing args and initializing game/spectator modes.
MODES:
- Spectate if 2 arguments passed
- Play if 3 arguments passed

EXITS:
- 0 if successful
- 1 if invalid number of arguments
- 2 if host/port is invalid
- 3 if messaging initialization fails
- 4 if message transmition module fails */
int main(const int argc, char* argv[]) {

    // PARSE ARGS
    // Check arg count
    if (argc >= 3) {

        // Check if message module initializes successfully.
        if (message_init(NULL) == 0) {
            exit (3);
        }

        char* serverHost = argv[1];
        char* serverPort = argv[2];
        addr_t server; // IP addr of server

        // Check if host/port name are valid
        if (!message_setAddr(serverHost, serverPort, &server)) {
            fprintf(stderr, "can't form address from %s %s\n", serverHost, serverPort);
            exit(2); //
        }

        // Check if server-messaging system initializes successfully
        bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
        if (!ok) {fprintf(stderr, "Not ok"); exit(4);}

        // If no player_name, spectate
        if (argc == 3) {spectate(serverHost, serverPort, server);} 
        
        // Otherwise, play
        else if (argc == 4) {char* player_name = argv[3]; play(serverHost, serverPort, player_name, server);}

        // If too many args
        else {fprintf(stderr, "Usage: ./client hostname port [playername]\n"); exit(1);}
    }

    // If too few args
    else {fprintf(stderr, "Usage: ./client hostname port [playername]\n"); exit(1);}

    // If all goes well
    exit(0);
}

/* Spectate function to tell server to setup client as a spectator, doesn't allow game interaction
@inputs - servername, port (1-26), IP of server
@outputs - messages recieved from server
*/
void spectate(char* serverHost, char* serverPort, addr_t serverAddress) {

    // Tell the server we're spectating
    const char* message = "SPECTATE";
    message_send(serverAddress, message);

    // Start spectating
    message_loop(&serverAddress, 0, NULL, handleInput, handleMessage);
    message_done();
}

/* Play function to tell server to setup client as a player, allows game interaction
@inputs - servername, port (1-26), player name, IP of server
@outputs - messages recieved from server
*/
void play(char* hostname, char* port, char* player_name, addr_t serverAddress) {

    // Tell the server we're playing
    const char* message = "PLAY";
    message_send(serverAddress, message);

    // Start playing
    message_loop(&serverAddress, 0, NULL, handleInput, handleMessage);
    message_done();
}

/***************** Static Functions ********************/
// Code used from miniclient.c
// Credit @DavidKotz

/**************** handleInput ****************/
/* stdin has input ready; read a line and send it to the client.
 * Return true if the message loop should exit, otherwise false.
 * i.e., return true if EOF was encountered on stdin, or fatal error.
 */
static bool
handleInput(void* arg)
{
  // We use 'arg' to receive an addr_t referring to the 'server' correspondent.
  // Defensive checks ensure it is not NULL pointer or non-address value.
  addr_t* serverp = arg;
  if (serverp == NULL) {
    fprintf(stderr, "handleInput called with arg=NULL");
    return true;
  }
  if (!message_isAddr(*serverp)) {
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }
  
  // allocate a buffer into which we can read a line of input
  // (it can't be any longer than a message)!
  char line[message_MaxBytes];

  // read a line from stdin
  if (fgets(line, message_MaxBytes, stdin) == NULL) {
    // EOF case: stop looping
    return true;
  } else {
    // strip trailing newline
    const int len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
      line[len-1] = '\0';
    }

    // send as message to server
    message_send(*serverp, line);

    // normal case: keep looping
    return false;
  }
}

/**************** handleMessage ****************/
/* Datagram received; print it.
 * We ignore 'arg' here.
 * Return true if any fatal error.
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  printf("'%s'\n", message);
  fflush(stdout);
  return false;
}
