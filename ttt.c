#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef enum {
    human, cpu
} player;

#define SIZE 9
char board[SIZE] = {
    ' ', ' ', ' ',
    ' ', ' ', ' ',
    ' ', ' ', ' '
};

typedef struct {
    int move;
    int score;
} best;

unsigned count_char(char board[static SIZE], char c) {
    unsigned count = 0;
    for(int i = 0; i < SIZE; ++i)
        if(board[i] == c)
            count++;
    return count;
}

void print_array(char array[static SIZE]) {
    for(int i = 0; i < SIZE; ++i)
        printf("%c ", array[i]);
}

char winner(char board[static SIZE]) {
         if (board[0] == board[1] && board[0] == board[2]) return board[0];
    else if (board[3] == board[4] && board[3] == board[5]) return board[3];
    else if (board[6] == board[7] && board[6] == board[8]) return board[6];
    else if (board[0] == board[3] && board[0] == board[6]) return board[0];
    else if (board[1] == board[4] && board[1] == board[7]) return board[1];
    else if (board[2] == board[5] && board[2] == board[8]) return board[2];
    else if (board[0] == board[4] && board[0] == board[8]) return board[0];
    else if (board[2] == board[4] && board[2] == board[6]) return board[2];
    else                                                   return ' ';
}

best choose_move(char board[static SIZE], player p) {
    // terminal node 
         if (winner(board) == 'X')        return (best){ .move = ' ', .score = -1 }; // I lose
    else if (winner(board) == 'O')        return (best){ .move = ' ', .score = 1 };  // I win
    else if (count_char(board, ' ') == 0) return (best){ .move = ' ', .score = 0 };  // draw

    // worst possible score
    best my_best = {
        .score = (p == cpu ? -2 : 2)
    };
    // recursively call choose_move() until you get to a leaf
    for(int i = 0; i < SIZE; ++i)
        if (board[i] == ' ') { //for each available spot
            best child;
            if (p == cpu) {
                board[i] = 'O';
                child = choose_move(board, human);
                board[i] = ' ';
            } else {
                board[i] = 'X';
                child = choose_move(board, cpu);
                board[i] = ' ';
            }
            // if we found a better score
            if ((p == cpu   && child.score > my_best.score) ||
                (p == human && child.score < my_best.score)) {
                my_best = (best){
                    .move = i,
                    .score = child.score
                };
            }
        }
    return my_best; //return the best move
}

void printTTTBoard(char board[static SIZE]) {
    #define RED "\x1b[41m"
    #define BLUE "\x1b[46m"
    #define RESET "\x1b[0m"

    printf("\n");
    for(int i = 0; i < SIZE; ++i) {
        if (board[i] == ' ')      printf(" %i ", i);
        else if (board[i] == 'X') printf(BLUE " X " RESET);
        else                      printf(RED " O " RESET);

        // ascii art
        if (i % 3 != 2) putchar('|');
        if (i == 2 || i == 5) printf("\n------------\n");
    }
    printf("\n\n");
}

bool board_is_full(char board[static SIZE]) {
    return count_char(board, ' ') == 0;
}

int main() {
    printf("Welcome to Ben's Tic Tac Toe. (You can't win)\n");

    bool cpu_turn = false;
    while(winner(board) == ' ' && !board_is_full(board)) {
        if (cpu_turn) {
            board[choose_move(board, 1).move] = 'O';
        } else {
            printTTTBoard(board);
            printf("enter your move: ");

            int move;
            if (scanf("%d", &move) != 1) {
                char garbage[64];
                fgets(garbage, sizeof garbage, stdin);
                puts("bad input");
                continue;
            }

            if(board[move] == ' '){
                board[move] = 'X';
            } else {
                puts("that spot is occupied");
                continue;
            }
        }
        cpu_turn = !cpu_turn;
    }

    printTTTBoard(board);
    if (winner(board) == 'X') puts("you won!");
    else if (winner(board) == 'O') puts("you lost!");
    else puts("the game was a draw");
}
