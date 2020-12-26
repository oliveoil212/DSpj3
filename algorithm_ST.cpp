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
int caculate_the_board_after_the_move(Board sandboard, Player player, const int row, int col);
int can_reach(int row, int col);
void algorithm_A(Board board, Player player, int index[])
{

    //////your algorithm design///////////
    static Board myboard;
    static Player me(RED);
    static Player rival(BLUE);
    static bool firstround = true;
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
        // cout<< "after rival's play the board  is  "<< caculate_the_board(myboard, me);
    }
    // algorithm
    int bestscore = -1000000000;
    int row, col; // location of best placement
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            int color = board.get_cell_color(i, j);
            if(color == mycolor || color == 'w') {
                int score = caculate_the_board_after_the_move(myboard, me, i, j);
                //  cout << "thinking " << i << ", "<< j <<"  score is " << score << endl;
                if (score > bestscore) {
                    // cout << "change mind form " << row << ". " << col <<"to "<< i << ". " << j ;
                    // cout << "==>  bestscore is " << bestscore <<"  score is " << score << endl;
                    row = i;
                    col =j;
                    bestscore = score;
                }
            }
        }
    }
    index[0] = row;
    index[1] = col;
    myboard.place_orb(index[0], index[1], &me);
    // cout<< " and after my play my board would be  "<< caculate_the_board(myboard, me) << endl;
    // myboard.print_current_board(row, col, -11111111);   
}
int caculate_the_board_after_the_move(Board sandboard, Player player, const int row, int col) {
    int mycolor = player.get_color();
    int ihaveorb = 0, rivalhaveorb = 0;
    int score = 0;
    int i, j;
    sandboard.place_orb(row, col, &player);
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++)
        {
            if(sandboard.get_cell_color(i, j) == mycolor) {
                ihaveorb+=sandboard.get_orbs_num(i, j);
                // traverse neighbors
                int dangerous = 1;
                for (int x = -1; x <= 1 ; x++)
                {
                    for(int y = -1; y <= 1; y++){
                        if(can_reach(i+x, j+y)){
                            if(!(sandboard.get_cell_color(i+x, j+y) == 'w' || sandboard.get_cell_color(i+x, j+y) == mycolor)){                            
                                if(sandboard.get_orbs_num(i+x, j+y) == sandboard.get_capacity(i+x, j+y) - 1){
                                    score -= 8 - sandboard.get_orbs_num(i+x, j+y);
                                    dangerous = -1;
                                } 
                            }
                        }
                    }
                }
                
                if(dangerous == 1) {      
                    score += 8 - sandboard.get_capacity(i, j);
                    if(sandboard.get_orbs_num(i, j) == sandboard.get_capacity(i, j) - 1){
                        score += 10;
                    }
                } 
                



            } else if(sandboard.get_cell_color(i, j) != 'w') {
                rivalhaveorb+= sandboard.get_orbs_num(i, j);
                int dangerous = 1;
                for (int x = -1; x <= 1 ; x++)
                {
                    for(int y = -1; y <= 1; y++){
                        if(can_reach(i+x, j+y)){
                            if(!(sandboard.get_cell_color(i+x, j+y) == 'w' || sandboard.get_cell_color(i+x, j+y) != mycolor)){                            
                                if(sandboard.get_orbs_num(i+x, j+y) == sandboard.get_capacity(i+x, j+y) - 1){
                                    score += 8 - sandboard.get_orbs_num(i+x, j+y);
                                    dangerous = -1;
                                } 
                            }
                        }
                    }
                }
                
                if(dangerous == 1) {      
                    score -= 8 - sandboard.get_capacity(i, j);
                    if(sandboard.get_orbs_num(i, j) == sandboard.get_capacity(i, j) - 1){
                        score -= 10;
                    }
                } 
            }
        }
    }
    if (rivalhaveorb == 0 && ihaveorb > 1) return 1999999999;
    return score;
}
int can_reach(int row, int col){
    if (row < 0 || row >= ROW) return false;
    if (col < 0 || col >= COL) return false;
    return true;
}