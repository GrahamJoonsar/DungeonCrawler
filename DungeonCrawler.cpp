#include <iostream>
#include <conio.h>
#include <Windows.h>

const int LEVEL_WIDTH = 60;
const int LEVEL_HEIGHT = 12;

char keyPressed = 'l';
bool keyBeenPressed = false;
bool running = true;

// The level struct, contains information about the current level
struct Level{
    char DispLevel[LEVEL_HEIGHT][LEVEL_WIDTH];

    Level(char _dp[LEVEL_HEIGHT][LEVEL_WIDTH]){
        for (int y = 0; y < LEVEL_HEIGHT; y++){
            for(int x = 0; x < LEVEL_WIDTH; x++){
                DispLevel[y][x] = _dp[y][x];
            }
        }
    }
};

// Player class
class Player{
    public:
        char disp = '$'; // What the player looks like on the console
        int x = 1, y = 1;

        void ProccessInput(char in, Level * l){
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
            }
            if (l->DispLevel[targY][targX] == ' '){ // Clear space
                x = targX;
                y = targY;
            }
        }
};

char TestDisp[LEVEL_HEIGHT][LEVEL_WIDTH] = {
    "###########################################################",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "###########################################################"
};

Level Test(TestDisp);

void DrawToConsole(Level * leveltoDraw, Player * p){
    for (int y = 0; y < LEVEL_HEIGHT; y++){
        for (int x = 0; x < LEVEL_WIDTH; x++){
            if (x == p->x && y == p->y){
                std::cout << p->disp;
            } else {
                std::cout << leveltoDraw->DispLevel[y][x];
            }
        }
        std::cout << std::endl; // Newline
    }
}

void ClearConsole(){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);
}

Level * currentLevel;

Player player;

int main(){
    system("cls");
    currentLevel = &Test;
    while (running){
        if (_kbhit()){ // Key pressed
            keyPressed = _getch();
            player.ProccessInput(keyPressed, currentLevel);
            DrawToConsole(currentLevel, &player);
            ClearConsole();
            //std::cout << keyBeenPressed << std::endl;
        }
    }
    system("cls");
}