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

/* Spectate function to tell server to setup client as a spectator, doesn't allow game interaction
@inputs - servername, port (1-26), IP of server
@outputs - messages recieved from server */
void spectate(char* serverHost, char* serverPort, addr_t serverAddress);

/* Play function to tell server to setup client as a player, allows game interaction
@inputs - servername, port (1-26), player name, IP of server
@outputs - messages recieved from server */
void play(char* hostname, char* port, char* player_name, addr_t serverAddress);