#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
int caculate_the_board(Board sandboard, Player player);
void algorithm_A(Board board, Player player, int index[])
{

    //////your algorithm design///////////
    static Board myboard;
    static Player me(RED);
    static Player rival(BLUE);
    static bool firstround = true;
    int row, col;
    int i, j;
    int mycolor = player.get_color();
    if (firstround)
    {
        firstround = false;
        // initial myboard
        for (i = 0; i < ROW; i++)
        {
            for (j = 0; j < COL; j++)
            {
                int n = board.get_orbs_num(i, j);
                int p = board.get_cell_color(i, j);
                for (int k = 0; k < n; k++)
                {
                    if (p == mycolor)
                        myboard.place_orb(i, j, &me);
                    // else if (p == 'w')
                    //     break;
                    else
                        myboard.place_orb(i, j, &rival);
                }
            }
        }
    }
    else {
        // update the board afer rival move
        myboard.place_orb(index[0], index[1], &rival);
        cout<< "after rival's play the board  is  "<< caculate_the_board(myboard, me);
    }
    // algorithm
    while (1)
    {
        row = rand() % 5;
        col = rand() % 6;
        if (board.get_cell_color(row, col) == mycolor || board.get_cell_color(row, col) == 'w')
            break;
    }
    index[0] = row;
    index[1] = col;
    myboard.place_orb(index[0], index[1], &me);
    cout<< " and after my play my board would be  "<< caculate_the_board(myboard, me) << endl;
    // myboard.print_current_board(row, col, -11111111);   
}
int caculate_the_board(Board sandboard, Player player) {
    int mycolor = player.get_color();
    int ihaveorb = 0, rivalhaveorb = 0;
    int score = 0;
    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++)
        {
            int n = sandboard.get_orbs_num(i, j);
            int color = sandboard.get_cell_color(i, j);
            if (color == mycolor) ihaveorb+=n;
            else if(color == 'w'){}
            else rivalhaveorb+=n;
        }
    }
    return ihaveorb - rivalhaveorb;
}