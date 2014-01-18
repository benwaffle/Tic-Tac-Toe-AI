//  just adding a comment
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

#define COMPUTER 1
#define HUMAN 0
#define SIZE 9
//current game board
 char board[SIZE] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};

 typedef struct best{
 	int move;
 	int score;
 } best;

 int numOfCharOnBoard(char check, char board[]){
 	int count = 0, i = 0;
 	for(; i < SIZE; i++) if(board[i] == check) count++;
 		return count;
 }

 void printArray(char array[]){
 	int i = 0;
 	for(; i <= sizeof(array)/sizeof(array[0]); i++) printf("%c ",array[i]);
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

    //return terminal node value
    if(checkwin(board) == 'X'){         //X has won (human)
    	myBest.move = ' ';
        myBest.score = -1;              //-1 for computer
        return myBest;
    } else if(checkwin(board) == 'O'){    //O has won (computer)
    	myBest.move = ' ';
        myBest.score = 1;               //+1 for computer
        return myBest;
    } else if(numOfCharOnBoard(' ', board) == 0){ //no empty slots, board is full
    	myBest.move = ' ';
        myBest.score = 0;               //its a draw
        return myBest;
    }
    
    //set the score to be worst possible
    myBest.score = (side == COMPUTER ? -2 : 2);
    
    //recursively call chooseMove() until you get to a leaf
    int i = 0;
    for(; i <= SIZE; i++) if(board[i] == ' '){ //for each available spot
    	if (side == COMPUTER){
    		board[i] = 'O';
                reply = chooseMove(board, 0); //recurse
                board[i] = ' ';
            } else {   
            	board[i] = 'X';
                reply = chooseMove(board, 1); //recurse
                board[i] = ' ';
            }
            // if we found a better score
            if ((side == COMPUTER && reply.score > myBest.score) || (side == HUMAN && reply.score < myBest.score)){   
            	myBest.move = i;
                myBest.score = reply.score; //set the move and the score (minimax value)
            }
        }
    return myBest; //return the best move
}

void printTTTBoard(char board[]){
	int i = 0;
	for(; i <= SIZE; i++){
		if(board[i] == ' ') printf(" %i ",i);
		else printf(" %c ", board[i]);
		if(i != 2 && i != 5 && i != 8) printf("|");
		if(i == 2 || i == 5) printf("\n------------\n");
	}
	printf("\n");
}

int isboardfull(char board[]){
	int i = 0;
	for(; i <= SIZE; i++) if(board[i] == ' ') return 0;
	return 1;
}

int main(int argc, char *argv[]){
	printf("Welcome to Ben's Tic Tac Toe. (You can't win)\n");

	int move;
	while(checkwin(board) == ' ' && isboardfull(board) == 0){
		printf("\n");
		printTTTBoard(board);
		printf("enter your move: ");
		scanf("%d", &move);

		// if(board[move] == ' '){
		// 	board[move] = 'X';
		// 	board[chooseMove(board, 1).move] = 'O';
		// } else printf("that spot is occupied\n");
	}

	printTTTBoard(board);
	if (checkwin(board) == 'X') printf("you won!\n");
	else if (checkwin(board) == 'O') printf("you lost!\n");
	else if (checkwin(board) == ' ') printf("the game was a draw");

	return 0;
}