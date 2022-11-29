#include "Previous.h"

#include <iostream>
using namespace std;

Previous::Previous(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
}

bool Previous::dropACrumb(int r, int c)
{
    if (r < 1 || c < 1 || r > m_rows || c > m_cols) {
        return false;
    }

    int row = r - 1;
    int col = c - 1;

    if (board[row][col] < 65 || board[row][col] > 90) { //letters A to Z
        board[row][col] = 65;
    } else {
        board[row][col]++; //increment by 1. if A, change to B, and so on. 
    }

    return true;
}

void Previous::showPreviousMoves() const
{
    clearScreen();

    int r, c;
    char state;
    for (r = 0; r < m_rows; r++){

        for (c = 0; c < m_cols; c++) {

            if (board[r][c] <= 90 && board[r][c] >= 65) {
                state = board[r][c];
                cout << state;
            }
            else {
                cout << ".";
            }
        }
        cout << endl;
    }
    cout << endl;
}