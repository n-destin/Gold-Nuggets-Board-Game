# CS50 Nuggets
## Design Spec
### Social Security, 24S, 2024

The Nuggets game requires two standalone programs: a client and a server.
Our design also includes map and visibility modules that handle various operations within the client and server.

We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec. We avoid repeating information that is provided in the requirements spec.\


## Team Structure
Izzy Axinn - Map
Sam Akire - Visibility
Sebastian Frazier - Client
Destin Niyomufasha - Server

## Client

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

A new player drops into randomly selected empty room with 0 nuggets
Visibility will be updated automatically after each move
A player can move in the the direction defined by the keystroke they press.
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

The client uses the CURSES library to open a display window and print the map. We also ensure the window is of the right size by checking if it is big enough at least to fit the grid.

### Inputs and Outputs

> The client is run through the command line with either 3 or 4 arguments. It is in the order: IP_addr PortNum [Playername]. If no player name is provided we join as a spectator\
> The client outputs running messages to Stdout and the CURSES window, it will exit with error code 0 if all goes well and nonzero otherwise.
> All messages from the server are also saved in a logfile.

## Server

The server module handles the running of the game on the backend. We start a game and establish communication with the client to read in inputs and send back messages that the client will interpret and show to the player in desired form. 

> The server is responsible for establishing a network for communication with the client
> The server should also provide a port to the client to join the nuggets game
> The server handles invalid inputs and broadcasts valid messages to the client using a call/response system
> When the game ends for any reason, we send a QUIT message along with a reason for quitting to stdout on the client side.
> We use the message_loop() function to establish a connection.

Messages include
> QUIT
> PLAY
> SPECTATE
> DISPLAY
> GOLD
> KEY _

### Inputs and Outputs
> In: A valid map to make the game with
> Out: Messages to the client detailed above

## Map
	
The map module handles the information for the nuggets game including data about where players and gold are, the map (seed) itself, and the map dimensions.

> map.c will create and validate a new map, loading it in from the provided maps folder. Once the map is loaded, we can pseudo-randomly scatter gold across it to create a unique 'game' and store the locations of the gold. Then, we track players being inserted into the map at pseudo-random locations and enable their movement. 
> The map handles keypresses from players and defines directional movement

map.c also provides functionality for printing/displaying the map, deleting it from memory, and 'cloning' it. This handles important server/client functionality by providing a base for gameplay necessary for both programs.

## Visibility

The visibility module handles the 'vision' component of the Nuggets game. We want to make it such that a player can only view a portion of the map if its in their line of sight. Once a part of the map has been seen, it will be permanently visible though gold/players will only be visible if it is currently in ones line of sight.

Basic Pseudo Code for the entire visibility process:
  We loop through all of the points on the grid:
 >      If the point is not a blank space than we check if it is visible by:
 >          Calculate the slope from the point to the player
 >          If the slope is between -1 and 1:
 >              If the player is further right
 >                  Draw a shallow line from the spot to the player
 >                  Return false if it intersects anything, and true otherwise
 >              else
 >                  Draw a shallow line from the player to spot
 >                  Return false if it intersects anything, and true otherwise
 >          If the abs(slope) is greater than 1:
 >              If the player is below the spot
 >                  Draw a steep line from the spot to the player
 >                  Return false if it intersects anything, and true otherwise
 >              else
 >                  Draw a steep line from the player to spot
 >                  Return false if it intersects anything, and true otherwise
 >      If the spot is visible
 >         Make that spot visible
 >      if the spot is gold and also not visible
 >          Make the gold no longer visible


## Structs
The map is a data structure called 'grid'
'grid' -> a 1 dimensional array of 'spots' and will contain a number of rows  and columns
'spots' -> the unit of grid and can contain various data structures that could be in the map

'person' -> will keep track of players location, visibility, and their held gold

'game' -> tracks all game data inside server including map, players, spectators, and gold

'gold' -> keeps track of all gold that is currently visible. Since we don't keep track of the locations of the gold throughout the game, we free this struct each time the character moves. Additionally, when the player picks up a piece of gold, it is added

'client' -> Player data structure for tracking player movement and input on the client-side. Should help in handling input and sending it to the server. Initialized upon new client connection

'map' -> contains a GRID, as well as players and stores a num of rows and columns

wall
corner
player
gold
hole 
passage
walking area
window
spectator