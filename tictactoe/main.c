//
//  main.c
//  tictactoe
//
//  Created by Ben on 9/28/12.
//  Copyright (c) 2012 ben. All rights reserved.
//
/*
  0 | 1 | 2 
 -----------
  3 | 4 | 5
 -----------
  6 | 7 | 8
 
 win: 012,345,678,036,147,258,048,246
 board = ['0','1','2','3','4','5','6','7','8'];
 */
#include <stdio.h>
#include <stdlib.h>

//current game board
char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};

typedef struct best{
    int move;
    int score;
} best;

int numOfCharOnBoard(char check, char board[]){
    int count = 0;
    for(int i=0;i<9;i++){
        if(board[i]==check){
            count++;
        }
    }
    return count;
}

void printArray(char array[]){
    for(int i=0;i<=sizeof(*array)/sizeof(array[0]);i++) printf("%c ",array[i]);
}

char checkwin(char board[]){
    if(board[0] == board[1] && board[0] == board[2]) return board[0];
    else if(board[3] == board[4] && board[3] == board[5]) return board[3];
    else if(board[6] == board[7] && board[6] == board[8]) return board[6];
    else if(board[0] == board[3] && board[0] == board[6]) return board[0];
    else if(board[1] == board[4] && board[1] == board[7]) return board[1];
    else if(board[2] == board[5] && board[2] == board[8]) return board[2];
    else if(board[0] == board[4] && board[0] == board[8]) return board[0];
    else if(board[2] == board[4] && board[2] == board[6]) return board[2];
    else return ' ';
}

best chooseMove(char board[], int side){
    best myBest;
    best reply;
    
    //-----------------------------------
    
    //return terminal node value
    if(checkwin(board) == 'X'){         //X has won (human)
        myBest.move = ' ';
        myBest.score = -1;              //-1 for computer
//        printf("X wins terminal node reached\n");
        return myBest;
    }
    else if(checkwin(board) == 'O'){    //O has won (computer)
        myBest.move = ' ';
        myBest.score = 1;               //+1 for computer
//        printf("O wins terminal node reached\n");
        return myBest;
    }
    else if(numOfCharOnBoard(' ', board)==0){ //no empty slots, board is full
        myBest.move = ' ';
        myBest.score = 0;               //its a draw
//        printf("draw terminal node reached\n");
        return myBest;
    }
    
    //-----------------------------------
    
    
    //set the score to be worst possible
    if(side==1){    //side 1 = O , or computer
        myBest.score = -2;
    } else {        //side 0 = X , or human
        myBest.score = 2;
    }
    
    //-----------------------------------
    
    //recursively call chooseMove() until you get to a leaf
    for(int i=0;i<=(sizeof(board)/sizeof(board[0]));i++){   //for each item in the board
        if(board[i]==' '){                                      //if it is empty
            if (side==1){                                           //if computer turn
                board[i] = 'O';                                         //place O
//                printf("placed an O in position %i.recursing...\n",i);
                reply = chooseMove(board, 0);                         //recursive part (0 = X = human)
                board[i] = ' ';                                         //undo move
//                printf("move %i undone\n",i);
            } else {                                                //else (human turn)
                board[i] = 'X';                                         //place X
//                printf("placed an X in position %i.recursing...\n",i);
                reply = chooseMove(board, 1);                         //recursive part (1 = O = computer)
                board[i] = ' ';                                         //undo move
//                printf("move %i undone\n",i);
            }
            if ((side==1 && (reply.score>myBest.score)) || (side==0 && (reply.score<myBest.score))){    //if computer turn and we got a better move or
                myBest.move = i;                                                                        //if human turn and has move worse for computer
                myBest.score = reply.score;                                                             //set the move and the score (minimax value)
            }
        }
    }
    return myBest;                                      //return the best move
}

void printTTTBoard(char board[]){
    for(int i=0;i<=(sizeof(board)/sizeof(board[0]));i++){
        if(board[i] == ' ') printf(" %i ",i);
        else printf(" %c ",board[i]);
        if(i != 2 && i != 5 && i != 8) printf("|");
        if(i == 2 || i == 5) printf("\n------------\n");
    }
    printf("\n");
}

int isboardfull(char board[]){
    for(int i=0;i<=(sizeof(board)/sizeof(board[0]));i++){
        printf("checking position %i\n",i);
        if (board[i] == ' ') {
            return 0;      //loop through board until you encounter empty slot, then board isn't full --> return 0
            break;         //break out of function
        }
    }
    return 1;                               //board has been looped through with out an empty slot found so return 1
}

int main(int argc, const char * argv[])
{
    char move[] = "";
    printf("Welcome to Ben's Tic Tac Toe. (You can't win)\nBegin by placing your first move:\n\n");
    while(checkwin(board) == ' ' || isboardfull(board) == 0){ //should exit on a win or draw (board is full) --> keep running while checkwin doesn't return x or o, or while board has empty slots
        printTTTBoard(board);                       //print the board
        printf("enter your move: ");                //prompt the user
        scanf("%s",move);                           //get input
        if(board[atoi(move)] == ' '){               //if slot is empty
            board[atoi(move)] = 'X';                //place user move
            int compmove = chooseMove(board, 1).move;
            if(compmove == 32) printf("error. while loop should have ended\n");
            board[compmove] = 'O'; //place computer move
            printf("computer moved at %i\n", compmove);
        } else {
            printf("that spot is occupied\n");
        }
    }
    printf("end of while loop reached\n");
    printTTTBoard(board);
    if(checkwin(board) == 'X') printf("you won!\n");
    else if(checkwin(board) == 'O') printf("you lost!\n");
    else if(checkwin(board) == ' ') printf("draw");
    return 0;
    //side 1 = O , or computer
    //side 0 = X , or human
}