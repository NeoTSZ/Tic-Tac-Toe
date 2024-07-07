// Created by Earl James Williams Aliñgasa (NeoTSZ on GitHub).

// Header file libraries
#include <stdio.h>
#include <conio.h>

// Macros to move the cursor
#define REPOSITION "\x1b[2;2H"
#define UP "\x1b[1A"
#define DOWN "\x1b[1B"
#define RIGHT "\x1b[1C"
#define LEFT "\x1b[1D"

// Macros to place and remove the cursor
#define CURSOR SAVE PINK "{" RIGHT "}" LOAD RECOLOR
#define UNCURSOR SAVE "\xff" RIGHT "\xff" LOAD

// Macros to control the colors
#define RECOLOR "\x1b[0m"
#define RED "\x1b[38;5;9m"
#define GREEN "\x1b[38;5;10m"
#define YELLOW "\x1b[38;5;11m"
#define PINK "\x1b[38;5;13m"
#define CYAN "\x1b[38;5;14m"

// Macros to save and load cursor positions
#define SAVE "\x1b[s"
#define LOAD "\x1b[u"

// Macros to show and hide the cursor
#define SHOW "\x1b[?25h"
#define HIDE "\x1b[?25l"

// Other macros
#define CLEAR "\x1b[2J\x1b[H"
#define BUTTON(X,Y) CYAN #X RECOLOR "\xff" #Y "\xff\xff\xff"

// Macros to hold the lines of the grid
#define TOP "\xc9\xcd\xcd\xcd\xd1\xcd\xcd\xcd\xd1\xcd\xcd\xcd\xbb\xff" SAVE "\n"
#define BOTTOM "\xc8\xcd\xcd\xcd\xcf\xcd\xcd\xcd\xcf\xcd\xcd\xcd\xbc\n"
#define CELLS "\xba\xff\xff\xff\xb3\xff\xff\xff\xb3\xff\xff\xff\xba\n"
#define BORDER "\xc7\xc4\xc4\xc4\xc5\xc4\xc4\xc4\xc5\xc4\xc4\xc4\xb6\n"

// Macros for quick printing
#define GRID CLEAR TOP CELLS BORDER CELLS BORDER CELLS BOTTOM
#define CONTROLS_A LOAD DOWN SAVE BUTTON([W],Up) BUTTON([S],Down)
#define CONTROLS_B BUTTON([D],Right) BUTTON([A],Left) LOAD DOWN SAVE
#define CONTROLS_C BUTTON([N],Place X/O) BUTTON([R],Restart)
#define CONTROLS_D BUTTON([E],Exit) LOAD DOWN DOWN SAVE
#define X_TURN SAVE "\x1b[5;15H" RED "X's turn." RECOLOR LOAD
#define O_TURN SAVE "\x1b[5;15H" RED "O's turn." RECOLOR LOAD
#define PUT_X SAVE RIGHT GREEN "X" RECOLOR LOAD
#define PUT_O SAVE RIGHT YELLOW "O" RECOLOR LOAD
#define X_WIN SAVE "\x1b[5;15H" PINK "X wins!\xff\xff" RECOLOR LOAD
#define O_WIN SAVE "\x1b[5;15H" PINK "O wins!\xff\xff" RECOLOR LOAD
#define DRAW SAVE "\x1b[5;15H" PINK "It's a draw!" RECOLOR LOAD

// Structure to hold the state of the game
typedef struct Game
{
    int grid[ 9 ];
    int cursor;
    int turn;
    int turnCount;
}
Game;

// Prototypes
void initializeGame( Game* game );
void drawGrid();
int processInput( char input, Game* game );
void moveCursor( int move, Game* game );
void putXO( Game* game );
int isMatching( int* grid, int value1, int value2, int value3, int turn );
void checkGame( int turn, Game* game );

int main( void )
{
    Game game;

    // Setting up the game and grid
    initializeGame( &game );
    drawGrid();

    // Looping the game
    while ( 1 )
    {
        // Processing user inputs
        if ( _kbhit() )
        {
            int quitGame = processInput( getch(), &game );

            if ( quitGame )
            {
                printf( "\x1b[9;1H" ":: GAME OVER ::\n\n" SHOW );
                return 0;
            }
        }
    }
}

void initializeGame( Game* game )
{
    // Initializing the cursor and turn indicator
    // 1: X
    // 2: O
    game->cursor = 0;
    game->turn = 1;
    game->turnCount = 0;

    // Initializing the cell values
    for ( int i = 0; i < 9; i ++ )
    {
        *(game->grid + i) = 0;
    }

    return;
}

void drawGrid()
{
    // Printing the grid and controls
    printf( GRID CONTROLS_A CONTROLS_B CONTROLS_C CONTROLS_D X_TURN );
    printf( REPOSITION HIDE CURSOR );

    return;
}

int processInput( char input, Game* game )
{
    // This function returns 1 if the user ends the game

    switch ( input )
    {
        // Moving the cursor up
        case 'w': case 'W': case '5':
            moveCursor( 5, game );
            break;

        // Moving the cursor down
        case 's': case 'S': case '2':
            moveCursor( 2, game );
            break;

        // Moving the cursor right
        case 'd': case 'D': case '3':
            moveCursor( 3, game );
            break;

        // Moving the cursor left
        case 'a': case 'A': case '1':
            moveCursor( 1, game );
            break;

        // Placing X/O
        case 'n': case 'N':
            putXO( game );
            break;

        // Restarting the game
        case 'r': case 'R':
            initializeGame( game );
            drawGrid();
            break;

        // Exiting the game
        case 'e': case 'E':
            return 1;
    }

    return 0;
}

void moveCursor( int move, Game* game )
{
    int temp = game->cursor;

    // Updating the location of the cursor
    switch ( move )
    {
        // Moving the cursor up
        case 5:
            if ( temp > 2 )
            {
                temp -= 3;
                printf( UNCURSOR UP UP CURSOR );
            }
            break;

        // Moving the cursor down
        case 2:
            if ( temp < 6 )
            {
                temp += 3;
                printf( UNCURSOR DOWN DOWN CURSOR );
            }
            break;

        // Moving the cursor right
        case 3:
            if ( temp % 3 != 2 )
            {
                temp ++;
                printf( UNCURSOR RIGHT RIGHT RIGHT RIGHT CURSOR );
            }
            break;

        // Moving the cursor left
        case 1:
            if ( temp % 3 != 0 )
            {
                temp --;
                printf( UNCURSOR LEFT LEFT LEFT LEFT CURSOR );
            }
            break;
    }

    game->cursor = temp;

    return;
}

void putXO( Game* game )
{
    // Checking if the cursor's cell is occupied
    if ( *(game->grid + game->cursor) )
    {
        return;
    }

    // Proceeding if empty
    game->turnCount += 1;
    if ( game->turn == 1 )
    {
        *(game->grid + game->cursor) = 1;
        printf( PUT_X O_TURN );
        game->turn = 2;
        checkGame( 1, game );
    }
    else
    {
        *(game->grid + game->cursor) = 2;
        printf( PUT_O X_TURN );
        game->turn = 1;
        checkGame( 2, game );
    }

    return;
}

int isMatching( int* grid, int value1, int value2, int value3, int turn )
{
    if ( ( *(grid + value1) == turn ) && ( *(grid + value2) == turn ) && ( *(grid + value3) == turn ) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void checkGame( int turn, Game* game )
{
    int* g = game->grid;

    // Checking for matching X/O patterns
    if (
        /* Checking the rows */
        isMatching( g, 0, 1, 2, turn ) ||
        isMatching( g, 3, 4, 5, turn ) ||
        isMatching( g, 6, 7, 8, turn ) ||

        /* Checking the columns */
        isMatching( g, 0, 3, 6, turn ) ||
        isMatching( g, 1, 4, 7, turn ) ||
        isMatching( g, 2, 5, 8, turn ) ||

        /* Checking the diagonals */
        isMatching( g, 0, 4, 8, turn ) ||
        isMatching( g, 2, 4, 6, turn )
    )
    {
        if ( turn == 1 )
        {
            printf( X_WIN );
        }
        else
        {
            printf( O_WIN );
        }

        // Blocking all remaining empty cells
        for ( int i = 0; i < 9; i ++ )
        {
            *(game->grid + i) += ( *(game->grid + i) ) ? 0 : 3;
        }

        return;
    }

    // Checking the draw condition
    if ( game->turnCount == 9 )
    {
        printf( DRAW );
    }

    return;
}
