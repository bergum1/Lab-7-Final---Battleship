/*
 Austin Bergum
 C++ Fall 2022
 Due: 12 December 2022
 Lab 7 Battleship Lab
 Description: For this lab you will make a simple battleship game. If you’re not familiar
 with the game, some quick internet searches should help. For this lab, you will need four
 10 x 10 grids (two for the user and two for the computer). The program will begin with
 setup. The program will need to randomly place the computers 5 ships, making sure the
 ships do not overlap or go off the board.
 */

//Board 0 = user board place ships
//Board 1 = computer board place ships
//Board 2 = shows user hit and misses on the enemy board
//Board 3 = tells computer where hit and miss are
//Board 4 = replaces # with 'char (5,4,3,2,1)' to tell user if they destroyed enemy ship
//Board 5 = replaces # with 'char (5,4,3,2,1)' to tell user if computer destroyed their ship
//Board 6 = wins and losses

#include <iostream> //library included to output to screen
#include <iomanip> //for setw
#include <stdlib.h> //random number generator
#include <time.h> //time call for the seeding

using namespace std;

typedef char grid [10][10][6]; //created 3 dimentional array

void table( grid water ); //precondition - 3 dimentional array must be made
                          //postcondition - generates table with water on it
void display( grid water, int board ); //precondition - table and array must be generated
                                       //postcondition - formats the board outputted and can later chose which board get outputted
void uShips( grid water, char shipType, bool &again ); //precondition - diplay must be valid and made
                                                       //postcondition - lets user place ships onto board 0, replaces ~ with #
void cShips( grid water, char &shipType ); //precondition - grid water must be called
                                           //postcondition - computer places ships onto board 1 so user cant see where they are
void uHM ( grid water, bool &again ); //precondition - all ships must be places from user and computer
                                      //postcondition - user gets to choose where to hit on the enemy board and displays H or M
void cHM ( grid water ); //precondition - all ships must be places from user and computer
                         //postcondition - computer chooses random position to hit on user board and displays H or M
void comShipsNum ( grid water, int &xpos, int &ypos, int &comCarrier, int &comBattleship, int &comCruiser, int &comSubmarine, int &comDestroyer ); //precondition - user has to pick a position on enemy board to hit on
                                                                                                                                                   //postcondition - keeps track of how many positions are left for each ship type and then outputs to screen if ship type is destroyed
void userShipsNum ( grid water, int &xnum, int &ynum, int &userCarrier, int &userBattleship, int &userCruiser, int &userSubmarine, int &userDestroyer ); //precondition - computer must pick a position to hit on user board
                                                                                                                                                         //postcondition - keeps track of how many positions are left for each ship type and then outputs to screen if ship type is destroyed
void header ( bool &again ); //precondition - program must run
                             //postcondition - lets player continue to game or stop program due to choice
void winLoss (); //just have it displaying 0 for now, going to work on function somw other time.

int main()
{
    srand(int(time(NULL)));
    char shipType;
    bool again = true;
    bool game = true;
    grid water;
    table(water);
    
    header(again);
    
    do
    {
        if ( !again ) //ends code if user chooses n
        {
            break ;
        }
        
        cout << endl << "You may enter 0 into the x position to surrender at any time" << endl;
        
        uShips(water, shipType, again);
        
        if ( !again ) //ends code if user surrenders
        {
            break ;
        }
        
        cShips(water, shipType);

        do
        {
            uHM(water, again);
            
            if ( !again ) //ends code if user surrenders
            {
                break ;
            }
            
        }while(game);
        
    }while(again);
    
    winLoss();
    
    return 0;
}

void table( grid water ) //generates the table for the grid
{
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            for (int z = 0; z < 6; z++)
            {
                water[x][y][z] = '~';
            }
        }
    }
}

void display( grid water, int board ) //creates board to be outputted later on
{
        cout << endl;
        cout << "   1 2 3 4 5 6 7 8 9 10" << endl;
        
        for (int y = 0; y < 10 ; y++)
        {
            cout << setw(2) << y + 1;
            cout << ' ';
            
            for (int x = 0; x < 10; x++)
            {
                cout << water[x][y][board] << ' ';
            }
            cout << endl;
        }
}

void uShips( grid water, char shipType, bool &again ) //user places ships
{
    int shipSize = 0;
    int xpos;
    int ypos;
    int direction;
    
    for (int a = 0; a < 5; a++)
    {
        display(water, 0);
        
        switch (a)
        {
            case 0:
                shipSize = 5;
                shipType = '5';
                cout << endl << "Please insert your carrier ship" << endl;
                break;
            case 1:
                shipSize = 4;
                shipType = '4';
                cout << endl << "Please insert your battleship" << endl;
                break;
            case 2:
                shipSize = 3;
                shipType = '3';
                cout << endl << "Please insert your cruiser ship" << endl;
                break;
            case 3:
                shipSize = 3;
                shipType = '2';
                cout << endl << "Please insert your submarine" << endl;
                break;
            case 4:
                shipSize = 2;
                shipType = '1';
                cout << endl << "Please insert your destroyer ship" << endl;
                break;
        }
        
        cout << "x position: ";
        cin >> xpos;
        
        while((xpos < 1 || xpos > 10) && xpos != 101010)
        {
            cout << "Input is invalid. Please try again" << endl << "x position: ";
            cin >> xpos;
        }
        
        if (xpos == 101010)
        {
            again = false;
            break;
        }
        
        cout << "y position: ";
        cin >> ypos;
        
        while (ypos < 1 || ypos > 10)
        {
            cout << "Input is invalid. Please try again" << endl << "y position: ";
            cin >> ypos;
        }
        
        if (water[xpos - 1][ypos - 1][0] == '#') //cant place first position of ship on another
        {
            a--;
            cout << endl << "Invalid position. Please choose a different position." << endl;
            continue;
        }
        
        water[xpos - 1][ypos - 1][0] = '#';
        water[xpos - 1][ypos - 1][5] = shipType;
        display(water, 0);
        
        bool invalidDir[4] = {false, false, false, false};
        
        cout << endl;
        
        for (int x = 1; x < shipSize; x++)
        {
            if (water[xpos - 1][ypos - 1 - x][0] != '#' && invalidDir[0] == false && xpos <= 10 && xpos >= 1 && ypos - x <= 10 && ypos - x >= 1)
            {
                invalidDir[0] = false;
            }
            else
            {
                invalidDir[0] = true;
            }
            
            if (water[xpos - 1][ypos - 1 + x][0] != '#' && invalidDir[1] == false && xpos <= 10 && xpos >= 1 && ypos + x <= 10 && ypos + x >= 1)
            {
                invalidDir[1] = false;
            }
            else
            {
                invalidDir[1] = true;
            }
            
            if (water[xpos - 1 - x][ypos - 1][0] != '#' && invalidDir[2] == false && xpos - x <= 10 && xpos - x >= 1 && ypos <= 10 && ypos >= 1)
            {
                invalidDir[2] = false;
            }
            else
            {
                invalidDir[2] = true;
            }
            
            if (water[xpos - 1 + x][ypos - 1][0] != '#' && invalidDir[3] == false && xpos + x <= 10 && xpos + x >= 1 && ypos <= 10 && ypos >= 1)
            {
                invalidDir[3] = false;
            }
            else
            {
                invalidDir[3] = true;
            }
        }
        
        if (invalidDir[0] == false)
        {
            cout << "(1)Up  ";
        }
        
        if (invalidDir[1] == false)
        {
            cout << "(2)Down  ";
        }
        
        if (invalidDir[2] == false)
        {
            cout << "(3)Left  ";
        }
        
        if (invalidDir[3] == false)
        {
            cout << "(4)Right  ";
        }
        
        cout << endl << "Choice of direction: ";
        
        do
        {
            cin >> direction;
            
            if (invalidDir[direction - 1] == true)
            {
                cout << endl << "Direction is invalid. Please try again" << endl << "Choice of direction: ";
            }
        }while(invalidDir[direction - 1] == true);
        
        for (int x = 0; x < shipSize; x++) //placing ships
        {
            if (direction == 1)
            {
                water[xpos - 1][ypos - 1 - x][0] = '#';
                water[xpos - 1][ypos - 1 - x][5] = shipType;
            }
            else if (direction == 2)
            {
                water[xpos - 1][ypos - 1 + x][0] = '#';
                water[xpos - 1][ypos - 1 + x][5] = shipType;
            }
            else if (direction == 3)
            {
                water[xpos - 1 - x][ypos - 1][0] = '#';
                water[xpos - 1 - x][ypos - 1][5] = shipType;
            }
            else if (direction == 4)
            {
                water[xpos - 1 + x][ypos - 1][0] = '#';
                water[xpos - 1 + x][ypos - 1][5] = shipType;
            }
            else
            {
                cout << endl << "Direction is invalid. Please try again" << endl << "Choice of direction: ";
                cin >> direction;
            }
        }
    }
}

void cShips( grid water, char &shipType ) //computer places ships
{
    int sizeShip = 0;
    int xnum = 0;
    int ynum = 0;
    int dnum;
    
    for (int a = 0; a < 5; a++)
    {
        switch (a)
        {
            case 0:
                sizeShip = 5;
                shipType = '5';
                break;
            case 1:
                sizeShip = 4;
                shipType = '4';
                break;
            case 2:
                sizeShip = 3;
                shipType = '3';
                break;
            case 3:
                sizeShip = 3;
                shipType = '2';
                break;
            case 4:
                sizeShip = 2;
                shipType = '1';
                break;
        }
        
        bool validSpot = false;
        
        do
        {
            xnum = rand() % 10 + 1;
            ynum = rand() % 10 + 1;
            
            if (water[xnum - 1][ynum - 1][1] == '#') //create new point if occupied
            {
                validSpot = false;
                xnum = rand() % 10 + 1;
                ynum = rand() % 10 + 1;
            }
            else
            {
                validSpot = true;
            }
            
        }while(validSpot == false);
        
        water[xnum - 1][ynum - 1][1] = '#';
        water[xnum - 1][ynum - 1][4] = shipType;
        
        bool invalidDirection[4] = {false, false, false, false};
        
        for (int x = 1; x < sizeShip; x++)
        {
            if (water[xnum - 1][ynum - 1 - x][1] != '#' && invalidDirection[0] == false && xnum <= 10 && xnum >= 1 && ynum - x <= 10 && ynum - x >= 1)
            {
                invalidDirection[0] = false;
            }
            else
            {
                invalidDirection[0] = true;
            }
            
            if (water[xnum - 1][ynum - 1 + x][1] != '#' && invalidDirection[1] == false && xnum <= 10 && xnum >= 1 && ynum + x <= 10 && ynum + x >= 1)
            {
                invalidDirection[1] = false;
            }
            else
            {
                invalidDirection[1] = true;
            }
            
            if (water[xnum - 1 - x][ynum - 1][1] != '#' && invalidDirection[2] == false && xnum - x <= 10 && xnum - x >= 1 && ynum <= 10 && ynum >= 1)
            {
                invalidDirection[2] = false;
            }
            else
            {
                invalidDirection[2] = true;
            }
            
            if (water[xnum - 1 + x][ynum - 1][1] != '#' && invalidDirection[3] == false && xnum + x <= 10 && xnum + x >= 1 && ynum <= 10 && ynum >= 1)
            {
                invalidDirection[3] = false;
            }
            else
            {
                invalidDirection[3] = true;
            }
        }
        
        do
        {
            dnum = rand() % 4 + 1;
            
            if (invalidDirection[dnum - 1] == true)
            {
                dnum = rand() % 4 + 1;
            }
            
        }while(invalidDirection[dnum - 1] == true);
        
        for (int x = 0; x < sizeShip; x++)
        {
            if (dnum == 1)
            {
                water[xnum - 1][ynum - 1 - x][1] = '#';
                water[xnum - 1][ynum - 1 - x][4] = shipType;
            }
            else if (dnum == 2)
            {
                water[xnum - 1][ynum - 1 + x][1] = '#';
                water[xnum - 1][ynum - 1 + x][4] = shipType;
            }
            else if (dnum == 3)
            {
                water[xnum - 1 - x][ynum - 1][1] = '#';
                water[xnum - 1 - x][ynum - 1][4] = shipType;
            }
            else if (dnum == 4)
            {
                water[xnum - 1 + x][ynum - 1][1] = '#';
                water[xnum - 1 + x][ynum - 1][4] = shipType;
            }
        }
    }

    cout << "-------------------------------------" << endl << "-------------------------------------" << endl;
}

void uHM ( grid water, bool &again ) //user picks position to hitn on enemy board
{
    int xpos;
    int ypos;
    bool play = true;
    int comCarrier = 5;
    int comBattleship = 4;
    int comCruiser = 3;
    int comSubmarine = 3;
    int comDestroyer = 2;
    
    display(water, 2);
    display(water, 0);
    
    cout << endl << "Lets start playing! " << endl;
    
    do
    {
        cout << endl << "Choose a position on the enemy board." << endl;
        
        cout << "x position: ";
        cin >> xpos;
        
        while((xpos < 1 || xpos > 10) && xpos != 101010)
        {
            cout << "Input is invalid. Please try again" << endl << "x position: ";
            cin >> xpos;
        }
        
        if (xpos == 101010)
        {
            again = false;
            break;
        }
        
        cout << "y position: ";
        cin >> ypos;
        
        while(ypos < 1 || ypos > 10)
        {
            cout << "Input is invalid. Please try again" << endl << "y position: ";
            cin >> ypos;
        }
        
        if (water[xpos - 1][ypos - 1][2] == 'H' || water[xpos - 1][ypos - 1][2] == 'M')
        {
            cout << "______________________________________" << endl;
            cout << "Position is invalid. Please try again." << endl;
            cout << "x position: ";
            cin >> xpos;
            
            while((xpos < 1 || xpos > 10) && xpos != 101010)
            {
                cout << "Input is invalid. Please try again" << endl << "x position: ";
                cin >> xpos;
            }
            
            if (xpos == 101010)
            {
                again = false;
                break;
            }
            
            cout << "y position: ";
            cin >> ypos;
            
            while(ypos < 1 || ypos > 10)
            {
                cout << "Input is invalid. Please try again" << endl << "y position: ";
                cin >> ypos;
            }
        }
        
        cout << "--------------------------------------" << endl << "--------------------------------------" << endl;
        
        if (water[xpos - 1][ypos - 1][1] == '~')
        {
            water[xpos - 1][ypos - 1][2] = 'M';
            cout << "You missed!";
        }
        else if (water[xpos - 1][ypos - 1][1] == '#')
        {
            water[xpos - 1][ypos - 1][2] = 'H';
            cout << "You hit!";
        }
        
        cHM(water);
        comShipsNum(water, xpos, ypos, comCarrier, comBattleship, comCruiser, comSubmarine, comDestroyer);
        
    }while(play == true);
    
    if (again)
    {
        display(water, 2);
        display(water, 0);
    }
}

void cHM ( grid water ) //computer picks position to hit on user board
{
    int xnum;
    int ynum;
    
    xnum = rand() % 10 + 1;
    ynum = rand() % 10 + 1;
        
    if (water[xnum - 1][ynum - 1][0] == '~')
    {
        water[xnum - 1][ynum - 1][0] = 'M';
        cout << endl << "Computer missed!" << endl;
    }
    else if (water[xnum - 1][ynum - 1][0] == '#')
    {
        water[xnum - 1][ynum - 1][0] = 'H';
        cout << endl << "Computer hit!" << endl;
    }
    
    if (water[xnum - 1][ynum - 1][0] == 'H')
    {
        xnum = rand() % 10 + 1;
        ynum = rand() % 10 + 1;
    }
    else if (water[xnum - 1][ynum - 1][0] == 'M')
    {
        xnum = rand() % 10 + 1;
        ynum = rand() % 10 + 1;
    }
    
    int userCarrier = 5;
    int userBattleship = 4;
    int userCruiser = 3;
    int userSubmarine = 3;
    int userDestroyer = 2;
    
    userShipsNum(water, xnum, ynum, userCarrier, userBattleship, userCruiser, userSubmarine, userDestroyer);
}

void comShipsNum ( grid water, int &xpos, int &ypos, int &comCarrier, int &comBattleship, int &comCruiser, int &comSubmarine, int &comDestroyer ) //output if you destroyed an enemy ship
{
    if (water[xpos - 1][ypos - 1][4] == '5')
    {
        comCarrier--;
    }
    else if (water[xpos - 1][ypos - 1][4] == '4')
    {
        comBattleship--;
    }
    else if (water[xpos - 1][ypos - 1][4] == '3')
    {
        comCruiser--;
    }
    else if (water[xpos - 1][ypos - 1][4] == '2')
    {
        comSubmarine--;
    }
    else if (water[xpos - 1][ypos - 1][4] == '1')
    {
        comDestroyer--;
    }
    
    if (comCarrier != 0 && comBattleship != 0 && comCruiser != 0 && comSubmarine != 0 && comDestroyer != 0)
    {
        cout << "All enemy ships are remaining" << endl;
    }
    
    if (comCarrier == 0)
    {
        cout << "Enemy Carrier destroyed!" << endl;
    }
    
    if (comBattleship == 0)
    {
        cout << "Enemy battleship destroyed!" << endl;
    }
    
    if (comCruiser == 0)
    {
        cout << "Enemy cruiser destroyed!" << endl;
    }
    
    if (comSubmarine == 0)
    {
        cout << "Enemy submarine destroyed!" << endl;
    }
    
    if (comDestroyer == 0)
    {
        cout << "Enemy destroyer destroyed!" << endl;
    }
    
    cout << "-----------------------------" << endl;
}

void userShipsNum ( grid water, int &xnum, int &ynum, int &userCarrier, int &userBattleship, int &userCruiser, int &userSubmarine, int &userDestroyer ) //tells you if computer destroyed your ship
{
    if (water[xnum - 1][ynum - 1][5] == '5')
    {
        userCarrier--;
    }
    else if (water[xnum - 1][ynum - 1][5] == '4')
    {
        userBattleship--;
    }
    else if (water[xnum - 1][ynum - 1][5] == '3')
    {
        userCruiser--;
    }
    else if (water[xnum - 1][ynum - 1][5] == '2')
    {
        userSubmarine--;
    }
    else if (water[xnum - 1][ynum - 1][5] == '1')
    {
        userDestroyer--;
    }
    
    display(water, 2);
    display(water, 0);
    
    cout << endl << "-----------------------------" << endl;
    
    if (userCarrier != 0 && userBattleship != 0 && userCruiser != 0 && userSubmarine != 0 && userDestroyer != 0)
    {
        cout << "All your ships are remaining" << endl;
    }
    
    if (userCarrier == 0)
    {
        cout << "Your Carrier is destroyed!" << endl;
    }
    
    if (userBattleship == 0)
    {
        cout << "Your battleship is destroyed!" << endl;
    }
    
    if (userCruiser == 0)
    {
        cout << "Your cruiser is destroyed!" << endl;
    }
    
    if (userSubmarine == 0)
    {
        cout << "Your submarine is destroyed!" << endl;
    }
    
    if (userDestroyer == 0)
    {
        cout << "Your destroyer is destroyed!" << endl;
    }
}

void header ( bool &again ) //header asks user if they would like to play the game
{
    char choice;
    
    cout << "Welcome to battleship! Would you like to play? (y/n): ";
    cin >> choice;
    
    if (choice == 'Y' || choice == 'y')
    {
        again = true;
        cout << "-------------------------------------------------------" << endl << "-------------------------------------------------------" << endl;
    }
    else if (choice == 'N' || choice == 'n')
    {
        again = false;
    }
    else
    {
        cout << "Input is invalid. Please try again." << endl << "Would you like to play? (y/n): ";
        cin >> choice;
    }
}

void winLoss () //going to do this function some other time
{
    int win = 0;
    int loss = 0;
    cout << endl << "-----------------" << endl;
    cout << "You won " << win << " times!" << endl;
    cout << "You lost " << loss << " times." << endl;
    cout << "-----------------" << endl << endl;
    
}

