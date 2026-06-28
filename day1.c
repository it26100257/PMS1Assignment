#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define ROWS 15
#define COLS 15
#define WALLS 30
#define WALL_SYMBOL '#'
#define TREASURES 12
#define TREASURE_SCORE 10
#define TREASURE_SYMBOL 'T'
#define HEALTH_PACKS 5
#define HEALTH_RESTORE 20
#define HEALTH_SYMBOL 'H'
#define KEYS 3
#define KEY_SYMBOL 'K'
#define DOORS 3
#define DOOR_SYMBOL 'D'
#define TRAPS 10
#define TRAP_DAMAGE 20
#define EMPTY_SYMBOL ' '
#define MAX_PLAYERS 3
#define STARTING_HP 100
#define MAX_HP 100
#define MAX_MOVES 4
#define MAX_ENTRIES 100
#define MSG_LENGTH 256
#define ENTRIES_MADE 5
#define SAVE_FILE "savegame.dat"
#define LOG_FILE "gamelog.txt"

int  cellIsEmpty(int r, int c);
void initializeMap();
void placeWalls();
void placeTreasures();
void placeTraps();
void placeHealthPacks();
void placeKeys();
void placeDoors();
void placePlayers();
void printMap();
void movePlayer(int index);
int  isValidMove(int x, int y);
void processTile(int index);
void gameLoop();
void saveGame();
void loadGame();
void showScores();
int  remainingTReasures();
void addLog(int r, char *m);
void printRecentLog();
void saveLog();
void showStats();

int main() {
	char map[ROWS][COLS], eventLog[MAX_ENTRIES][MSG_LENGTH], name[40], symbol;
	int x, y, score, health, keys, movesMade, treasuresFound,trapsTriggered;
	int damageTaken, healthPacksUsed, keysCollected, doorsUnlocked;
	int playerCount = 0;
	int hiddenTrap[ROWS][COLS];

}

//check wheter cell is empty
int  cellIsEmpty(int r, int c){
	int map[r][c], hiddenTrap[r][c];
	return (map[r][c] == EMPTY_SYMBOL && hiddenTrap[r][c] == 0);
}
void initializeMap() {
    int r, c; //rows and columns 
    int map[r][c], hiddenTrap[r][c];
    srand((unsigned int)time(NULL)); //rand num gen to get new map every game.
    
    memset(map,        EMPTY_SYMBOL, sizeof(map)); 
    memset(hiddenTrap, 0,            sizeof(hiddenTrap));

    for (c=0; c< COLS; c++){
	    map[0][c] = WALL_SYMBOL; //top border 
	    map[ROWS - 1][c] = WALL_SYMBOL; //bottom
    }
    for (r=0; r< ROWS; r++){
            map[0][r] = WALL_SYMBOL; //left
	    map[COLS - 1][r] = WALL_SYMBOL; //right
    }
    // Calling all place helpers
}

