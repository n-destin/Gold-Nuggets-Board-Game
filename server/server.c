#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <messaga.h>
#include <time.h>

#define MAX_PLAYERS 26
#define MAX_SPECTATORS 1

int GoldMinNumPiles = 5;
int GoldMaxNumPiles = 10;
int MaxPlayers = 26;
int MaxNameLength = 50;

void initialize_game();
void handle_client_messages(int sockfd);
void load_map(const char* map_file);
void send_summary_and_quit(int sockfd);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <mapfile> [seed]\n", argv[0]);
        exit(1);
    }

    long seed = (argc == 3) ? atol(argv[2]) : getpid();
    if (seed <= 0 && argc == 3) {
        fprintf(stderr, "Seed must be a positive integer\n");
        exit(1);
    }

    srand(seed);
    load_map(argv[1]);

    int sockfd, port;
    setup_network(&sockfd, &port);
    printf("Server started on port %d\n", port);

    initialize_game();
    handle_client_messages(sockfd);

    send_summary_and_quit(sockfd);
    close(sockfd);
    return 0;
}

void load_map(const char* map_file) {
    FILE* file = fopen(map_file, "r");
    if (!file) {
        perror("Failed to open map file");
        exit(1);
    }
    //////
    fclose(file);
}


void initialize_game() {
    // Initialize your game state, e.g., placing gold randomly
    printf("Game initialized with random gold placement\n");
}

void handle_client_messages(int sockfd) {
    // Handle incoming messages from clients
    printf("Handling client messages\n");
}

void send_summary_and_quit(int sockfd) {
    // Prepare summary, send QUIT messages, print summary
    printf("Game over, sending summary and quitting\n");
}
