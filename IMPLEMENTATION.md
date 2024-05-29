# CS50 Nuggets
## Implementation Spec
### Social-Security, 24S

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes a map directory, which contains map, person, spot, and visibility modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

### Samuel

Responsible for the visibility module. This includes checking which items are currently visible from the players position, what has previously been seen/if it should be visible(gold shouldn't be), and all of the math behind this program. He will also do the implementation spec, header files, and other descriptions of our program as needed. 

### Izzy

Izzy will handle all of the map/grid work. This includes the map, person, and the spots(each part of the grid). He will work on this early, so that the rest of us can do our parts later(which rely quite heavily on having a functioning map).

### Sebastian

Sebastian will work on the client part of the project. This is described in detail in the requirements, but the client basically handles user interaction with the game and relies on the server.

### Destin

Destin will work on the server part of the project which is also described in the requirements spec. This will send messages to the client and be run to create a port for the users to use.


# Modules

## Map

### Data Structure

```c
typedef struct map{
    int rows;
    int columns;
    spot_t** grid;
    person_t** players; 
} map_t;
```
The map struct will contain several values. Firstly, the number of rows and columns in the map. An array of spots which will make up the grid that the map is represented by. Finally, we will have an array of persons who are the different players in the gam

### Functions

Creates a new map given a path to the text file that represents it
```c
map_t* map_new(char* path);
```

#### `map_new`:
	Check to make sure that the provided file exists and is valid
	Call a static map validate method to check that the map file provided meets the specs and can be loaded in
	Create a grid the correct size to store all of the elements of the map
	Loop through all elements in the maps text file:
		Create a given spot for the element
		Add this spot to the grid
---

Used to print out the map in its current state to a given output file:
```c
void map_print(map_t* map, FILE* out);
```

#### `map_print`:
	Loop through the rows
		Loop through the columns
			If the current position is null
				Break loop since we reached the end of the line
			Else If we are currently on a player
				Print the player in that position
			Else
				Get the current spot that we are on
				If the spot is visible
					Print out that spot
		Print out a newline character
---

Deletes the map and frees all memory associated with it:

```c
void map_delete(map_t* map);
```

#### `map_delete`:
	Loop through the grid
		If it is a spot
			Delete that spot
		If it is a player
			Delete the player
	Free the grid of spots
	Free the grid of players
---

Used to move a person within a map in a specified direction.
Returns true if the person moves as intended, false if unable to move
```c
bool move_person(map_t* map, person_t* person, char direction);
```
#### `move_person`:
	Find the persons current position within the grid
	Check which direction the person should move depending on the character
	Calculate where in the grid the player should move
		If moving vertically/diagonally, we need to divide by the number of columns to find the right spot
	Check if there is a player in the way
		Handle as needed
	Check if they are moving to a valid spot
		Move the player to that spot
		Check and add for gold as needed
	Otherwise if moving to an invalid spot
		Return false
	Return true
---

Used to insert a person into the map at a random location
```c
person_t* insert_person(map_t* map, char c, char* name, addr_t address);
```
#### `insert_person`:
	Calculate a random spot within the map
	Insert the player at this random spot within the map
---

Converts the grid into a string for what a spectator should see
This is the entire map, since a spectator should be able to see everything
```c
char* grid_to_string_spectator(map_t* map);
```
#### `grid_to-string_spectator`:
	Create a string for the length of the grid
	Loop through all the elements in the grid
		Append the elements to the string
	Return the string
---

Converts the grid into a string for what the player sees
Uses visible to keep track of what should/should not be displayed
 ```c
char* grid_to_string_player(map_t* map, char letter);
```

#### `grid_to_string_player`:

	Loop through all of the elements in the grid
	If the spot is visible
		Check if the spot is gold that is visible but no longer in range
			If so, append a dot to the string in place of it
		If not 
			Append the spot to the string
	Else
		Append a blank space
	Return the string
---

Initializes the gold into the map:
```c
void gold_initialize(map_t* map);
```

#### `gold_initialize`:
	Calculate the number of piles of gold that we want to make randomly
	Get the remaining amount of gold
	Randomly distribute the gold among the piles
	Randomly add the gold spots throughout the grid as described by the specs
---


### Getter/Setter Functions for Map:

The following methods are getters/setters so won't require pseduo code as they simply return/set the value:


Returns the array of people on the given map.
```c
person_t** get_players(map_t* map);
```
---

Return the number of rows in the map
```c
int get_rows(map_t * map);
```
---

Return the number of columns in the mao
```c
int get_columns(map_t* map);
```
---

Return the grid associated with the map
```c
spot_t** get_grid(map_t* map);
```
---

## Person

### Data structure

```c
typedef struct person{
    char letter;
    int pos;
    int gold;
    int* visible;
    char name[MaxNameLength];
    addr_t address;
} person_t;
```
The person struct will contain the letter that represents them on the screen, an int which is their position on the map, another int which is how much gold that person has, an int-array of which objects are visible to them, a name, and finally their address.


### Functions

Used to create a new person.
The caller is responsible for calling person_delete

```c
person_t* person_new(char c, char* name, addr_t address, int max_positions);
```
#### `person_new`:

	Allocates memory for size of a person struct, check if allocated correctly.
	Allocate memory for visible array and set all positions to zero
	Set the persons gold value to zero
	Set the persons letter and adress to the value inputted.
---

Used to delete a person and free all memory associated with them
```c
void person_delete(person_t* person);
```
#### `person_delete`:
	Free the memory associated with all the elements the person holds
	Free the memory associated with the person
---

#### Getter/Setter Functions for Person:

Since the following methods are basic getter/setter methods, no pseudo code is provided as they will be max one to two lines.

Used to add a specified amount of gold to a persons total count
```c
void person_addGold(person_t* person, int gold);
```
---

Returns a person's gold count

```c
int person_getGold(person_t* person);
```
---

Returns the letter used to represent a person
```c
char person_getLetter(person_t* person);
```
---

Returns a person's name
```c
char* person_getName(person_t* person);
```
---

Set a persons position within the grid to a specified index
```c
void person_setPos(person_t* person, int new_pos);
```
---

Returns the position of a person
```c
int person_getPos(person_t* person);
```

Returns the adress of a person
```c
addr_t person_getAddr(person_t* person);
```
---

Set a persons name to a specified character
```c
void person_setName(person_t* person, char c);
```
---

Returns the array of what is visible/not visible for a given person
Within the array, zero means it is visible and otherwise it should be visible to that player
```c
int* get_visible(person_t* person);
```
---

Updates what is visible and not visible to the player
```c
void update_visible_array(person_t* person, int index);
```

## Spot

### Data Structures

```c
typedef struct spot{ 
    char item;
    char person; 
    bool visible;
    bool invisibleGold;
    int gold;
} spot_t;
```
---
The spot struct is used to store the information for each spot on the map for the game. It will store the item of the spot, the potential person on the spot, whether the spot is visible, whether the spot has gold that is no longer visible, and how much gold the spot contains.

### Functions

Allocates memory for and creates a new spot
Caller is responsible for calling spot_delete
```c
spot_t* spot_new();
```
#### `spot_new`:
	Allocate memory for spot struct and check that it is not NULL
	Initialize all the values within the spot
---

Used to delete the spot and free all memory associated with it

```c
void spot_delete(spot_t* spot);
```

#### `spot_delete`:
	Simply free the spot
---

### Getter/Setter Methods for Spot

Sets item of spot to char 'c'
```c
bool spot_insert(spot_t* spot, char c);
```
---

Returns the item held by the spot
```c
char spot_item(spot_t* spot);
```
---

Get the current visibility of the spot
```c
bool get_visibility(spot_t* spot);
```
---

Set the visibility of the spot to the entered boolean
```c
void set_visibility(spot_t* spot, bool visibility);
```
---

Returns the number of gold at the spot
```c
int spot_get_gold(spot_t * spot);
```
---

Adds a certain amount of gold to the spot
```c
void spot_add_gold(spot_t* spot, int gold);
```
---

Set the item of a given spot
```c
void spot_set_item(spot_t* spot, char item);
```
---

If gold has been seen, wasn't picked up, and now should be invisible, we set the 
gold to be invisible until seen by the player again
```c
void set_invisible_gold(spot_t* spot, bool set);
```
---

Returns if a given spot contains gold that isn't visible
```c
bool spot_invisible_gold(spot_t* spot);
```
---

## Visibility

No structs are created for the this module.

### Functions

This method is called on a person after each move and given the person and map as parameters
It will go through the entire map and set every point as visible/invisible as needed
```c
void visibility(person_t* person, map_t* map)
```
Below is the pseudo code/logic for the entire visibility program. It requires static methods that aren't listed but the overall logic of the module will follow as described below.

#### `visibility`:
	Basic Pseudo Code for the entire visibility process:
	We loop through all of the points on the grid:
		If the point is not a blank space than we check if it is visible by:
			Calculate the slope from the point to the player
			If the slope is between -1 and 1:
				If the player is further right
					Draw a shallow line from the spot to the player
					Return false if it intersects anything, and true otherwise
				else
					Draw a shallow line from the player to spot
					Return false if it intersects anything, and true otherwise
			If the abs(slope) is greater than 1:
				If the player is below the spot
					Draw a steep line from the spot to the player
					Return false if it intersects anything, and true otherwise
				else
					Draw a steep line from the player to spot
					Return false if it intersects anything, and true otherwise
		If the spot is visible
					Make that spot visible
		if the spot is gold and also not visible
			Make the gold no longer visible
---


## Client

### Data Structures

winsize
```c
struct winsize w
```
This is used to store the size of the window, which is very important as the player adjusts the window size to work for how big the map is/how big they want it to be.

### Functions

The main function is used to run the client and is described below.
```c
int main(const int argc, char* argv[])
```

#### `main`:

	MODES:
		Spectate if 2 arguments passed
		Play if 3 arguments passed
	EXITS:
		0 if successful
		1 if invalid number of arguments
		2 if host/port is invalid
		3 if messaging initialization fails
		4 if message transmition module fails
---

Used if the person chooses to play the game.
Play function to tell server to setup client as a player, allows game interaction
@inputs - servername, port (1-26), player name, IP of server
@outputs - messages recieved from server
```c
void play(char* hostname, char* port, char* player_name, addr_t serverAddress)
```
#### `play`:
	Tell the server that we are playing
	Check that destination buffer is large enough
		If not exit non-zero
	Send the message to the server
	Check that the message was succesful
	Continue to loop through user input using the handleInput static method as needed
	End the message once the player quits
---

Spectate function to tell server to setup client as a spectator, doesn't allow game interaction
Spectators can watch the game unfold and see total remaining gold count
in - servername, port (1-26), IP of server
out - messages recieved from server into curses window
```c
void spectate(char* serverHost, char* serverPort, addr_t serverAddress)
```
#### `spectate`:
	Tell the server that we are spectating the game
	While the spectator is still there
		Display the current state of the game to the spectator
		Make sure that everything is visible for this person
	Clear the window
	Handle ending the message
---

### Static Functions for Client
We map out some static functions for this section since they will be important for this part of the program.

Used to handle the users input, called within play and the spectator methods.
Return true if there is a fatal error.
```c
static bool handleInput(void* arg)
```
#### `handleInput`:
stdin has input ready; read a line and send it to the client.
	Return true if the message loop should exit, otherwise false.
	i.e., return true if EOF was encountered on stdin, or fatal error. 
	inputs - Server address
	We strip trailing new lines ("\n"), then check if a key is a valid input. If it is send it to the server.
	We log all keystrokes/inputs

---

Used to handle messages with the server.
Return true if there is a fatal error.
```c
 static bool handleMessage(void* arg, const addr_t from, const char* message)
```
#### `handleMessage`:
	Datagram received; print it. We ignore 'arg' here.
	Return true if any fatal error.
	Recieve messages from the server and check their validity/display them 
	First, we check if the window is of the right size, if it isn't wait for the user to resize is
 	Once window is of right size
		print the map to CURSES with the header describing our gold count
	We log all messages from the server
	If the game should end for every reason
		 we display why: q press, game over, error, etc.
---

Used to initialize the curses window that the game is played on.
```c
static void init_curses(void)
```
#### `init_curses`:	
	Initialize curses window and define proper settings
	No inputs or outputs, sets up CURSES window
	Hide user types, cursor.
	Remove line buffering
	Enable keystrokes
	Define window boundaries
	Get screen size so we can check if its large enough
	Setup colors
---

## Server 

### Data Structures

```c
typedef struct {
    map_t* map; // this is the base map
    int num_players;
    int num_spectators; // binary
    int total_gold;
    int remaining_gold;
    addr_t spectator_address;
} game_t;
```
The game struct will be used to store many of the important factors that go on throughout the game.
Firstly, the base map is important to store as each player will have their own version of the map based on visibility
We also store the number of players and the number of spectators that are participating the game.
For gold, we keep track of the total gold and also the amount that is remaining for the players to pick up.
Finally, we note the address of any spectator who is watching the game.

### Functions

Initializes the game with the map provided and sets initial game parameters.
@param file_pathname The path to the file containing the map layout.
```c
void initialize_game(char* file_pathname);
```
#### `initialize_game`:
	Set the player and spectator counts to zero
	Set the total and remaining gold to the total amount of gold
	Initialize the map by calling map_new
	Initalize the gold onto the mape
---

Continuously handles messages from clients connected to the server.
```c
void handle_client_messages();
```
#### `handle_client_messages`:
	Prints out when it is handling a message from the client into the log
	Message loops through all the messages
---

Sends a summary of the game to all players and quits the game.
@param game A pointer to the game structure containing game state information.
```c
void send_summary_and_quit(game_t* game);
```
#### `send_summary_and_quit`:
	Creates a string for the summary starting with game over
	Get the array of players involved with the game
	Loop through all of the spots in the grid
		If a player is on that spot, create a summary to print
		This will include their letter, score, and name
	If there are any spectators
		Let the spectators know the game is over
---

 Handles individual messages received from clients and updates the game state accordingly.
 @param arg A void pointer to the game state, used for passing multiple data types.
 @param from The address structure of the message sender.
 @param message The message content received from a client.
 @return Returns true if the message was handled successfully, otherwise false.
 ```c
bool handle_message(void* arg, const addr_t from, const char* message);
```
#### `handle_message`:
	Create a game struct and a person one for the corresponding player
	If we recieve a message to play the game
		If we have less than the max allowed amount of players
			Check that the players name is valid
			Send out the needed messages to start the game
		Otherwise
			We tell them that we are at the max players
	Else If it is a spectator
		Send the message that we are creating a new spectator
	Else if it is a key inputted by a player
		if it is Q
			Then we quit the game
		Else
			If the key is uppercase(sprinting)
				Keep calling move person in that direction until we hit a wall
				Call visibility on each individual movement to show the path
			Else
				Call move person on the direction of the entered key

---

Sets up the network communications necessary for the server to receive client messages.
```c
void setup_network();
```
#### `setup_network`:
	Generate a number for the port
	If the port number is zero
		Then we failed to make the port and exit non-zero
	Print out the port number for the user to use in the client
---

Helper function to check if a string contains non-space characters.
@param str The string to be checked.
@return Returns 1 if non-space characters are found, otherwise 0.
```c
static int contains_non_space(const char *str);
```

## Testing plan

### Unit testing for modules

#### Spot
	Spot will be tested first by doing a unittest within the spot.c file(creating a main method that is only used for this testing).This should be fairly easy and will just be done by creating spots and making sure that all of the methods work. Further testing will be done in the maptest file once we put all of the other parts together within the map directory.

#### Person
	Similarly to spot, we can test person within its own file using a unittest. This will be very straightforward and just make sure that all of the methods work as we intended them to. One important thing to test will be for memory leaks, as we want to catch and fix them early before the program gets too large. This will be done by using valgrind and creating/deleting people. This applies to all of the other modules with structs too, as delete methods are crucial.

#### Map
	Map will be tested by creating a maptest file. This executable program will take in a map, initalize it into our struct, and peform the different methods onto it. We can make sure that all maps are read in correctly by printing them out and making sure they are identical to the text files that we read them in from. This also is a great place for us to test the person and spot modules as they are part of the map and we may discover errors within them that we didn't see before.

#### Visibility
	Visibility will initially be tested within its own file. This will be done by placing the player onto a map, printing out the result, and making sure that the visiblity is correct for that one spot. We will start with smaller maps which are basic and work our way up to the more challenging/complex ones that will test many of the edge cases for visibility. Once we get visibility working for this basic case and the client is running, we will then test it with movement. This includes going through the pathways between rooms and hittting dead ends. Additionally, we need to test rules around seeing gold, once it is out of range, and also other players.

### Integration Testing

#### Testing the Client
	The client can be tested by making sure that all user-input is handled correctly, making sure that the window/curses works correctly especially when being dragged to make larger for bigger maps, and also that messages are recieved/sent correctly. For the keys and input that a user puts in, we can use the client.log file to make sure that the input recieived lines up with what we put into it. The window size can be tested by running the client for maps of varying sizes. Small ones shouldn't require the window to expand much, but the larger ones will require us to potentially expand the window to most of the screen. Additionally, even if the map is small we should make sure that the client is able to handle the user expanding the screen as we never know what they may choose to do. Finally, we can check the ouput messages as this will be crucial for the next step of making sure that the client interacts correctly with the server.

	One thing we ended up doing is creating a mini-client first which was used to test our integration with the server. This allowed development to occur around the same time, as the server coder didn't have to wait until the client was complete to start testing some cases. Eventually, we will transtion to the fully implemented client and make sure that they work together. As always, we also will want to test for incorrect arguments both given by the user and also recieved from the summer. This should be done by testing with an incorrect number of arguments, invalid arguments, and invalid messages recieved from the server.

#### Testing the Server
	The Server will be tested to make sure that it is able to run and generate a port for the client correctly, be able to manage seperate visibility fields for the different players, and make sure that it sends and correctly recieves the messages from the client. We will start by running a server and making sure that the ports generated allow one player to join and play the game. We can then transition into multiple players and making sure that the server is able to handle them. On the note of multiple players, making sure that the visibility of each player is unique will be a challenge and something we need to make sure is fully functional. We then will need to test to make sure that the program works with the addition of spectators who should be able to see the full map and all of the players. This will be done by adding different combinations of players and spectators to make sure that everything works. The tracking of gold is also an important aspect that needs to be tracked. We need to make sure that toal/remaining gold are displayed, updated, and correct most of all. We should also test with multiple players to make sure the gold can be distributed amongst them. Finally, we will need to test the quit function and make sure that the correct message is displayed to the users once the game ends. Particularly, that the game ends once all the gold has been picked up or if all of the players quit.

### System testing and Edge Cases
	Now it is finally time for us to test everything together and make sure that the whole project works! While we should know that there aren't issues with each individual module, we need to make sure that they work together, especially the client and server. We will begin by running the server and then creating a client, making sure that it connects and runs correctly. We can then transition into multiple users and test all of this on a variety of different maps. We already will have tested the interaction of the two through the use of the mini-client so this interaction shouldn't be too much of an issue.

	We also need to test for many different edge cases that are rare but could still occur. Many of these will come to mind as we program this game, but I will list some to begin. One notable is the player limit, making sure that once the max players is reached that no more are allowed to join. Additionally, just making sure the game runs smoothly for this many players is quite important. On the note of multiple players, we should test the interactions between them such as trying to be on the same spot, crossing in tunnels, and a variety of other factors. These edge cases will be tested along with many others we come up with along the way, such as invalid arguments or certain players quitting mid game.

	Finally, Valgrind will be used to make sure that no memeory leaks exist within the program. This shouldn't be too hard as we will have been using Valgrind for all of the modules, and combining them together shouldn't cause issues. It is vital although that we remember to call delete functions on any modules we create, such as deleting the map when a game ends.

---

## Limitations

We don't have any notable limitations.
