#include <iostream>
#include <conio.h>
#include <Windows.h>

#define STANDARD_LEVEL_WIDTH 60
#define STANDARD_LEVEL_HEIGHT 20

const unsigned short DARK_BLUE = FOREGROUND_BLUE;
const unsigned short BRIGHT_BLUE = FOREGROUND_RED | FOREGROUND_INTENSITY;
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
};
Player player;

class Level{
    public:
        char selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH];
    Level(char _selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH]){
        // For some reason I can't assign this directly
        for (int y = 0; y < STANDARD_LEVEL_HEIGHT; y++){
            for (int x = 0; x < STANDARD_LEVEL_WIDTH; x++){
                selfDisplay[y][x] = _selfDisplay[y][x];
            }
        }
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
            if (y != player.y){
                std::cout << dispLevel->selfDisplay[y];
                break;
            } else if (x != player.x){
                putchar(dispLevel->selfDisplay[y][x]);
            } else {
                putchar(player.display);
            }
        }
        putchar('\n');
    }
}

int main(){
    setConsoleColour(BRIGHT_BLUE);
    Level Level1(Level1Disp);
    dispLevel = &Level1;
    while(running){
        // Drawing the level with the player inside
        draw();
        // Getting input
        getInput();
    }
}