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
#define MAX_HEALTH 100
#define MAX_MOVES 4
#define MAX_ENTRIES 100
#define MSG_LENGTH 256
#define ENTRIES_MADE 5
#define SAVE_FILE "savegame.dat"
#define LOG_FILE "gamelog.txt"

int  cellIsEmpty(int r, int c); 
void initializeGame();
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
int  checkGameEnd();            
void gameLoop();                
void saveGame();                
int  loadGame();                
void showScores();              //used bubble sort
int  remainingTreasures();      
void addLog(int r, char *m);    //circular buffer logic
void printRecentLog();          
void saveLog();                 
void showStats();                            

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
    srand((unsigned int)time(NULL));
    int choice;
    
    while (1) {
        printf("========================================\n");
        printf("   QUEST FOR THE LOST TREASURE\n");
        printf("========================================\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. Exit\n");
        printf("========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1:
                initializeGame();
                gameLoop();
                break;
            case 2:
                if (loadGame() == 1) {
                    printf("Game loaded successfully!\n");
                    gameLoop();
                } else {
                    printf("No save file found. Starting new game...\n");
                    initializeGame();
                    gameLoop();
                }
                break;
            case 3:
                printf("\nGoodbye! Thanks for playing!\n");
                return 0;
            default:
                printf("Invalid choice! Please enter 1, 2, or 3.\n");
        }
    }
}

void initializeGame(){
    roundCounter = 0;
    logCount = 0;

    do{
        printf("Enter number of players (1-3): ");
        scanf("%d", &playerCount);
        getchar();
    } while(playerCount < 1 || playerCount > MAX_PLAYERS);

    for(int i = 0; i < MAX_PLAYERS; i++){
        if(i < playerCount){
            sprintf(players[i].name, "Player %d", i + 1);
            players[i].symbol = (i == 0) ? '1' : (i == 1) ? '2' : '3';
            players[i].health = STARTING_HP;
            players[i].score = 0;
            players[i].keys = 0;
            players[i].alive = true;
            players[i].movesMade = 0;
            players[i].treasuresFound = 0;
            players[i].trapsTriggered = 0;
            players[i].damageTaken = 0;
            players[i].healthPacksUsed = 0;
            players[i].keysCollected = 0;
            players[i].doorsUnlocked = 0;
        } else {
            players[i].alive = false;
        }
    }

    initializeMap();
    addLog(roundCounter, "Game started");
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
	    map[COLS - 1][c] = WALL_SYMBOL; //bottom
    }
    for (r=0; r< ROWS; r++){
            map[0][r] = WALL_SYMBOL; //left
	        map[ROWS-1][r] = WALL_SYMBOL; //right
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
			map[r][c] = HEALTH_SYMBOL;
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
			printf("%c ", map[r][c]);
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

void movePlayer(int index){
	if(!players[index].alive){
		return;
	}	
		char move, movesMade= 0;
		printf("%s (%c) - enter 4 moves(WASD):\n", players[index].name, players[index].symbol);

		//get 4 moves
		for (int i=0; i<MAX_MOVES; i++){
			printf("move %d: ", i+1);
			scanf("%c", &move);
			move= toupper(move);

			int dx=0, dy= 0; //change made in position
			switch(move){
				case 'W': dx= -1; 
					  break;
				case 'S': dx= 1; 
				          break;
				case 'A': dy= -1;
				          break;
				case 'D': dy =1;
					  break;
				default: 
					  printf("invalid move '%c' skipped", move);
					  i--;
					  continue;
			}
			//new position of player.
			int newX = players[index].x + dx;
			int newY = players[index].y + dy;
                        
			if(!isValidMove(newX, newY)){
				printf("invalid move!!\n");
				continue;
			}

			//locked doors
			if(map[newX][newY] == DOOR_SYMBOL){
				if(players[index].keys > 0){
					players[index].keys--;
					players[index].doorsUnlocked++;
					map[newX][newY] = EMPTY_SYMBOL;
					printf("door unlocked! keys remaining: %d\n", players[index].keys);
				} else{
					printf("door locked! you have no keys.");
					continue;
				}
			}
			//empty tile on previous position
			map[players[index].x][players[index].y] = EMPTY_SYMBOL;
			//update player position
			players[index].x = newX;
			players[index].y = newY;
		        players[index].movesMade++;
		        map[newX][newY] = players[index].symbol;
		        
			processTile(index);

			//check if player died after processtile..
			if (!players[index].alive){
				map[newX][newY] = EMPTY_SYMBOL;
				printf("%s has died!", players[index].name);
				break;
			}

			movesMade++;
		}

		if(movesMade == MAX_MOVES){
			printf("turn complete, 4 moves made");
		}
}

int isValidMove(int x, int y){
	if(x<0 || x>=ROWS){
		return 0;
	} else if(y<0 || y>=COLS){
		return 0;
	} else if(map[x][y]==WALL_SYMBOL){
		return 0;
	} else{
		return 1;
	}
}

void processTile(int index){
	int x= players[index].x;
	int y= players[index].y;
	
	//check for hidden traps
	if(hiddenTrap[x][y] ==1){
		players[index].health-= TRAP_DAMAGE;
		players[index].trapsTriggered++;
		players[index].damageTaken+= TRAP_DAMAGE;
		hiddenTrap[x][y] = 0;

                printf("%s triggered a trap!! new HP: %d\n", players[index].name, players[index].health);
	
	        if(players[index].health <= 0){
		        players[index].health = 0;
		        players[index].alive = 0;
		        printf("%s has been eliminated!\n", players[index].name);
	         }
		return;
	}

	//check for treasures
	if(map[x][y] ==TREASURE_SYMBOL){
		players[index].score+= TREASURE_SCORE;
                players[index].treasuresFound++;
		printf("treasure found! new SCORE:%d\n", players[index].score);
		return;
	}
	if(map[x][y] == HEALTH_SYMBOL){
                if(players[index].health == MAX_HEALTH){
			printf("already at maxx health 100!\n");
		} else{
			players[index].health+= HEALTH_RESTORE;
			if(players[index].health> MAX_HEALTH){
				players[index].health = MAX_HEALTH;
			}
			players[index].healthPacksUsed++;
		
		        printf("%s health is at %d, %d health packs used.\n", players[index].name, players[index].health, players[index].healthPacksUsed);
		}
		return;
	}
	if(map[x][y]==KEY_SYMBOL){
		players[index].keys++;
                players[index].keysCollected++;
		printf("%s has collected a key! current key count: %d\n", players[index].name, players[index].keys);
		return;
	}
}

int checkGameEnd(){
	if(remainingTreasures() ==0){
		return 1;
	} 
	int aliveCount= 0;
	for(int i=0; i<playerCount; i++){
		if(players[i].alive){
			aliveCount++;
		}
	}
	if(aliveCount ==0){
		return 1;
	}
	return 0;
}

void gameLoop(){
	char answer;

	while(!checkGameEnd()){
		printMap();

		for(int i= 0; i<playerCount; i++){
			if(players[i].alive){
				movePlayer(i);
				if(checkGameEnd()){
					break;
				}
			}
		}

		roundCounter++;

		//save game
		printf("save game? (Y/N): ");
		scanf(" %c", &answer);
		getchar();
		answer = toupper(answer);
		if(answer== 'Y'){
			saveGame();
			printf("game saved~~");
		}
	}
	saveLog();
	showScores();
	showStats();
}

void saveGame(){
	FILE *file= fopen(SAVE_FILE, "wb");
	if (file == NULL){
		printf("error: could not save game.\n");
		return;
	}

	fwrite(&playerCount, sizeof(int), 1, file);
	fwrite(&roundCounter, sizeof(int), 1, file);
	fwrite(players, sizeof(player), MAX_PLAYERS, file);
	fwrite(map, sizeof(char), ROWS*COLS, file);
	fwrite(hiddenTrap, sizeof(int), ROWS*COLS, file);

	fclose(file);
	printf("game saved!!\n");
}

int loadGame(){
	FILE *file= fopen(SAVE_FILE, "rb");
	if(file ==NULL){
		return 0;
	}

	fread(&playerCount, sizeof(int), 1, file);

	if(playerCount<1 || playerCount>MAX_PLAYERS){
		fclose(file);
		return 0;
	}

	fread(&roundCounter, sizeof(int), 1, file);
	fread(players, sizeof(player), MAX_PLAYERS, file);
	fread(map, sizeof(char), ROWS*COLS, file);
	fread(hiddenTrap, sizeof(int), ROWS*COLS, file);

	fclose(file);
	return 1;
}
void showScores(){
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("                    GAME OVER                        \n");	
        printf("                   LEADERBOARD                       \n");

	//create copy for this func
	player sorted[MAX_PLAYERS];
	for (int i=0; i<playerCount; i++){
		sorted[i] = players[i];
        }

	for (int i=0; i<playerCount -1; i++){
		for (int j=0; j<playerCount -i -1; j++){
			int score1, score2;
			if(sorted[j].alive){
				score1= sorted[j].score + (sorted[j].health/2);
			} else{
				score1 = sorted[j].score;
			}

			if(sorted[j+1].alive){
				score2 = sorted[j+1].score + (sorted[j+1].health/2);
			} else{
				score2 = sorted[j+1].score;
			}

			if(score1 < score2){
				player swap= sorted[j];
				sorted[j] = sorted[j+1];
				sorted[j+1] = swap;
			}
		}
	}
        
	for(int i=0; i<playerCount; i++){
		if(sorted[i].alive){
			int bonus = sorted[i].health/2;
			int total = sorted[i].score + bonus;
			printf("%d. %s (%c): %d +%d(bonus) = %d\n", i+1, sorted[i].name, sorted[i].symbol, sorted[i].score, bonus, total);
		} else {
			printf("%d. %s (%c): %d(dead)\n", i+1, sorted[i].name, sorted[i].symbol, sorted[i].score);
		}
	}

	if (playerCount >=1 && sorted[0].alive){
		int top1= sorted[0].score + (sorted[0].health/2);

		if (playerCount >=2 &&sorted[1].alive){
			int top2 = sorted[1].score + (sorted[1].health/2);

			if(top1==top2){
				printf("\nTIE between players %s and %s with %d points\n",sorted[0].name, sorted[1].name, top1);
			} else { 
				printf("\nWINNER:%s(%c) with %d points!\n", sorted[0].name, sorted[0].symbol,top1);
			}
		} else if (playerCount>=2 && !sorted[1].alive){
			printf("\nWINNER:%s (%c) with %d points!\n",sorted[0].name, sorted[0].symbol, top1);
		} else{
			printf("\nWINNER: %s (%c) with %d points!\n", sorted[0].name, sorted[0].symbol, top1);
		}
	} else if(playerCount >=1 && !sorted[0].alive) {
		printf("\nALL PLAYERS ARE DEAD!\n");
	}
}

int remainingTreasures(){
	int count =0;
	for(int r= 0; r<ROWS; r++){
		for(int c=0; c<COLS; c++){
			if(map[r][c]==TREASURE_SYMBOL){
				count ++;
			}
		}
	}
	return count;
}

void addLog(int r, char*m){
	char msg[MSG_LENGTH];
	sprintf(msg, "[R%d] %s",r, m);

	if(logCount< MAX_ENTRIES) {
		strcpy(eventLog[logCount], msg);
		logCount++;
	} else if(logCount == MAX_ENTRIES){
		for(int i=0; i<MAX_ENTRIES -1; i++){
			strcpy(eventLog[i], eventLog[i+1]);
		}
		strcpy(eventLog[MAX_ENTRIES - 1], msg);
	}
}

void printRecentLog(){
	printf("\nrecent events: \n");
	if(logCount < ENTRIES_MADE){
		for(int i=0; i<logCount; i++){
			printf("%s\n", eventLog[i]);
		}
	} else if (logCount>= ENTRIES_MADE){
		int start = logCount - ENTRIES_MADE;
		for(int i= start; i<logCount; i++){
			printf(" %s\n", eventLog[i]);
		}
	}
}

void saveLog(){
	FILE *file =fopen(LOG_FILE, "w");
	if(file ==NULL){
		printf("error!! couldnt open file");
		return;
	}
	for(int i=0; i<logCount; i++){
		fprintf(file, "%s\n", eventLog[i]);
	}

	fclose(file);
}

void showStats(){
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("                 PLAYER STATISTICS                 \n");
	printf("Player     Treasures     Traps     Health packs     Keys     Doors\n");

	for(int i=0; i<playerCount; i++){
		printf("%s %-10d %-10d %-10d %-10d %-10d %-10d %-10d\n", players[i].name, players[i].movesMade, players[i].treasuresFound, players[i].trapsTriggered, players[i].damageTaken, players[i].healthPacksUsed, players[i].keysCollected, players[i].doorsUnlocked);
	}
}
