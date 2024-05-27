/*
Developed by Earl James Williams Aliñgasa (NeoTSZ on GitHub).

Controls:
    (W) Up
    (S) Down
    (D) Right
    (A) Left
    (N) Put X/O
    (E) Exit
*/



//// HEADER FILE lIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>



//// CONTROL MACROS
#define CELL_COUNT      9
#define CSI(SEQ)        "\x1B[" #SEQ    // Control Sequence Introducer
#define FG(SEQ)         CSI(38;5;) #SEQ // Foreground Color Setter
#define RESET_STYLE     CSI(0m)
#define CLEAR_SCREEN    CSI(2J) CSI(H)

//// COLOR MACROS
#define RED             FG(1m)
#define GRN             FG(10m)         // Green
#define YLW             FG(11m)         // Yellow
#define PNK             FG(13m)         // Pink
#define CYN             FG(14m)         // Cyan

//// CURSOR MACROS
#define SHOW_CURSOR     CSI(?25h)
#define HIDE_CURSOR     CSI(?25l)
#define RESET_CURSOR    CSI(2;2H)
#define REMOVE_CURSOR   CSI(s) " " CSI(1C) " " CSI(u)
#define PLACE_CURSOR    CSI(s) GRN ">" CSI(1C) "<" CSI(u) RESET_STYLE
#define MOVE_UP         CSI(2A)
#define MOVE_DOWN       CSI(2B)
#define MOVE_RIGHT      CSI(4C)
#define MOVE_LEFT       CSI(4D)

//// TURN-BASED MACROS
#define PLACE_X         CSI(s) PNK CSI(1C) "X" CSI(u) RESET_STYLE
#define PLACE_O         CSI(s) YLW CSI(1C) "O" CSI(u) RESET_STYLE
#define X_TURN          CSI(s) CSI(9;1H) GRN "X's TURN" RESET_STYLE CSI(u)
#define O_TURN          CSI(s) CSI(9;1H) GRN "O's TURN" RESET_STYLE CSI(u)
#define X_WIN           CSI(s) CSI(9;1H) GRN "X WINS! " RESET_STYLE CSI(u)
#define O_WIN           CSI(s) CSI(9;1H) GRN "O WINS! " RESET_STYLE CSI(u)
#define DRAW            CSI(s) CSI(9;1H) GRN "GAME DRAW!" RESET_STYLE CSI(u)
#define EXIT_PROMPT     CSI(11;1H) RED "GAME OVER!\n" RESET_STYLE "Press ENTER to close the terminal."

//// BOARD MACROS
#define TOP_BORDER      "\xC9\xCD\xCD\xCD\xD1\xCD\xCD\xCD\xD1\xCD\xCD\xCD\xBB\n"
#define CELL_LINE       "\xBA\xFF\xFF\xFF\xB3\xFF\xFF\xFF\xB3\xFF\xFF\xFF\xBA\n"
#define INNER_BORDER    "\xC7\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xB6\n"
#define BOTTOM_BORDER   "\xC8\xCD\xCD\xCD\xCF\xCD\xCD\xCD\xCF\xCD\xCD\xCD\xBC\n"



//// PROTOTYPES
void draw_board();
void process_input(char input, int* cursor, int* turn, int* turnCount, int* cells);
int move_cursor(int move, int cursor);
int place_xo(int* cursor, int* turn, int* turnCount, int* cells);
void check_game_state(int turnCount, int* cells);
void restart_game(int* cursor, int* turn, int* turnCount, int* cells);
void end_game();



int main(void)
{
    int cursor = 0;
    int turn = 1;
    int turnCount = 0;
    int cells[CELL_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    draw_board();

    // Looping game
    while (1)
    {
        if (_kbhit())
        {
            process_input(getch(), &cursor, &turn, &turnCount, cells);
        }
    }
}



//// DEFINITIONS
void draw_board()
{
    // Clearing screen and printing board
    printf(CLEAR_SCREEN);
    printf(TOP_BORDER CELL_LINE INNER_BORDER CELL_LINE INNER_BORDER CELL_LINE BOTTOM_BORDER);

    // Printing turn information and controls
    printf(X_TURN);
    printf(CSI(1;17H) CYN "(W) " RESET_STYLE "Move Up");
    printf(CSI(2;17H) CYN "(S) " RESET_STYLE "Move Down");
    printf(CSI(3;17H) CYN "(D) " RESET_STYLE "Move Right");
    printf(CSI(4;17H) CYN "(A) " RESET_STYLE "Move Left");
    printf(CSI(5;17H) CYN "(N) " RESET_STYLE "Put X/O");
    printf(CSI(6;17H) CYN "(R) " RESET_STYLE "Restart Game");
    printf(CSI(7;17H) CYN "(E) " RESET_STYLE "Exit Game");

    // Resetting cursor
    printf(RESET_CURSOR PLACE_CURSOR HIDE_CURSOR);

    return;
}



void process_input(char input, int* cursor, int* turn, int* turnCount, int* cells)
{
    // Processing user input
    switch (input)
    {

    case 'w': case 'W': case '5': // Move the cursor up
        *cursor = move_cursor(5, *cursor);
        break;

    case 's': case 'S': case '2': // Move the cursor down
        *cursor = move_cursor(2, *cursor);
        break;

    case 'd': case 'D': case '3': // Move the cursor right
        *cursor = move_cursor(3, *cursor);
        break;

    case 'a': case 'A': case '1': // Move the cursor left
        *cursor = move_cursor(1, *cursor);
        break;

    case 'n': case 'N': // Place X/O according to turn
        if (place_xo(cursor, turn, turnCount, cells))
        {
            check_game_state(*turnCount, cells);
        }
        break;

    case 'r': case 'R': // Restart the game
        restart_game(cursor, turn, turnCount, cells);
        break;

    case 'e': case 'E': // Exit the game
        end_game();
        break;

    }

    return;
}



int move_cursor(int move, int cursor)
{
    // Removing cursor
    printf(REMOVE_CURSOR);

    // Processing movement
    switch(move)
    {
    case 5: // Move up
        if (cursor > 2)
        {
            cursor -= 3;
            printf(MOVE_UP);
        }
        break;
    case 2: // Move down
        if (cursor < 6)
        {
            cursor += 3;
            printf(MOVE_DOWN);
        }
        break;
    case 3: // Move right
        if (cursor % 3 != 2)
        {
            cursor ++;
            printf(MOVE_RIGHT);
        }
        break;
    case 1: // Move left
        if (cursor % 3 != 0)
        {
            cursor --;
            printf(MOVE_LEFT);
        }
        break;
    }

    // Placing cursor back
    printf(PLACE_CURSOR);

    return cursor;
}



int place_xo(int* cursor, int* turn, int* turnCount, int* cells)
{
    // Checking if the current cell is occupied
    if (*(cells + *cursor))
    {
        return 0;
    }

    // Proceeding if the current cell is empty
    *turnCount += 1;
    if (*turn == 1)
    {
        // X's turn
        *(cells + *cursor) = 1;
        *turn = 2;
        printf(PLACE_X);
        printf(O_TURN);
    }
    else
    {
        // O's turn
        *(cells + *cursor) = 2;
        *turn = 1;
        printf(PLACE_O);
        printf(X_TURN);
    }

    return 1;
}



void check_game_state(int turnCount, int* cells)
{
    int winFlag = 0;

    // Checking both X and O for win states
    for (int turn = 1; turn < 3; turn ++)
    {
        if (
            (*(cells + 0) == turn && *(cells + 1) == turn && *(cells + 2) == turn) ||
            (*(cells + 3) == turn && *(cells + 4) == turn && *(cells + 5) == turn) ||
            (*(cells + 6) == turn && *(cells + 7) == turn && *(cells + 8) == turn) ||
            (*(cells + 0) == turn && *(cells + 3) == turn && *(cells + 6) == turn) ||
            (*(cells + 1) == turn && *(cells + 4) == turn && *(cells + 7) == turn) ||
            (*(cells + 2) == turn && *(cells + 5) == turn && *(cells + 8) == turn) ||
            (*(cells + 0) == turn && *(cells + 4) == turn && *(cells + 8) == turn) ||
            (*(cells + 2) == turn && *(cells + 4) == turn && *(cells + 6) == turn)
        )
        {
            winFlag = turn;
            break;
        }
    }

    // Processing game state
    if (winFlag == 1)
    {
        printf(X_WIN);
        end_game();
    }
    else if (winFlag == 2)
    {
        printf(O_WIN);
        end_game();
    }
    else if (turnCount == 9)
    {
        printf(DRAW);
        end_game();
    }

    return;
}



void restart_game(int* cursor,int* turn,int* turnCount,int* cells)
{
    // Drawing empty board
    draw_board();

    // Resetting variables
    *cursor = 0;
    *turn = 1;
    *turnCount = 0;

    // Resetting cells
    for (int i = 0; i < CELL_COUNT; i ++)
    {
        *(cells + i) = 0;
    }

    return;
}



void end_game()
{
    fflush(stdin);
    printf(EXIT_PROMPT);
    getchar();
    printf(CSI(13;1H) SHOW_CURSOR);
    exit(0);
}