#include <iostream>
#include <conio.h>
#include <Windows.h>

#define STANDARD_LEVEL_WIDTH 60
#define STANDARD_LEVEL_HEIGHT 20

// Player : $
// HealthPack : H
// Enemy : X
// Portal Entrance : @
// Portal Exit : &

// Colors
const unsigned short BRIGHT_BLUE = 11;
const unsigned short BRIGHT_RED = 12;
const unsigned short BRIGHT_GREEN = 10;
const unsigned short BRIGHT_YELLOW = 14;
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
        const char *name;
        char selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH];
        int portalDestination; // The index of the level the portal goes to
        int selfIndex;
        int entranceX, entranceY;
    Level(char _selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH], const char * _name, int _portalDestination, int _newX, int _newY, int _selfIndex){
        // For some reason I can't assign this directly
        for (int y = 0; y < STANDARD_LEVEL_HEIGHT; y++){
            for (int x = 0; x < STANDARD_LEVEL_WIDTH; x++){
                selfDisplay[y][x] = _selfDisplay[y][x];
            }
        }
        name = _name;
        entranceX = _newX;
        entranceY = _newY;
        portalDestination = _portalDestination;
        selfIndex = _selfIndex;
    }
};

char topAndBottom[STANDARD_LEVEL_WIDTH] = "###########################################################";

char Level1Disp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
    "###########################################################",
    "#&          ###         ###         ###                   #",
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
    "#     ###         ###         ###         ###            @#",
    "###########################################################"
};


char LevelDefaultDisp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
    "###########################################################",
    "#&                                                        #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                           X                             #",
    "#            @                                            #",
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

Level MazeLevel(Level1Disp, "Maze", 0, 1, 1, 1);
Level EmptyLevel(LevelDefaultDisp, "Empty Level", 1, 1, 1, 0);

Level allLevels[] = {EmptyLevel, MazeLevel};

void getInput(){
    char key = _getch();
    int targetx = player.x;
    int targety = player.y;
    if (key == 'w'){
        targety--;
    }
    if (key == 's'){
        targety++;
    }
    if (key == 'a'){
        targetx--;
    }
    if (key == 'd'){
        targetx++;
    }
    if (key == 'q'){
        running = false;
    }
    if (dispLevel->selfDisplay[targety][targetx] == ' ' || dispLevel->selfDisplay[targety][targetx] == '&'){
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
    } else if (dispLevel->selfDisplay[targety][targetx] == '@'){
        allLevels[dispLevel->selfIndex] = *dispLevel; // This saves what the player did in the level
        *dispLevel = allLevels[dispLevel->portalDestination]; // this updates the level

        player.x = dispLevel->entranceX;  // Setting the player xPos
        player.y = dispLevel->entranceY; // Setting the player yPos
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
                } else if (dispLevel->selfDisplay[y][x] == '@' || dispLevel->selfDisplay[y][x] == '&'){
                    setConsoleColour(BRIGHT_YELLOW);
                    putchar(dispLevel->selfDisplay[y][x]);
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
    std::cout << "Health: " << player.health;
    setConsoleColour(WHITE);
    std::cout << "  |  Level: " << dispLevel->name << std::endl;
}

int main(){
    // Setting console size
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); 
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 510, 395, TRUE);

    setConsoleColour(WHITE);
    dispLevel = &MazeLevel;
    while(running){
        // Drawing the level with the player inside
        draw();
        // Getting input
        getInput();
    }
}
