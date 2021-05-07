#include <iostream>
#include <conio.h>
#include <Windows.h>

#define STANDARD_LEVEL_WIDTH 60
#define STANDARD_LEVEL_HEIGHT 20

// Colors
const unsigned short BRIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const unsigned short BRIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const unsigned short BRIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned short WHITE = 15;

void setConsoleColour(unsigned short colour)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    SetConsoleTextAttribute(hOut, colour);
}

bool running = true;

class Player{
    public:
        char display = 'O';
        // Putting this in one statement didn't work for some reason
        int x = 1;
        int y = 1;
        int health = 5;
};
Player player;

class HealthPack {
    public:
        int x, y;
        char disp = 'H';
    HealthPack(int _x, int _y, char _disp){
        x = _x;
        y = _y;
        disp = _disp;
    }
};

class Level{
    public:
        char selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH];
        HealthPack * hPacks;
        int healthNum; // Number of Health packs
    Level(char _selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH], HealthPack * _hPacks, int _healthNum){
        // For some reason I can't assign this directly
        for (int y = 0; y < STANDARD_LEVEL_HEIGHT; y++){
            for (int x = 0; x < STANDARD_LEVEL_WIDTH; x++){
                selfDisplay[y][x] = _selfDisplay[y][x];
            }
        }
        hPacks = _hPacks;
        healthNum = _healthNum;
    }
    bool isYPosHealth(int y){
        for (int i = 0; i < healthNum; i++){
            if (y == hPacks[i].y){
                return true;
            }
        }
        return false;
    }
    bool isXPosHealth(int x){
        for (int i = 0; i < healthNum; i++){
            if (x == hPacks[i].x){
                return true;
            }
        }
        return false;
    }
};


char Level1Disp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
    "###########################################################",
    "#           ###         ###         ###                   #",
    "#           ###         ###         ###                   #",
    "#           ###         ###         ###                   #",
    "#           ###         ###         ###                   #",
    "#           ###         ###         ###                   #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "###########################################################"
};

HealthPack Level1HealthPacks[] = {
    {55, 15, 'H'}
};

char LevelDefaultDisp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
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

// The level that will be displayed on screen
Level * dispLevel;

void getInput(){
    char key = _getch();
    if (key == 'w'){
        if (dispLevel->selfDisplay[player.y - 1][player.x] == ' '){
            player.y -= 1;
        }
    }
    if (key == 's'){
        if (dispLevel->selfDisplay[player.y + 1][player.x] == ' '){
            player.y += 1;
        }
    }
    if (key == 'a'){
        if (dispLevel->selfDisplay[player.y][player.x - 1] == ' '){
            player.x -= 1;
        }
    }
    if (key == 'd'){
        if (dispLevel->selfDisplay[player.y][player.x + 1] == ' '){
            player.x += 1;
        }
    }
    if (key == 'q'){
        running = false;
    }
}

void draw(){
    system("CLS");
    for (int y = 0; y < STANDARD_LEVEL_HEIGHT; y++){
        for (int x = 0; x < STANDARD_LEVEL_WIDTH; x++){
            if (y != player.y && !dispLevel->isYPosHealth(y)){
                setConsoleColour(WHITE);
                std::cout << dispLevel->selfDisplay[y];
                break;
            } else if (player.y == y && !dispLevel->isYPosHealth(y)){ // if just the player is on the line.
                if (x != player.x){
                    setConsoleColour(WHITE);
                    putchar(dispLevel->selfDisplay[y][x]);
                } else {
                    setConsoleColour(BRIGHT_BLUE);
                    putchar(player.display);
                }
            } else if (player.y != y && dispLevel->isYPosHealth(y)){ // If only a health pack is on the line
                if (!dispLevel->isXPosHealth(x)){
                    setConsoleColour(WHITE);
                    putchar(dispLevel->selfDisplay[y][x]);
                } else {
                    setConsoleColour(BRIGHT_GREEN);
                    putchar('H');
                }
            } else { // if both
                if (x != player.x && !dispLevel->isXPosHealth(x)){
                    setConsoleColour(WHITE);
                    putchar(dispLevel->selfDisplay[y][x]);
                } else if (x == player.x){
                    setConsoleColour(BRIGHT_BLUE);
                    putchar(player.display);
                } else if (dispLevel->isXPosHealth(x)){
                    setConsoleColour(BRIGHT_GREEN);
                    putchar('H');
                }
            }
        } // Seems like this above if statement could be optimized, but I'm to tired to fix rn
          // This is probably he best way to expand with other items like chests and enemies though
        putchar('\n');
    }
}

int main(){
    setConsoleColour(BRIGHT_BLUE);
    Level Level1(Level1Disp, Level1HealthPacks, 1);

    dispLevel = &Level1;
    while(running){
        // Drawing the level with the player inside
        draw();
        // Getting input
        getInput();
    }
}
