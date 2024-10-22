#include <stdbool.h>
#include <stdio.h>

// Algorithm by Niklaus Wirth, original code by Deepank Pant

const unsigned int N = 8;

void printSolution(bool board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", board[i][j] ? 'Q' : '-');
        }
        printf("\n");
    }
}

bool isSafe(bool board[N][N], int row, int col) {
    int i, j;
    // Check rows
    for (i = 0; i < col; i++) {
        if (board[row][i]) {
            return false;
        }
    }
    // Check upper diagonal
    for (i = row, j = col; i >= 0 && j >= 0; i -= 1, j -= 1) {
        if (board[i][j]) {
            return false;
        }
    }
    // Check lower diagonal
    for (i = row, j = col; j >= 0 && i < N; i += 1, j -= 1) {
        if (board[i][j]) {
            return false;
        }
    }
    return true; // No conflicts!
}

bool solveNQP(bool board[N][N], int col) {
    if (col >= N) {
        return true;
    }
    // Placing the queen in each row
    for (int i = 0; i < N; i++) {
        // Did that work?
        if (isSafe(board, i, col)) {
            // OK, do it!
            board[i][col] = true;
            // Recurse to place the others
            if (solveNQP(board, col + 1)) {
                return true;
            }
            board[i][col] = false; // Did not work back up!
        }
    }
    // No solution found!
    return false;
}

int main(void) {
    bool board[N][N] = { false };
    if (solveNQP(board, 0)) {
        printSolution(board);
        return 0;
    } else {
        printf("Solution does not exist.\n");
        return 1;
    }
}
