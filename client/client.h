/*
client.h
CS50 - Nuggets - Social Security
@SebastianFrazier26
header file for client.c
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../map/spot.h"
#include "../map/map.h"
#include "../map/person.h"
#include "../support/message.h"
#include "../support/log.h"

/**************** main ****************/
/* main function to handle parsing args and initializing game/spectator modes.
MODES:
- Spectate if 2 arguments passed
- Play if 3 arguments passed
EXITS:
- 0 if successful
- 1 if invalid number of arguments
- 2 if host/port is invalid
- 3 if messaging initialization fails
- 4 if message transmition module fails */
int main(const int argc, char* argv[]);

/**************** spectate ****************/
/* Spectate function to tell server to setup client as a spectator, doesn't allow game interaction
* spectators can watch the game unfold and see total remaining gold count
* @inputs - servername, port (1-26), IP of server
* @outputs - messages recieved from server into curses window */
void spectate(char* serverHost, char* serverPort, addr_t serverAddress);

/**************** play ****************/
/* Play function to tell server to setup client as a player, allows game interaction through keypresses
| keystroke | Action                                      |
| --------- | ------------------------------------------- |
| `Q`       | quit the game                               |
| `h`       | move left, if possible                      |
| `l`       | move right, if possible                     |
| `j`       | move down, if possible                      |
| `k`       | move up, if possible                        |
| `y`       | move diagonally up and left, if possible    |
| `u`       | move diagonally up and right, if possible   |
| `b`       | move diagonally down and left, if possible  |
| `n`       | move diagonally down and right, if possible |
* @inputs - servername, port (1-26), player name, IP of server
* @outputs - messages recieved from server as input in curses window */
void play(char* hostname, char* port, char* player_name, addr_t serverAddress);

/**************** static functions ****************/
/*************************************************/

/**************** handleInput ****************/
/* stdin has input ready; read a line and send it to the client.
 * Return true if the message loop should exit, otherwise false.
 * i.e., return true if EOF was encountered on stdin, or fatal error. 
 * inputs - Server address
 * We strip trailing new lines ("\n"), then check if a key is a valid input. If it is send it to the server.
 * 
 * We log all keystrokes/inputs
 * 
 * Note - Some logic borrowed from miniclient @DavidKotz */
static bool handleInput(void* arg);

/**************** is_key ****************/
/* Check if a provided input is a valid key
 * takes in a keystroke
 * outputs the keystroke iff its valid
 * 
 * Valid strokes if player: k, h, j, l, y u, b, n, Q
 * Valid strokes if spectator: Q
 */
int is_key(char input);

/**************** handleMessage ****************/
/* Datagram received; print it. We ignore 'arg' here.
 * Return true if any fatal error.
 * Recieve messages from the server and check their validity/display them
 * 
 * First, we check if the window is of the right size, if it isn't wait for the user to resize is
 * Once window is of right size, we print the map to CURSES with the header describing our gold count
 * 
 * We log all messages from the server
 * 
 * If the game should end for every reason, we display why: q press, game over, error, etc.
 *  
 * Note - Some logic borrowed from miniclient @DavidKotz */
static bool handleMessage(void* arg, const addr_t from, const char* message);

/**************** init_curses ****************/
/* Initialize curses window and define proper settings
* No inputs or outputs, sets up CURSES window
* Hide user types, cursor.
* Remove line buffering
* Enable keystrokes
* Define window boundaries
* Get screen size so we can check if its large enough
* Setup colors
*/
static void init_curses(void);