#include <iostream>
#include <conio.h>
#include <Windows.h>

#define STANDARD_LEVEL_WIDTH 60
#define STANDARD_LEVEL_HEIGHT 20

// Colors
const unsigned short BRIGHT_BLUE = 11;
const unsigned short BRIGHT_RED = 12;
const unsigned short BRIGHT_GREEN = 10;
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
        char display = '$';
        // Putting this in one statement didn't work for some reason
        int x = 1;
        int y = 1;
        int health = 5;

        bool isPlayerIn(int testingX, int testingY){
            return x == testingX && y == testingY;
        }
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

char topAndBottom[STANDARD_LEVEL_WIDTH] = "###########################################################";

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


char LevelDefaultDisp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
    "###########################################################",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                           X                             #",
    "#                                                         #",
    "#                           X                             #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                       HH#",
    "#                                                       HH#",
    "###########################################################"
};


// The level that will be displayed on screen
Level * dispLevel;

void getInput(){
    char key = _getch();
    int targetx = player.x;
    int targety = player.y;
    if (key == 'w'){
        targety -= 1;
    }
    if (key == 's'){
        targety += 1;
    }
    if (key == 'a'){
        targetx -= 1;
    }
    if (key == 'd'){
        targetx += 1;
    }
    if (key == 'q'){
        running = false;
    }
    if (dispLevel->selfDisplay[targety][targetx] == ' '){
        player.x = targetx;
        player.y = targety;
    } else if (dispLevel->selfDisplay[targety][targetx] == 'H'){
        dispLevel->selfDisplay[targety][targetx] = ' ';
        player.health++;
        player.x = targetx;
        player.y = targety;
    } else if (dispLevel->selfDisplay[targety][targetx] == 'X'){
        dispLevel->selfDisplay[targety][targetx] = ' ';
        player.health--;
        player.x = targetx;
        player.y = targety;
    }
}

void draw(){
    system("CLS");
    setConsoleColour(WHITE);
    std::cout << topAndBottom << std::endl;
    for (int y = 1; y < STANDARD_LEVEL_HEIGHT - 1; y++){
        putchar('#');
        for (int x = 1; x < STANDARD_LEVEL_WIDTH - 2; x++){
            if (!player.isPlayerIn(x, y)){
                if (dispLevel->selfDisplay[y][x] == 'H'){
                    setConsoleColour(BRIGHT_GREEN);
                    putchar('H');
                } else if (dispLevel->selfDisplay[y][x] == 'X'){
                    setConsoleColour(BRIGHT_RED);
                    putchar('X');
                } else {
                    setConsoleColour(WHITE);
                    putchar(dispLevel->selfDisplay[y][x]);
                }
            } else {
                setConsoleColour(BRIGHT_BLUE);
                putchar(player.display);
            }
        }
        setConsoleColour(WHITE);
        putchar('#'); // the sides will always be #, so this makes it more efficient
        putchar('\n'); // the entire line has been looped through so move to the next
    }
    std::cout << topAndBottom;
    setConsoleColour(BRIGHT_GREEN);
    std::cout << "Health: " << player.health << std::endl; 
}

int main(){
    // Setting console size
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); 
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 510, 395, TRUE);

    setConsoleColour(WHITE);
    Level Level1(Level1Disp);
    Level DefaultLevel(LevelDefaultDisp);

    dispLevel = &DefaultLevel;
    while(running){
        // Drawing the level with the player inside
        draw();
        // Getting input
        getInput();
    }
}
