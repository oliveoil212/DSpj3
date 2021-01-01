#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

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
int caculate_the_board(Board &sandboard);
int can_reach(int row, int col);
int minmax(Board sandboard, int depth, bool ismyturn);
int omaewashiteru(Board sandboard);
void copy_board(Board &colone, Board mother, Player me);
void algorithm_A(Board board, Player player, int index[])
{   
    auto start_time = Clock::now();
    
    //////your algorithm design///////////
    static Player me(RED);
    static Player rival(BLUE);
    // static bool isfirstround = true;
    int i, j;
    Board myboard; // On my board, red is me!
    int bestscore = -2000010000;
    copy_board(myboard, board, player);
    int row = -1, col = -1; // location of best placement
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            char color = myboard.get_cell_color(i, j);
            if(color != BLUE) {
                Board newboard;
                copy_board(newboard, board, player);
                newboard.place_orb(i, j, &me);
                int score = minmax(newboard, 1, false);
                if (score >= bestscore) {
                    row = i;
                    col =j;
                    bestscore = score;
                }
            }
        }
    }
    index[0] = row;
    index[1] = col;
    auto end_time = Clock::now();
    int dura = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()/1000000;
    std::cout << "Time difference: of a move"
              << dura << " nanoseconds" << std::endl;
    if(dura > 1000){
        cout << "time exceed" << endl;
        exit(1);
    }
}
int minmax(Board sandboard, int depth, bool ismyturn){
    Player me(RED);
    Player rival(BLUE);
    // sandboard.print_current_board(depth,123,123);
    int areuwiningson = omaewashiteru(sandboard);
    if(areuwiningson == 1) return 1999999999;
    else if(areuwiningson == -1) return -1999999999;
    if(depth == 4) {
         return caculate_the_board(sandboard);
    }
    if(ismyturn) {
        int bestscore = -2000000000;
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                char color = sandboard.get_cell_color(i, j);
                if(color != BLUE)  {
                    Board newboard;
                    copy_board(newboard, sandboard, me);
                    newboard.place_orb(i, j, &me);
                    int score = minmax(newboard, depth + 1, false);
                    bestscore = max(score, bestscore);
                }
            }
        }
        return bestscore;
    }
    else {
        int bestscore = 2000000000;
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                char color = sandboard.get_cell_color(i, j);
                if(color != RED) {
                    Board newboard;
                    copy_board(newboard, sandboard, me);
                    newboard.place_orb(i, j, &rival);
                    int score = minmax(newboard, depth + 1, true);
                    bestscore = min(score, bestscore);
                }
            }
        }
        return bestscore;   
    }
}
int caculate_the_board(Board &sandboard) {
    int ihaveorb = 0, rivalhaveorb = 0;
    int score = 0;
    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++)
        {
            char color = sandboard.get_cell_color(i, j);
            if(color == RED) {
                ihaveorb+=sandboard.get_orbs_num(i, j);
                // traverse neighbors
                int dangerous = 1;
                for (int x = -1; x <= 1 ; x++)
                {
                    for(int y = -1; y <= 1; y++){
                        if(can_reach(i+x, j+y)){
                            char neighbor_color = sandboard.get_cell_color(i+x, j+y);
                            if(neighbor_color == BLUE){                            
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
            } else if(color == BLUE) {
                rivalhaveorb+= sandboard.get_orbs_num(i, j);
                int dangerous = 1;
                for (int x = -1; x <= 1 ; x++)
                {
                    for(int y = -1; y <= 1; y++){
                        if(can_reach(i+x, j+y)){
                            char neighbor_color = sandboard.get_cell_color(i+x, j+y);
                            if(neighbor_color == RED){                            
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
    if (rivalhaveorb == 0 && ihaveorb > 1) return 1999999989;
    else if(rivalhaveorb > 0 && ihaveorb == 0) return -199999989;
    return score + ihaveorb - rivalhaveorb;
}
int can_reach(int row, int col){
    if (row < 0 || row >= ROW) return false;
    if (col < 0 || col >= COL) return false;
    return true;
}
void copy_board(Board &colone, Board mother, Player me){
    Player I(RED);
    Player rival(BLUE);
    char mycolor = me.get_color();
    int i ,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            int n = mother.get_orbs_num(i, j);
            char p = mother.get_cell_color(i, j);
            for (int k = 0; k < n; k++)  {
                if (p == mycolor)
                    colone.place_orb(i, j, &I);
                else if (p != 'w')
                    colone.place_orb(i, j, &rival); 
            }
        }
    }
}
int omaewashiteru(Board sandboard) {
    int myorbs = 0, notmyorbs =0;
    int i , j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            int n = sandboard.get_orbs_num(i, j);
            int p = sandboard.get_cell_color(i, j);
            if (p == RED)
                myorbs += n;
            else if(p == BLUE)
                notmyorbs += n;
        }
    }
    if(myorbs > 0 && notmyorbs == 0) return 1;
    if(notmyorbs > 0 && myorbs == 0) return -1;
    return 0;
}