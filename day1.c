#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 40
#define EMPTY ' '
#define WALL '#'
#define TREASURE '$'
#define BORDER '*'

char map[ROWS][COLS];

// Function prototypes
void initializeMap();
void placeWalls();
void placeTreasures();

int main() {
    srand(time(NULL));
    initializeMap();
}

void initializeMap() {
    // Zero-fill arrays (set all cells to EMPTY)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            map[i][j] = EMPTY;
        }
    }

    // Draw border walls
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == 0 || i == ROWS - 1 || j == 0 || j == COLS - 1) {
                map[i][j] = BORDER;
            }
        }
    }

    // Call all place* helpers
    placeWalls();
    placeTreasures();
}

void placeWalls() {
    int placed = 0;
    while (placed < 30) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        // Place only on empty cells (not on border or already occupied)
        if (map[row][col] == EMPTY) {
            map[row][col] = WALL;
            placed++;
        }
    }
}

void placeTreasures() {
    int placed = 0;
    while (placed < 12) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        // Place only on empty cells (not on border, walls, or other treasures)
        if (map[row][col] == EMPTY) {
            map[row][col] = TREASURE;
            placed++;
        }
    }
}