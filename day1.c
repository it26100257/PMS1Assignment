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

int  cellIsEmpty(int r, int c); //done
void initializeMap();           //done
void placeWalls();              //done
void placeTreasures();          //done
void placeTraps();              //done
void placeHealthPacks();        //done
void placeKeys();               //done
void placeDoors();              //done
void placePlayers();            //done
void printMap();                //done
void movePlayer(int index);                   //1
int  isValidMove(int x, int y);               //2
void processTile(int index);                  //3
void gameLoop();                              //4
void saveGame();                              //5
void loadGame();                              //6
void showScores();                            //7
int  remainingTreasures();                    //8
void addLog(int r, char *m);                  //9
void printRecentLog();                        //10
void saveLog();                               //11
void showStats();                             //12

char map[ROWS][COLS], eventLog[MAX_ENTRIES][MSG_LENGTH];
int  hiddenTrap[ROWS][COLS];
int playerCount = 0;
int roundCounter = 0;
int logCount = 0;

typedef struct{
      char name[40];
      int movesMade, treasuresFound, trapsTriggered, damageTaken, healthPacksUsed, keysCollected;
      int doorsUnlocked, health, score, keys, x, y, symbol;
      bool alive;    
}player;

player players[MAX_PLAYERS];

int main() {
   initializeMap();
}

//check whether cell is empty
int  cellIsEmpty(int r, int c){
	return (map[r][c] == EMPTY_SYMBOL && hiddenTrap[r][c] == 0);
}
void initializeMap() {
    int r, c; //rows and columns 
    for (int r=0; r<ROWS; r++){
	    for (int c=0; c<COLS; c++){
		    map[r][c] = EMPTY_SYMBOL;
		    hiddenTrap[r][c] = 0;
	}
    } 

    for (c=0; c< COLS; c++){
	    map[0][c] = WALL_SYMBOL; //top border 
	    map[ROWS - 1][c] = WALL_SYMBOL; //bottom
    }
    for (r=0; r< ROWS; r++){
            map[0][r] = WALL_SYMBOL; //left
	    map[COLS - 1][r] = WALL_SYMBOL; //right
    }
    // Calling all place helpers
    placeWalls();
    placeTreasures();
    placeTraps();
    placeHealthPacks();
    placeKeys();
    placeDoors();
    placePlayers();
}

void placeWalls(){
     int wallIsPlaced = 0;
     int r, c;

     while(wallIsPlaced<WALLS){
          r = rand() % ROWS;
          c = rand() % COLS;

	  if(r>0 && r< ROWS - 1 && c>0 && c< COLS-1 && cellIsEmpty(r,c)){
		  map[r][c] = WALL_SYMBOL;
		  wallIsPlaced++;
        }	  
    }
}

void placeTreasures(){
     int treasuresPlaced= 0;
     int r, c;
     while(treasuresPlaced<TREASURES){
	     r = rand()% ROWS;
	     c = rand()% COLS;  
             
	     if(cellIsEmpty(r, c)){
		     map[r][c] = TREASURE_SYMBOL;
		     treasuresPlaced++;
             }
      }	     
}

void placeTraps(){
	int trapsPlaced= 0;
	int r, c;
	while(trapsPlaced < TRAPS){
		r = rand() %ROWS;
		c = rand() %COLS;

		if(cellIsEmpty(r, c)){
			hiddenTrap[r][c] = 1;
			trapsPlaced++;
		}
	}
}	

void placeHealthPacks(){
	int packsPlaced = 0;
	int r, c;

	while(packsPlaced < HEALTH_PACKS){
		r = rand() % ROWS;
		c = rand() % COLS;

		if(cellIsEmpty(r, c)){
			map[r][c] = HEALTH_PACKS;
			packsPlaced++;
		}
	}
}

void placeKeys(){
	int keysPlaced = 0;
        int r, c;

	while(keysPlaced< KEYS){
		r = rand() % ROWS;
		c = rand() % COLS;

		if(cellIsEmpty(r, c)){
			map[r][c] = KEY_SYMBOL;
			keysPlaced++;
		}
	}
}

void placeDoors(){
	int doorsPlaced = 0;
	int r, c;

	while(doorsPlaced< DOORS){
		r = rand() % ROWS;
		c = rand() % COLS;

		if(cellIsEmpty(r, c)){
			map[r][c] = DOOR_SYMBOL;
			doorsPlaced++;
		}
	}
}

void placePlayers(){
	int r, c, m;
	int playerCount;

	for (int i = 0; i< playerCount; i++){
		do{
			r= rand() % ROWS;
			c= rand() % COLS;
		}while(!cellIsEmpty(r, c));

		map[r][c] = players[i].symbol;

		players[i].x = r;
		players[i].y = c;
	}
}

void printMap(){
	system("clear");

	//header
	printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("           QUEST FOR LOST TREASURE             \n");
	printf("                   ROUND:%d                    \n", roundCounter);
	printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
        
	//render grid
	for(int r = 0; r<ROWS; r++){
		for(int c= 0; c<COLS; c++){
			printf("%c", map[r][c]);
		}
	}

	//HUD
	printf("\n----------------------------------------------\n");
	printf("PLAYER STATUS:\n");
	for (int i = 0; i<playerCount; i++){
		if (players[i].alive){
		printf(" %c %s: HP=%d, Score=%d, Kes=%d, Position=(%d,%d)\n",
			players[i].symbol, players[i].name,
		        players[i].health, players[i].score,
		        players[i].keys, players[i].x, players[i].y);
		} else{
		  printf(" %c %s: ELIMINATED\n", players[i].symbol, players[i].name);
		}
	}
        printf("treasures remaining: %d\n", remainingTreasures());
        printf("----------------------------------------------\n");

        //recent log
	printf("\nRECENT EVENTS:\n");
        int start = (logCount < ENTRIES_MADE) ? 0 : logCount - ENTRIES_MADE;
        for(int i = start; i<logCount; i++){
               printf(" %s\n", eventLog[i]);
	}
        printf("---------------------------------------------\n");
}

//TEMPORARYYYYYYYYYYYYYYYY
int remainingTreasures(){
	return 0;
}
