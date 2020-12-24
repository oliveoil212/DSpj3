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


void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    Board sandTable;
    Player me(RED);
    Player rival(BLUE);

    int mycolor = player.get_color();
    // duplicate the current board
   for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            int n = board.get_orbs_num(i, j);
            int p = board.get_cell_color(i, j);
            for(int k = 0; k < n; k++) {
                if(p == mycolor)
                    sandTable.place_orb(i, j, &me);
                else
                    sandTable.place_orb(i, j, &rival);
            }
        }
   }
   // consider each legal movem
    int topscore = 0;
    bool knowsomething = false;
   for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if (board.get_cell_color(i, j) == mycolor) { // if it would be a legal move
                int score = caculate_the_score_of_this_move(sandTable, i, j, me);
                if(knowsomething && score >= topscore)
                {
                    topscore = score;
                    index[0] = i;
                    index[1] = j;
                }
                else if(!knowsomething) { // hajimede
                    topscore = score;
                    knowsomething = true;
                }
            }
        }
   }
    
}

int caculate_the_score_of_this_move (Board sandtable, int i, int j, Player player) {
    int score; // the higer, the better for me
    bool win = false;
    sandtable.place_orb(i, j, &player);
    ///// some formula the caculate the score of this move ////// 
    return score;
}