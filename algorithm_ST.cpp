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
int minmax(Board sandboard, int depth, Player playerme, Player rival, bool isme, bool isfirststep);
void copy_board(Board &colone, Board &mother, Player me);
void algorithm_A(Board board, Player player, int index[])
{   
    char player1 = RED;
    char player2 = BLUE;
    if(player.get_color() == BLUE){
        player2 = RED;
        player1 = BLUE;
    }
    //////your algorithm design///////////
    static Board myboard;
    static Player me(player1);
    static Player rival(player2);
    static bool isfirstround = true;
    static bool isfirststep;
    if(isfirstround)printf("ST is %c and me is %c\n", player.get_color(), player1);
    int i, j;
    int mycolor = player.get_color();
    if (isfirstround)
    {
        isfirstround = false;
        isfirststep =true;
        copy_board(myboard, board, me);
    }
    else {
        // update the board afer rival move
        myboard.place_orb(index[0], index[1], &rival);
        // cout<< "after rival's play the board  is  "<< caculate_the_board(myboard, me);
    }
    // algorithm
    int bestscore = -2000010000;
    int row, col; // location of best placement
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            int color = board.get_cell_color(i, j);
            if(color == mycolor || color == 'w') {
                Board newboard;
                copy_board(newboard, myboard, player);
                newboard.place_orb(i, j, &player);
                int score = minmax(newboard, 0, player, rival, true, isfirststep);
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
    isfirststep = false;
}
int minmax(Board sandboard, int depth, Player playerme, Player rival, bool isme, bool isfirststep){
    if(sandboard.win_the_game(playerme) && !isfirststep) {if(isme)return 2000000000;return -2000000000;}
    if(sandboard.win_the_game(rival) && !isfirststep) {if(isme)return -2000000000;return 2000000000;}
    if(depth == 2) {
        if(isme) return caculate_the_board_after_the_move(sandboard, playerme, 0 ,0);
        else return -caculate_the_board_after_the_move(sandboard, rival, 0 ,0);
    }
    int mycolor = playerme.get_color();
    if(isme) {
        int bestscore = -1000000000;
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                int color = sandboard.get_cell_color(i, j);
                if(color == mycolor || color == 'w') {
                    Board newboard;
                    copy_board(newboard, sandboard, playerme);
                    newboard.place_orb(i, j, &playerme);
                    int score = minmax(newboard, depth + 1, playerme, rival, false, false);
                    bestscore = max(score, bestscore);
                }
            }
        }
        return bestscore;
    }
    else {
        int bestscore = 1000000000;
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                int color = sandboard.get_cell_color(i, j);
                if(color != mycolor || color == 'w') {
                    Board newboard;
                    copy_board(newboard, sandboard, playerme);
                    newboard.place_orb(i, j, &rival);
                    int score = minmax(newboard, depth + 1,playerme, rival, true, false);
                    bestscore = min(score, bestscore);
                }
            }
        }
        return bestscore;   
    }
}
int caculate_the_board_after_the_move(Board sandboard, Player player, const int row, int col) {
    int mycolor = player.get_color();
    int ihaveorb = 0, rivalhaveorb = 0;
    int score = 0;
    int i, j;
    // sandboard.place_orb(row, col, &player);
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
    // if (rivalhaveorb == 0 && ihaveorb > 1) return 1999999999;
    // else if(rivalhaveorb > 0 && ihaveorb == 0) return -199999999;
    return score;
}
int can_reach(int row, int col){
    if (row < 0 || row >= ROW) return false;
    if (col < 0 || col >= COL) return false;
    return true;
}
void copy_board(Board &colone, Board &mother, Player me){
    int mycolor = me.get_color();
    int i ,j;
    for (i = 0; i < ROW; i++)
        {
            for (j = 0; j < COL; j++)
            {
                int n = mother.get_orbs_num(i, j);
                int p = mother.get_cell_color(i, j);
                for (int k = 0; k < n; k++)
                {
                    if (p == mycolor)
                        colone.place_orb(i, j, &me);
                    // else if (p == 'w')
                    //     break;
                    else{
                        Player rival(p);
                        colone.place_orb(i, j, &rival);
                    }
                }
            }
        }
}