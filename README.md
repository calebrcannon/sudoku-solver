# sudoku-solver
This is a student project Sudoku solver, which uses a CSV file containing a million puzzles sourced from: https://www.kaggle.com/bryanpark/sudoku

The puzzles in the CSV were generated using: https://www.ocf.berkeley.edu/~arel/sudoku/main.html

In order to use this code, you will need to download the CSV mentioned above. You'll also need to change the file path of the code to reflect where you saved the CSV, I've added
a comment to indicate where. 

The code reads through the puzzle, and attempts to find places where there is only one possible number to enter into an empty space. If the puzzle hasn't been changed since looking
through the puzzle for a space with only one possible answer, the code will move on to searching for naked pairs. Naked pairs are two sets of empty spaces in a row, column, or 
subgrid that have only two identical possible numbers. Once a naked pair is found, those two possbile numbers are removed from the rest of the row, column, or subgrid.  
