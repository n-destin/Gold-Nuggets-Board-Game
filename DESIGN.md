# CS50 Nuggets
## Design Spec
### Social Security, 24S, 2024

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.


Team Structure
Izzy Axinn - Map
Sam Akire - Visibility
Sebastian Frazier - Client
Destin Niyomufasha - Server

Player

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

A new player drops into randomly selected empty room with 0 nuggets
Visibility will be updated automatically after each move 
A player can move in the the direction defined by 





User Interface
> You may not need much more.
See the requirements spec for both the command-line and interactive UI.
Inputs and outputs
Client
> The user will be given a display with the top line showing current game status. The rest of the display lines will load the game itself (map, players, gold, etc.)

> Users interact with the game through keystrokes

> Briefly describe the inputs (keystrokes) and outputs (display).
> If you write to log files, or log to stderr, describe that here.
Client
> Launched from command line with args: playername (name of new player), port# (which port the client is linking too), and hostlocation (server address).

> Keystrokes:
> q - quit if possible
> h - move left if possible
> l - move right if possible
> j - move down if possible
> k - move up if possible
> y - move diagonally up and left if possible
> u - move diagonally up and right if possible
> b - move diagonally down and left if possible
> n - move diagonally down and right if possible
Log
> Server sends messages to clients to explain current client operations in real time
> Information such as errors, exits, and some gameplay functions are logged on a visible screen for client, log file saved at termination.

Server


Client
> Displays critical server messages
> Saves all server messages to local file

Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

Visibility
Create an empty visible struct
Create an empty gold struct
Each time the player moves to a new block
	If the player is currently on gold
		Add the coordinates to the list of gold that is currently taken in visible
		Add a normal character  to visible so it appears where the gold used to be
Create an empty gold struct
Loop through lines from 0 to 359 degrees from the players current point
	Given a line, follow its path and add elements until it intersects a wall
		If it is a piece of gold and it isn’t on the list inside visible
			Add the gold to the gold struct to be viewed
		Else add it to the visible struct
Add the wall to the visible struct
Delete all the gold that used to be visible 
Print out all of the gold
Print out all of the visible elements that haven’t been printed yet
Free the gold structure since we don’t track gold at new positions
Free the visible struct once the game is done

Map
	responds to initializeGame()
read in name of map file
	read through map file, ensuring it is valid and determining ‘rows’ and ‘columns’ 
	once validated:
		Allocate memory for the ‘grid’ 
		Go through each square and create a new ‘spot’ 
		Add spot to ‘grid’
	return grid
Server
	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up
Client
	Execute from command line
	Parse the command line and validate arguments, handle erroneous args
	Initialize player module for new client
	If playing as player, send player message to server to indicate new player
	If playing as spectator, send spectator message to server to indicate new spectator.
	Send messages to server as play occurs to handle user inputs from the client-side
	Free memory and exit

Major data structures
> This description should be independent of the programming language.
> Mention, but do not describe, data structures implemented by other modules (such as the new modules you detail below, or any libcs50 data structures you plan to use).
Map 
The map is a data structure called ‘grid’
‘grid’ -> a 1 dimensional array of ‘spots’ and will contain a number of ‘ rows’  and  ‘columns’ 
‘spots’ -> the unit of grid and can contain various data structures that could be in the map
wall
corner
player
gold
hole 
passage
walking area

Visible
The two major data structures for this are visible and gold.

Visible keeps track of all previous non-gold characters that have been visible to the player along with the gold that the player has picked up. Whenever a new part of the map is in view, it is added to visible and printed out onto the map. For gold, once the player picks up gold we keep track of it so we don’t display it to the player anymore.

Gold keeps track of all gold that is currently visible. Since we don’t keep track of the locations of the gold throughout the game, we free this struct each time the character moves. Additionally, when the player picks up a piece of gold, it is added

Client
Player data structure for tracking player movement and input on the client-side. Should help in handling input and sending it to the server. Initialized upon new client connection

Server 

Functional Decomposition
Visibility
We will have a visible module which is used to store everything that has been previously seen aside from gold. It will have the following methods:

visible_new() - Creates a new visible struct
visible_add() - Adds a character along with its coordinates to the struct
visible_print() - prints the new characters out for the user to see
visible_taken()- Keeps track of gold already taken
visible_delete() - frees all memory associated with this module

We will also have a gold module that stores the gold that can currently be seen and erases it when out of view. It will have the following methods:

	gold_new() - Creates a new gold module
	gold_add() - Add new piece of gold that is seen
	gold_print() - prints out all of the gold
	gold_delete()- frees all memory with this module
	

Server


Map
We will have a map Module
	initialize_map(File* map) - initializes map
	validate_map() - called by initialize_map() and validates that map is correct 
load_map() - called by initialize_map() and loads data into ‘grid’ module 
print_spots() - calls spot_iterate() on grid 

We will have a grid Module which contains a struct of ‘spots’
	Initialize_grid() - initializes grid
	add_spot(char* spot) - adds a new spot
	delete_spot() -deletes a spot
	spot_iterate() - iterates through spot
	
	


Client
We will have a player module which contains a struct of ‘player_t’
player_new() - creates a new player upon client connection
player_loadmap() - loads the map onto the client for the player
player_delete() - deletes a player
player_move - handles input to move the player around the map



