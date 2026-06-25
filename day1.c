#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define ROWS 20
#define COLS 40
#define EMPTY ' '
#define WALL '#'
#define TREASURE 'T'
#define HEALTH_PACK 'H'
#define KEY 'K'
#define LOCKED_DOOR 'D'
#define MAX_PLAYERS 3

char map[ROWS][COLS];

void initializeMap();
void placeWalls();
void placeTreasures();
void readKeyboard();
void move(int new_x, int new_y);

int main() {
    initializeMap();
    placeWalls();
    placeTreasures();
}

void initializeMap() {
    // Zero-fill arrays (set all cells to EMPTY)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            
        }
    }

   /////removed here  // Draw border walls
   // for (int i = 0; i < ROWS; i++) {
     //   for (int j = 0; j < COLS; j++) {
       //     if (i == 0 || i == ROWS - 1 || j == 0 || j == COLS - 1) {
         //       map[i][j] = BORDER;
         //   }
     //   }
    //}

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

void readKeyboard(){
     char ch;
    scanf(" %c", &ch);  // Note the space before %c to skip whitespace
    ch = tolower(ch); 

    switch (ch){
        case 'w' : move (0, -1); break;
        case 's' : move (0,  1); break;
        case 'a' : move (-1, 0); break;
        case 'd' : move (1,  0); break;
    }
}

void move(int new_x, int new_y){

}