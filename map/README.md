# Map 
## CS50 Nuggets
## May 28th, 2024

This directory contains information for the initiating and updating the map used for the nuggets game. For detailed information about each of these modules, look at the corresponding header file for in-depth analysis of each method.

### map.c
A map is a holds the information for the nuggest game including where objects are, where people are, and the various properties of these tings

### spot.c 
A spot is a place a place on a map which can hold a person and object. A spot may be visible or not visible, though the person on it is always visible

### person.c
A person is an object held within a map and corresponds to a player in the game nuggets. A person has an amount of gold, as well as a name and position on a map.

### maptest.c
A testing program which samples how to use the map directory 

### visibility.c
This program contains only one method called visible, which when called on a player and its map sets the spots that should be visible for that given player on the map. This method and its documentation can be found in `visibility.h` file.

I will note that the visibility program relies on many static methods that are thus not included in the header file. However, detailed descriptions can be found above these methods within `visibility.c` and more so within the methods themselves.
Collapse












