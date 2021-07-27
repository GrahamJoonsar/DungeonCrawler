#include <iostream>
#include <conio.h>
#include <Windows.h>

const int LEVEL_WIDTH = 60;
const int LEVEL_HEIGHT = 12;

unsigned int turnNumber = 0;

// Console colors
const unsigned short BRIGHT_BLUE = 11;
const unsigned short BRIGHT_RED = 12;
const unsigned short BRIGHT_GREEN = 10;
const unsigned short BRIGHT_YELLOW = 14;
const unsigned short BRIGHT_PINK = 13;
const unsigned short WHITE = 15;

char keyPressed;
bool keyBeenPressed = false;
bool running = true;

// Console Functions
void ClearConsole(){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);
}

void setConsoleColour(unsigned short color){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    SetConsoleTextAttribute(hOut, color);
}

// Player class
class Player{
    private:
    public:
        char disp = '$'; // What the player looks like on the console
        int x = 1, y = 1;
        unsigned short health = 9;

        void ProccessInput(char in, char DispLevel[][60]){
            int targX = x, targY = y;
            switch(in){
                case 'w':
                    targY--;
                    break;
                case 's':
                    targY++;
                    break;
                case 'a':
                    targX--;
                    break;
                case 'd':
                    targX++;
                    break;
                case 'q':
                    running = false;
                    break;
                case 'x':
                    health--;
                    break;
            }
            if (DispLevel[targY][targX] == ' '){ // Clear space
                x = targX;
                y = targY;
            } else if (DispLevel[targY][targX] == 'H'){ // Health pack
                DispLevel[targY][targX] = ' ';
                health++;
                x = targX;
                y = targY;
            }
        }
};

class Enemy{
    public:
        int x, y;
        bool dead = false;

        Enemy(int _x, int _y){
            x = _x;
            y = _y;
        }

        void move(char DispLevel[][60], Player * p){
            int targy = y, targx = x;
            if (p->y < y){ // Moving up
                targy--;
            } else if (p->y > y){
                targy++;
            } else if (p->x < x){
                targx--;
            } else {
                targx++;
            }
            if (DispLevel[targy][targx] == ' '){ // Clear space
                x = targx;
                y = targy;
            } else if (p->x == x && p->y == y){ // Player
                p->health--;
                dead = true;
            }
        }
};

// The level struct, contains information about the current level
struct Level{
    public:
        char DispLevel[LEVEL_HEIGHT][LEVEL_WIDTH]; // The char grid that represents the level
        const char * lname; // The name of the level
        Enemy * enemies; // the Enemies in the level
        int ENum; // Number of enemies

        // Constructor
        Level(char _dp[LEVEL_HEIGHT][LEVEL_WIDTH], const char * _lname, Enemy * _es, int _en){
            for (int y = 0; y < LEVEL_HEIGHT; y++){
                for(int x = 0; x < LEVEL_WIDTH; x++){
                    DispLevel[y][x] = _dp[y][x];
                }
            }
            lname = _lname;
            enemies = _es;
            ENum = _en;
        }

        bool EnemyAt(int x, int y){ // Checking if an enemy is at a position
            for (int i = 0; i < ENum; i++){
                if (enemies[i].x != x){
                    continue;
                } else {
                    return enemies[i].y == y && !enemies[i].dead;
                }
            }
            return false;
        }
};

char TestDisp[LEVEL_HEIGHT][LEVEL_WIDTH] = {
    "###########################################################",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                       H #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "###########################################################"
};

Enemy TestEnemies[] = {{30, 6}};

Level Test(TestDisp, "test", TestEnemies, 1);

void DrawToConsole(Level * leveltoDraw, Player * p){
    // Looping through level
    for (int y = 0; y < LEVEL_HEIGHT; y++){
        for (int x = 0; x < LEVEL_WIDTH; x++){
            char currentChar = leveltoDraw->DispLevel[y][x];
            if (x == p->x && y == p->y){ // If its the player
                setConsoleColour(BRIGHT_BLUE);
                std::cout << p->disp;
            } else if (currentChar == 'H'){
                setConsoleColour(BRIGHT_GREEN);
                std::cout << 'H';
            } else if (leveltoDraw->EnemyAt(x, y)){
                setConsoleColour(BRIGHT_RED);
                std::cout << 'X';
            } else {
                setConsoleColour(WHITE);
                std::cout << leveltoDraw->DispLevel[y][x];
            }
        }
        std::cout << std::endl; // Newline
    }
    std::cout << "HEALTH: ";
    setConsoleColour(BRIGHT_GREEN);
    std::cout << p->health;
    setConsoleColour(WHITE);
    std::cout << "  |  Level: ";
    std::cout << leveltoDraw->lname;
    std::cout << "                       " << std::endl; // Clearing the chars

    setConsoleColour(WHITE);
}


// The level thats being displayed
Level * currentLevel;
Player player;

int main(){
    currentLevel = &Test; // Setting the level
    
    // Hiding the cursor
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo); // Get console cursor information
    CursorInfo.bVisible = false; // Hide the console cursor
    SetConsoleCursorInfo(handle, &CursorInfo); // Set the console cursor state

    system("title Dungeon Crawler"); // Setting the title of the console window
    system("cls"); // Clearing the screen

    std::cout << "Welcome to Dungeon Crawler!" << std::endl;
    system("pause");
    system("cls");
    player.x = player.y = 1; // top left

    while (running){ // Main game loop
        if (_kbhit()){ // Key pressed
            player.ProccessInput(_getch(), currentLevel->DispLevel); // Proccess the input
            for (int i = 0; i < currentLevel->ENum; i++){
                currentLevel->enemies[i].move(currentLevel->DispLevel, &player);
            }
            DrawToConsole(currentLevel, &player); // Drawing the level to console
            ClearConsole(); // Clearing the console without flicker
        }
    }
    system("cls"); // Clearing screen again
    std::cout << "Thanks for playing!" << std::endl;
}