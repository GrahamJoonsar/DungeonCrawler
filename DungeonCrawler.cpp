#include <iostream>
#include <conio.h>
#include <Windows.h>

#define STANDARD_LEVEL_WIDTH 60
#define STANDARD_LEVEL_HEIGHT 20

// Compiler command:
// g++ -static-libgcc -static-libstdc++ DungeonCrawler.cpp -o DungeonCrawler.exe

/********************************************
 *              Dungeon Crawler             *
 * A text based dungeon crawler written in  *
 * C++.                                     *
 *                                          *
 * WASD to move Q to exit                   *
 *                                          *
 *                                          *
 *                                          *
 * Author: Graham Joonsar                   *
 * ******************************************/

/* These are what the Objects of in the game look like */
// Player : $
// HealthPack : H
// Enemy : X
// Portal Entrance : @
// Portal Exit : &

// Colors for the console 
const unsigned short BRIGHT_BLUE = 11;
const unsigned short BRIGHT_RED = 12;
const unsigned short BRIGHT_GREEN = 10;
const unsigned short BRIGHT_YELLOW = 14;
const unsigned short BRIGHT_PINK = 13;
const unsigned short WHITE = 15;

/* This functions sets the color of the characters outputted to the console */
void setConsoleColour(unsigned short colour)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    SetConsoleTextAttribute(hOut, colour);
}

const char w = 178;

// This is what causes the main game loop
bool running = true;
// The round number // Used for moving the enemies every three rounds
long int roundNum = 0;
// This is for making the olayer sword disapear after 1 frame
bool playerAttacked = false;

// This is the main player class
class Player{
    public:
        char display = '$'; // what the player looks like on the console
        // Putting this in one statement didn't work for some reason
        // x position
        int x = 1;
        // y position
        int y = 1;
        // The player's health
        int health = 5;
        
        // This function if for test if the player is at a specific position 
        bool isPlayerIn(int testingX, int testingY){
            return x == testingX && y == testingY;
        }
        
        // Checks if the player is dead
        void checkSelf(){
            if (health <= 0){
                running = false;
            }
        }
};
// The Player declaration 
Player player;

class Level{
    public:
        // Name of the level
        const char *name;
        // The grid of chars that will be displayed
        char selfDisplay[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH];
        // The index of the level the portal goes to
        int portalDestination;
        // The index that the level is // Used for updating the level when the player leaves
        int selfIndex;
        // The position of the entrance to the level
        int entranceX, entranceY;
    // The constructor
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

// the top and bottom of every level 
char topAndBottom[STANDARD_LEVEL_WIDTH] = "###########################################################";

// The maze level display
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
    "#     ###   ###   ###   ###   ###   ###   ###     X       #",
    "#     ###   ###   ###   ###   ###   ###   ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###             #",
    "#     ###         ###         ###         ###            @#",
    "###########################################################"
};


// The default level display
char LevelDefaultDisp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
    "###########################################################",
    "#&                                                        #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                           X                             #",
    "#            @                              X             #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#              X                                          #",
    "#                                                         #",
    "#                                X                        #",
    "#                                                         #",
    "#                                                       HH#",
    "#                                                       HH#",
    "###########################################################"
};

char LaserLevelDisp[STANDARD_LEVEL_HEIGHT][STANDARD_LEVEL_WIDTH] = {
    "###########################################################",
    "#&                                                        #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#               >                                  <      #",
    "#                                                         #",
    "#                                                         #",
    "#                                                         #",
    "#>                        <                               #",
    "#                                                         #",
    "#                                                         #",
    "#                            @                            #",
    "#                                                         #",
    "#                                                         #",
    "#     X                                                   #",
    "###########################################################"
};


// The level that will be displayed on screen
Level * dispLevel;

// The maze level
Level MazeLevel(Level1Disp, "Maze", 0, 1, 1, 1);
// The default empty level
Level EmptyLevel(LevelDefaultDisp, "Empty Level", 2, 1, 1, 0);
// the level with lasers
Level LaserLevel(LaserLevelDisp, "Laser level", 1, 1, 1, 2);

// The list of all levels // used for changing between levels
Level allLevels[] = {EmptyLevel, MazeLevel, LaserLevel};

// This should be done
// Updates enemies and lasers
void updateEnemies(){
    bool cantMoveOnX[STANDARD_LEVEL_WIDTH];
    bool isLasering = false;
    for (int y = 1; y < STANDARD_LEVEL_HEIGHT - 1; y++){ // For every char in the level
        // These are used for stopping the loop from moving the enemies more than once per player movement
        bool wentRight = false;
        for (int x = 1; x < STANDARD_LEVEL_WIDTH - 1; x++){
            // Enemy movement code
            if (dispLevel->selfDisplay[y][x] == 'X' && roundNum % 3 == 0){ // If enemy // Updates every 3 rounds
                int targetX = x;
                int targetY = y;
                if (x > player.x && dispLevel->selfDisplay[y][x - 1] == ' '){ // Move left
                    targetX -= 1;
                } else if (x < player.x && dispLevel->selfDisplay[y][x + 1] == ' '){ // move right
                    targetX += 1;
                } else if (y > player.y && dispLevel->selfDisplay[y - 1][x] == ' '){ // Move up
                    targetY -= 1;
                } else if (y < player.y && dispLevel->selfDisplay[y + 1][x] == ' ' && !cantMoveOnX[targetX]){ // move down
                    targetY += 1;
                }

                dispLevel->selfDisplay[y][x] = ' ';
                
                if (wentRight){
                    targetX = x;
                }
                /*
                 * The reason that I can't do the same thing with the Y as the X to stop the for loops
                 * from updating the enemies multiple times per round is because the X updates faster than the Y
                 * So I have to use an array of booleans to check if the Enemy on a specific X moved down on the previous for X loop.
                 */
                if (cantMoveOnX[targetX]){
                    cantMoveOnX[targetX] = false;
                }
                // If the player isn't where the enemy wants to move
                if (!player.isPlayerIn(targetX, targetY)){
                    dispLevel->selfDisplay[targetY][targetX] = 'X';
                } else {
                    player.health -= 1;
                }

                wentRight = false;

                if (targetX > x){
                    wentRight = true;
                }
                if (targetY > y){
                    cantMoveOnX[targetX] = true;
                }
            // Laser code
            } else if (dispLevel->selfDisplay[y][x] == '>'){
                isLasering = true;
            } else if (dispLevel->selfDisplay[y][x] == '<'){
                isLasering = false;
            } else if (isLasering && roundNum % 5 == 0){
                if (!player.isPlayerIn(x, y)){
                    dispLevel->selfDisplay[y][x] = '~';
                } else {
                    player.health--;
                    isLasering = false;
                }
            } else if (isLasering && roundNum % 5 == 1){ // after the laser was fired
                dispLevel->selfDisplay[y][x] = ' ';
            }
        }
        //isLasering = false;
    }
}

// Getting the player's input from the keyboard
// Also should be done
void getInput(){
    // Gets what the player pressed
    char key = _getch();
    int targetx = player.x;
    int targety = player.y;
    // Movement
    if (key == 'w'){
        targety--;
    }
    else if (key == 's'){
        targety++;
    }
    else if (key == 'a'){
        targetx--;
    }
    else if (key == 'd'){
        targetx++;
    }
    else if (key == 'q'){
        running = false;
    }
    // Sword Input
    if (true){ // If the player didn't attack the previous turn
        if (key == 'i' && (dispLevel->selfDisplay[player.y - 1][player.x] == 'X' || dispLevel->selfDisplay[player.y - 1][player.x] == ' ')){
            dispLevel->selfDisplay[player.y - 1][player.x] = '|';
            playerAttacked = true;
        }
        else if (key == 'k' && (dispLevel->selfDisplay[player.y + 1][player.x] == 'X' || dispLevel->selfDisplay[player.y + 1][player.x] == ' ')){
            dispLevel->selfDisplay[player.y + 1][player.x] = '|';
            playerAttacked = true;
        }
        else if (key == 'j' && (dispLevel->selfDisplay[player.y][player.x - 1] == 'X' || dispLevel->selfDisplay[player.y][player.x - 1] == ' ')){
            dispLevel->selfDisplay[player.y][player.x - 1] = '-';
            playerAttacked = true;
        }
        else if (key == 'l' && (dispLevel->selfDisplay[player.y][player.x + 1] == 'X' || dispLevel->selfDisplay[player.y][player.x + 1] == ' ')){
            dispLevel->selfDisplay[player.y][player.x + 1] = '-';
            playerAttacked = true;
        }
    }
    // If where the player wants to move is empty space or a portal exit
    if (dispLevel->selfDisplay[targety][targetx] == ' ' || dispLevel->selfDisplay[targety][targetx] == '&'){
        player.x = targetx;
        player.y = targety;
    } else if (dispLevel->selfDisplay[targety][targetx] == 'H'){ // If where the player wants to move is a health pack
        dispLevel->selfDisplay[targety][targetx] = ' ';
        player.health++;
        player.x = targetx;
        player.y = targety;
    } else if (dispLevel->selfDisplay[targety][targetx] == '@'){
        allLevels[dispLevel->selfIndex] = *dispLevel; // This saves what the player did in the level
        *dispLevel = allLevels[dispLevel->portalDestination]; // this updates the level

        player.x = dispLevel->entranceX;  // Setting the player xPos
        player.y = dispLevel->entranceY; // Setting the player yPos
    }
}

// Probably done
void draw(){
    // Clearing the console
    system("CLS");
    setConsoleColour(WHITE);
    // Outputting the top of the level
    std::cout << topAndBottom << std::endl;
    for (int y = 1; y < STANDARD_LEVEL_HEIGHT - 1; y++){ // for every line
        putchar('#');
        for (int x = 1; x < STANDARD_LEVEL_WIDTH - 2; x++){ // for every char on the line
            if (!player.isPlayerIn(x, y)){
                char currentChar = dispLevel->selfDisplay[y][x];
                if (currentChar == 'H'){
                    setConsoleColour(BRIGHT_GREEN);
                    putchar('H');
                } else if (currentChar == 'X'){
                    setConsoleColour(BRIGHT_RED);
                    putchar('X');
                } else if (currentChar == '@' || currentChar == '&'){
                    setConsoleColour(BRIGHT_YELLOW);
                    putchar(currentChar);
                } else if (currentChar == '|' || currentChar == '-'){
                    if (playerAttacked){
                        setConsoleColour(BRIGHT_PINK);
                        putchar(currentChar);
                        playerAttacked = false;
                    } else {
                        dispLevel->selfDisplay[y][x] = ' ';
                        putchar(' ');
                    }
                } else if (currentChar == '>' || currentChar == '<'){
                    setConsoleColour(BRIGHT_YELLOW);
                    putchar(currentChar);
                } else {
                    setConsoleColour(WHITE);
                    putchar(currentChar);
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
    std::cout << topAndBottom; // printing out the bottom
    setConsoleColour(BRIGHT_GREEN);
    std::cout << "Health: " << player.health;
    setConsoleColour(WHITE);
    std::cout << "  |  Level: " << dispLevel->name << '\n';
    std::cout.flush();
}

int main(){
    // Setting console size
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); 
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 510, 395, TRUE);

    system("title Dungeon Crawler!"); // Setting the title of the command prompt

    setConsoleColour(WHITE);
    // Setting the starter level to the maze level
    dispLevel = &MazeLevel;
    // Drawing the Welcome screen
    std::cout << "Welcome to DungeonCrawler" << std::endl;
    std::cout << "Use WASD to move, IJKL to use your sword" << std::endl;
    std::cout << "Press any key to start (besides q)" << std::endl;
    while(running){
        if (_kbhit()){ // If a key is pressed
            roundNum++;
            // Getting input
            getInput();
            // Moving the enemies
            updateEnemies();
            // Checking the player health
            player.checkSelf();
            // Drawing the level with the player inside
            draw();
        }
    }
    system("CLS");
    std::cout << "You died!" << std::endl;
    system("pause");
}
