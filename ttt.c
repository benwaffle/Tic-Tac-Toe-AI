// vim: et:ts=4:sw=4:cc=120
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <err.h>
#include <glib.h>

typedef enum { HUMAN, CPU } player;
typedef struct { int move, score; } best;

unsigned count_char(int const n, char const board[n*n], char c) {
    unsigned count = 0;
    for(int i = 0; i < n*n; ++i)
        if(board[i] == c)
            count++;
    return count;
}

static inline bool board_is_full(int const n, char const board[n*n]) {
    return count_char(n, board, ' ') == 0;
}

static bool check_row(int const n, char const board[n*n], int row) {
    char c = board[row*n];
    if (c == ' ') return false;
    for (int i = 0; i < n; ++i)
        if (board[row*n + i] != c)
            return false;
    return true;
}

static bool check_col(int const n, char const board[n*n], int col) {
    char c = board[col];
    if (c == ' ') return false;
    for (int i = 0; i < n; ++i)
        if (board[col + i*n] != c)
            return false;
    return true;
}

static bool check_diag(int const n, char const board[n*n], bool dir) {
    char c = board[dir ? 0 : n-1];
    if (c == ' ') return false;
    for (int i = 0; i < n; ++i)
        if (board[i*n + (dir ? i : n-1-i)] != c)
            return false;
    return true;
}

char winner(int const n, char const board[n*n]) {
    for (int i = 0; i < n; ++i)
        if (check_row(n, board, i))
            return board[i*n];
        else if (check_col(n, board, i))
            return board[i];

    if (check_diag(n, board, true))
        return board[0];
    if (check_diag(n, board, false))
        return board[n-1];

    return ' ';
}

best minimax(int const n, char board[n*n], player turn, int alpha, int beta) {
    // terminal node 
         if (winner(n, board) == 'X')     return (best){ .move = -1, .score = -1 }; // I lose
    else if (winner(n, board) == 'O')     return (best){ .move = -1, .score = 1 };  // I win
    else if (board_is_full(n, board))     return (best){ .move = -1, .score = 0 };  // draw

    // worst possible score
    best my_best = {
        .score = (turn == CPU ? -2 : 2)
    };
    // recursively call minimax() until you get to a leaf
    for(int i = 0; i < n*n; ++i) {
        if (board[i] == ' ') { //for each available spot 
            best child;
            if (turn == CPU) {
                board[i] = 'O';
                child = minimax(n, board, HUMAN, alpha, beta);
                if (child.score > my_best.score)
                    my_best = (best){
                        .move = i,
                        .score = child.score
                    };
                if (my_best.score > alpha)
                    alpha = my_best.score;
            } else if (turn == HUMAN) {
                board[i] = 'X';
                child = minimax(n, board, CPU, alpha, beta);
                if (child.score < my_best.score)
                    my_best = (best){
                        .move = i,
                        .score = child.score
                    };
                if (my_best.score < beta)
                    beta = my_best.score;
            } else {
                g_assert_not_reached();
            }
            board[i] = ' ';
        }
    }
    return my_best; //return the best move
}

void printTTTBoard(int const n, char const board[n*n]) {
    #define RED "\x1b[41m"
    #define BLUE "\x1b[46m"
    #define RESET "\x1b[0m"

    g_autofree char *nxnstr = g_strdup_printf("%d", n*n);
    const int maxwidth = strlen(nxnstr);

    printf("\n");
    for(int i = 0; i < n*n; ++i) {
             if (board[i] == ' ') printf(" %*d ", maxwidth, i);
        else if (board[i] == 'X') printf(BLUE " X " RESET);
        else if (board[i] == 'O') printf(RED " O " RESET);
        else g_assert_not_reached();

        // ascii art
        if (i % n != n-1) {
            putchar('|');
        } else if (i / n != n-1) {
            putchar('\n');
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < maxwidth+2; ++k) putchar('-');
                if (j != n-1) putchar('+');
            }
            putchar('\n');
        }
    }
    printf("\n\n");
}

int main() {
    int n = 3;
    printf("enter game size: ");
    scanf("%d", &n);

    char board[n*n];
    memset(board, ' ', sizeof(board));

    player turn = HUMAN;
    while (winner(n, board) == ' ' && !board_is_full(n, board)) {
        if (turn == CPU) {
            board[minimax(n, board, 1, INT_MIN, INT_MAX).move] = 'O';
        } else if (turn == HUMAN) {
            printTTTBoard(n, board);
            printf("Enter your move: ");

            int move;
            if (scanf("%d", &move) != 1) {
                char garbage[64];
                fgets(garbage, sizeof garbage, stdin);
                fprintf(stderr, "bad input\n");
                continue;
            }

            if (move < 0 || move > n*n) {
                fprintf(stderr, "Out of bounds\n");
                continue;
            }

            if (board[move] != ' ') {
                fprintf(stderr, "That spot is occupied\n");
                continue;
            }

            board[move] = 'X';
        } else {
            g_assert_not_reached();
        }
        turn = (turn == HUMAN) ? CPU : HUMAN;
    }

    printTTTBoard(n, board);
         if (winner(n, board) == 'X') puts("you won!");
    else if (winner(n, board) == 'O') puts("you lost!");
    else                           puts("the game was a draw");
}
