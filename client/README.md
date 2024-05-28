# client

## Purpose
This module creates the client for the nuggets game. The client handles user interactions through keystrokes and sends messages
to the server to indicate game-state changes. The client additionally handles the display.

## Desgin
The client gets a message containing a 'GRID' from the server which is used to determine window sizing and eventually display the map.
It also opens a log file to track game-state updates, error messages, and keystrokes. 

We parse passed arguments for any startup failures or invalid inputs

If the user is joining as a spectator, we initialize the CURSES window and only allow the keystroke 'Q' for the spectator to quit.

If the user joins as a player, we initialize the CURSES window and allow for the keystrokes: k, h, j, l, y u, b, n, Q to handle movement
and quitting

Once a user has joined, we wait for them to resize their screen to fit our required GRID dimensions if necessary.

## Implementation

The implementation is described in depth in the header file. Below are brief descriptions of the functions used in client.c

int main(const int argc, char* argv[]) parses arguments, and initializes the game. It handles everything by passing off responsibilities to other functions. We exit 0 if all is successful, nonzero otherwise

void spectate(char* serverHost, char* serverPort, addr_t serverAddress) tells the server that we're joining as a spectator

void play(char* hostname, char* port, char* player_name, addr_t serverAddress) tells the server that we're joining as a player

static bool handleInput(void* arg) handles keystrokes from a user and sends them to the server

int is_key(char input) checks if a keystroke is valid

static bool handleMessage(void* arg, const addr_t from, const char* message) handles messages from the server and displays them in the clientside CURSES window

static void init_curses(void) initializes the CURSES window and adjusts its settings

## Usage
First we run <make> in terminal inside the client directory to compile client.c
After this we can startup the client with the command:
```bash
./client hostname port [playername]
```
where the playername is an optional parameter and the port is given by the server upon startup.

If we pass all 3 arguments in terminal, we will be entered into the nuggets map as a player. Otherwise if no name is given we will be a
spectator.

Once we run this command, we are connected to server. At this point we resize the window if necessary, otherwise we can play/spectate.

## Limitations
As per the requirement spec, the client will NOT handle window resizing after it has already been set to a valid size. IE, the player should NOT adjust the window after the map has been loaded
