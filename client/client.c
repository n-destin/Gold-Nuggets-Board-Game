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
#include <unistd.h>
#include <sys/ioctl.h>

// References to required functions
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
void spectate(char* serverHost, char* serverPort, addr_t serverAddress);
void play(char* hostname, char* port, char* player_name, addr_t serverAddress);
int is_key(char c);

char real_name;
int NR;
int NC;
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

    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }
    NR =  w.ws_row;
    NC = w.ws_col;
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
        printf("Re \n");
        // Check if server-messaging system initializes successfully
        //bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
        //if (!ok) {fprintf(stderr, "Not ok"); exit(4);}
        //message_done();

        printf("H \n");
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
    char* message = "SPECTATE";
    message_send(serverAddress, message);

    // Start spectating
    bool ok = message_loop(&serverAddress, 0, NULL, handleInput, handleMessage);
    if (!ok) {fprintf(stderr, "Not ok"); exit(4);}
    message_done();
}

/* Play function to tell server to setup client as a player, allows game interaction
@inputs - servername, port (1-26), player name, IP of server
@outputs - messages recieved from server
*/
void play(char* hostname, char* port, char* player_name, addr_t serverAddress) {
    
    // Tell the server we're playing
    char message[100] = "PLAY ";
    if (sizeof(message) >= (5 + strlen(player_name) + 1)) {
      strcat(message, player_name);
    } else {
      fprintf(stderr, "Destination buffer is not large enough.\n");
      exit(1);
    }
    printf("%s", message);
    message_send(serverAddress, message);

    // Start playing
    bool ok = message_loop(&serverAddress, 0, NULL, handleInput, handleMessage);
    if (!ok) {fprintf(stderr, "Not ok"); exit(4);}
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
    char* quitMessage = "KEY Q";
    message_send(*serverp, quitMessage);
    return true;
  } else {
    // strip trailing newline
    const int len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
      line[len-1] = '\0';
    }

    // send as message to server
    char key[6] = "KEY ";
    if (len == 2) {
      if (is_key(line[0])) {
        size_t key_len = strlen(key);
        key[key_len] = line[0];
        key[key_len + 1] = '\0';
        printf("Key: %s\n", key);
        message_send(*serverp, key);
      }
    }

    // normal case: keep looping
    return false;
  }
}

int is_key(char c) {
    return c == 'k' || c == 'h' || c == 'j' || c == 'l' || c == 'y' || c == 'u' || c == 'b' || c == 'n' || c == 'Q';
}

/**************** handleMessage ****************/
/* Datagram received; print it.
 * We ignore 'arg' here.
 * Return true if any fatal error.
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  printf("Client recieved: '%s'\n", message);
  if (strncmp(message, "OK", 2) == 0) {
    if (sscanf(message, "OK %c", &real_name) == 1) {
    } else {
      fprintf(stderr, "Failed to parse the message.\n");
    }
  }
  else if (strncmp(message, "GRID", 4) == 0){
    int gridRows = 0; 
    int gridColumns = 0;
    if (sscanf(message, "GRID %d %d", &gridRows, &gridColumns) == 2) {
    } else {
      fprintf(stderr, "Failed to parse the message.\n");
    }

    if(!(gridRows + 1 < NR && gridColumns + 1 < NC)){
      fprintf(stdout, "Waiting for player to resize their grid");
    }
  }
  else if (strncmp(message, "GOLD", 4) == 0){
    int n = 0; 
    int p = 0;
    int r = 0;
    if (sscanf(message, "GOLD %d %d %d", &n, &p, &r) == 3) {
    } else {
      fprintf(stderr, "Failed to parse the message.\n");
    }
    fprintf(stdout, "Player %c has %d nuggets (%d nuggets unclaimed)", real_name, p, r); 
  }
  else if (strncmp(message, "DISPLAY", 7) == 0){
    fprintf(stdout, "%s", message);
  }
  else if (strncmp(message, "QUIT", 2) == 0){
    char reason[200];
    memset(reason, 0, sizeof(reason));
    if (sscanf(message, "QUIT %199[^\n]", reason) == 1) {
      fprintf(stdout, "Reason: %s\n", reason);
    } else {
      fprintf(stderr, "Failed to parse the message.\n");
    }
    fflush(stdout);
    exit(0);
  }
  else if (strncmp(message, "ERROR", 2) == 0){
    char reason[200];
    memset(reason, 0, sizeof(reason));
    if (sscanf(message, "QUIT %199[^\n]", reason) == 1) {
      fprintf(stdout, "Reason: %s\n", reason);
    } else {
      fprintf(stderr, "Failed to parse the message.\n");
    }
  }
  fflush(stdout);
  return false;
}
