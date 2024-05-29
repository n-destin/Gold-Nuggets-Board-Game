# Nuggets

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.


## Group members

Izzy Axinn - Map
Sam Akire - Visibility
Destin Niyomufasha - Server
Sebastian Frazier - Client

## Directories

### Map
Contains all functions to intialize a map, move players, and set their visibility

### Visibility
Contains all the functions to set the visibility rules for a players map and update that map accordingly

### Client
Contains all the functions for a client to connect to a server and play nuggets

### Server
Contains all logic of the nuggets game and communicates with the clients individually


## How to run

Run 'make' in the main directory 
'CD' into server and run 'make test' (you can change the map in the makefile) or 'make deadend' if you want to test our personalized map. 
Then 'CD' into client and connect with to the server with the port adress provided by server. 
Run `./client hostname port [playername]` to connect as a player or
Run `./client hostname port` to connect as a spectator 


## Notes

Valgrind runs clean on server and map, but client has curses errors which are accpetable and can't be fixed. 