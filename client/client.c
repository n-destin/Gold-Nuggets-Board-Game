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
#include <ctype.h>
#include <ncurses.h>
#include <curses.h>

// References to required functions
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
void spectate(char* serverHost, char* serverPort, addr_t serverAddress);
void play(char* hostname, char* port, char* player_name, addr_t serverAddress);
int is_key(char c);
static void init_curses(void);

// Init global vars
WINDOW *inputwin; // CURSES
FILE* log_file;  
char real_name;
int NR; // Rows Columns global to capture CURSES window dimensions
int NC;
int n = 0; 
int p = 0;
int r = 0;
bool spectator = false; 

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
    
    log_file = fopen("client.log", "w"); //Opens file
    if (log_file == NULL) {
        fprintf(stderr, "Error opening client.log");
        exit(1);
    }
    flog_init(log_file);
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
          char log_message[40];
          snprintf(log_message, sizeof(log_message), "Can't form address from %s %s\n", serverHost, serverPort);
          flog_e(log_file, log_message);
          flog_done(log_file);
          fclose(log_file);
          exit(2); 
        }

        // Open our window
        init_curses();
        // fill_curses();

        // If no player_name, spectate
        if (argc == 3) {
          spectate(serverHost, serverPort, server);
          flog_v(log_file, "User has chosen to spectate\n");
        }
        
        // Otherwise, play
        else if (argc == 4) {
          char* player_name = argv[3];
          play(serverHost, serverPort, player_name, server);
          flog_v(log_file, "User has chosen to play\n");
        }

        // If too many args
        else {fprintf(stderr, "Usage: ./client hostname port [playername]\n"); exit(1);}
    }

    // If too few args
    else {fprintf(stderr, "Usage: ./client hostname port [playername]\n"); exit(1);}

    // If all goes well
    message_done(); // Close messaging communication
    exit(0); // Exit with no errors
}

/* Spectate function to tell server to setup client as a spectator, doesn't allow game interaction
@inputs - servername, port (1-26), IP of server
@outputs - messages recieved from server
*/
void spectate(char* serverHost, char* serverPort, addr_t serverAddress) {
  spectator = true;
  // Tell the server we're spectating
  char* message = "SPECTATE";
  message_send(serverAddress, message);
  flog_v(log_file, "Spectating initiated succesfully\n");
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
    message_send(serverAddress, message);
    flog_v(log_file, "Playing initiated succesfully\n");

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
  int ch = wgetch(inputwin);
  // read a line from stdin
  if (ch == '\0') {
    // EOF case: stop looping
    char* quitMessage = "KEY Q";
    message_send(*serverp, quitMessage);
    return true;

  } else {
    // strip trailing newline

    char key[6] = "KEY ";
    size_t key_len = strlen(key);
    key[key_len] = ch;
    key[key_len + 1] = '\0';
    
    // Log and send the key if it is valid
    if (is_key(ch)) {
      char log_message[20];
      snprintf(log_message, sizeof(log_message), "User pressed: %c\n", ch);
      // Assuming flog_v is a logging function
      flog_v(log_file, log_message);
      message_send(*serverp, key);
    }
    

    // normal case: keep looping
    return false;
  }
}

int is_key(char input) {
  char c = tolower(input);
  if(spectator){
    return input == 'Q';
  }
  return c == 'k' || c == 'h' || c == 'j' || c == 'l' || c == 'y' || c == 'u' || c == 'b' || c == 'n' || input == 'Q';
}

/**************** handleMessage ****************/
/* Datagram received; print it.
 * We ignore 'arg' here.
 * Return true if any fatal error.
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  
  // Check if all is good on server side
  if (strncmp(message, "OK", 2) == 0) {
    if (sscanf(message, "OK %c", &real_name) == 1) {
    } else {
      char errorMessage[256];
      snprintf(errorMessage, sizeof(errorMessage), "Failed to parse OK message: %s.\n", message);
      flog_e(log_file, errorMessage);
    }
  }

  // Get the GRID object from server to display map and check screen dimensions
  else if (strncmp(message, "GRID", 4) == 0){
    int gridrows = 0; 
    int gridcolumns = 0;
    if (sscanf(message, "GRID %d %d", &gridrows, &gridcolumns) == 2) {
    } else {
      char errorMessage[256];
      snprintf(errorMessage, sizeof(errorMessage), "Failed to parse GRID message: %s.\n", message);
      flog_e(log_file, errorMessage);
    }
    
    // Get grid window of right size, loop until.
    while (NR + 1 < gridrows || NC + 1 < gridcolumns) {
      
      clear();
      
      // Tell the player & log message to resize
      mvprintw(0, 0, "Waiting for player to resize their grid");
      flog_v(log_file, "Waiting for player to resize their grid");
      
      // Update Window
      refresh();

      // Check if resizing occured
        int ch = getch();
        if (ch == KEY_RESIZE) {getmaxyx(stdscr, NR, NC);}
      }
  }

  // Get GOLD information from client
  else if (strncmp(message, "GOLD", 4) == 0){
    if(spectator){
      if (sscanf(message, "GOLD %d", &r) == 1) {
      } else {
        char errorMessage[256];
        snprintf(errorMessage, sizeof(errorMessage), "Failed to parse GOLD message for spectator: %s.\n", message);
        flog_e(log_file, errorMessage);
      }
    }

    // Defense
    else{
      if (sscanf(message, "GOLD %d %d %d", &n, &p, &r) == 3) {
      } else {
        char errorMessage[256];
        snprintf(errorMessage, sizeof(errorMessage), "Failed to parse GOLD message for player: %s.\n", message);
        flog_e(log_file, errorMessage);
      }
    }
  }

  // Information for CURSES
  else if (strncmp(message, "DISPLAY", 7) == 0){
    
    // HEADERS
    if(spectator){
      mvprintw(0, 1, "Spectator: %d nuggets unclaimed", r); 
      refresh();
    }
    
    else{
      mvprintw(0, 1, "Player %c has %d nuggets (%d nuggets unclaimed)", real_name, p, r);
      refresh();
    }

    // MAP
    mvprintw(1, NC/2, "%s", message + 8); 
    refresh();
  }
  // Determine QUIT and print reason for quiiting to stdout + logfile
  else if (strncmp(message, "QUIT", 2) == 0){
    endwin(); //close window
    char reason[1048];
    int offset;
    if (sscanf(message, "QUIT %n", &offset) == 0) {
      // Copy the remainder of the message into reason
      strncpy(reason, message + offset, sizeof(reason) - 1);
      reason[sizeof(reason) - 1] = '\0'; // Ensure null-termination
      fprintf(stdout, "%s\n", reason);
    } else {
      char errorMessage[256];
      snprintf(errorMessage, sizeof(errorMessage), "Failed to parse QUIT message: %s.\n", message);
      flog_e(log_file, errorMessage);
    }
    fflush(stdout);

    // Shut it all down
    flog_v(log_file, "Exiting program sucessfully");
    flog_done(log_file);
    fclose(log_file);
    exit(0);
  }

  // If errors print them
  else if (strncmp(message, "ERROR", 2) == 0){
    char reason[200];
    memset(reason, 0, sizeof(reason));
    if (sscanf(message, "ERROR %180[^\n]", reason) == 1) {
      fprintf(stdout, "%s\n", reason);
    } else {
      char errorMessage[256];
      snprintf(errorMessage, sizeof(errorMessage), "Failed to parse ERROR message for spectator: %s.\n", message);
      flog_e(log_file, errorMessage);
    }
  }
  else{
    char errorMessage[256];
    snprintf(errorMessage, sizeof(errorMessage), "Logged '%s.\n", message);
    flog_e(log_file, errorMessage);
  }
  fflush(stdout);
  return false;
}



/* Function to start up curses window for game display
@inputs - NONE
@outputs - Displays curses window */
static void init_curses(void) {
  initscr();            // Start curses mode
  noecho();             // Don't echo while we do getch
  cbreak();             // Disable line buffering
  keypad(stdscr, TRUE); // Enable special keys capturing (like KEY_RESIZE)
    // Create the input window
  int starty = 1;       // Position: one line below the top of the screen
  int startx = 0;       // Start at the beginning of the line
  int width = COLS;     // Width spans the entire screen width
  inputwin = newwin(1, width, starty, startx);
  wrefresh(inputwin);
    // Set our screen size
  getmaxyx(stdscr, NR, NC);

    // Visuals
  curs_set(0); // Don't show user cursor

    // Colors
  start_color();
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK); // Color pair for background/text
  attron(COLOR_PAIR(2));
}