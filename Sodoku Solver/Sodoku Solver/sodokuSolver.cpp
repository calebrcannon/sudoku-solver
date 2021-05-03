
#define _CRT_SECURE_NO_WARNINGS
#define WIDTH_OF_ONE_PUZZLE 81
#define WIDTH_OF_ONE_PUZZLE_PLUS_SPACE 82
#define WIDTH_OF_UNFINISHED_SOLVED_AND_SPACES 164

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initializePossibleNumbersArray(int possibleNumbersForSpace[][9][9]);
void initializeCurrentPuzzleTwoDimensionsArray(int currentPuzzleTwoDimensions[][9], char currentUnsolvedPuzzle[]);
void initializeAnswerOfCurrentPuzzleArray(int answerTwoDimensions[][9], char answerOfCurrentPuzzle[]);
void printArray(int anyArray[][9]);
void updateCurrentPuzzlePossibleNumbers(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9]);
void updatePossibleNumsInColumn(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int columnIndex);
void updatePossibleNumsInRow(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int rowIndex);
void updatePossibleNumsInSubGrid(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int rowIndex, int columnIndex);
void createSnapshotOfCurrentPuzzle(int currentPuzzleTwoDimensions[][9], int snapShot[][9]);
void insertSinglePossibleNumbers(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9]);
bool isSnapshotChanged(int currentPuzzleTwoDimensions[][9], int snapShot[][9]);
void printPossibleNumbersArray(int possibleNumbersForSpace[][9][9]);
bool isCurrentPuzzleSolved(int currentPuzzleTwoDimensions[][9], int answerTwoDimensions[][9]);
void searchForNakedPairs(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9]);
void searchForNakedPairsHorizontally(int currenPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9]);
void searchForNakedPairsVertically(int possibleNumbersForSpace[][9][9]);
void searchForSecondNakedPair(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairColumnIndex, int firstPairsFirstNumber, int firstPairsSecondNumber);
void searchForSecondNakedPairsVertically(int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairColumnIndex, int firstNumInPair, int secondNumInPair);
void removeNumsInNakedPairFromRow(int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairsFirstNumber, int firstPairsSecondNumber, int firstPairColumnIndex, int secondPairColumnIndex);
bool areOtherPossibleNumbersEmpty(int possibleNumbersForSpace[][9][9], int firstPairFirstNumber, int firstPairSecondNumber, int firstPairRowIndex, int firstPairColumnIndex, int secondPairsColumnIndex);
bool areOtherPossibleNumbersEmptyVertically(int possibleNumbersForSpace[][9][9], int firstNumInPair, int secondNumInPair, int firstPairRowIndex, int firstPairColumnIndex, int rowIndex);
void removeNumsInNakedPairFromColumn(int possibleNumbersForSpac[][9][9], int firstPairRowIndex, int firstNumInPair, int secondNumInPair, int firstPairColumnIndex, int rowIndex);
void removeNumsInNakedPairsFromSubgrid(int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairColumnIndex, int firstPairFirstNumber, int FirstPairSecondNumber, int secondPairColumnIndex, int secondPairRowIndex);
char findPairsSubgrid(int firstPairColumnIndex, int firstPairRowIndex);
int findColumnOfSubgrid(int firstPairColumnIndex);
int findRowOfSubgrid(int firstPairRowIndex);
void removePossNumsFromSubgrid(int startingColumnForSubgrid, int startingRowForSubgrid, int firstPairColumnIndex, int firstPairRowIndex, int secondPairColumnIndex, int secondPairRowIndex, int pairsFirstNumber, int pairsSecondNumber, int possibleNumbersForSpace[][9][9]);
int main() {

	time_t start, end;
	double cpu_time_used;
	start = clock();

	FILE* puzzleFile;
	puzzleFile = fopen("C:\\Users\\caleb\\OneDrive\\Desktop\\sudoku.csv", "r");
	if (puzzleFile == NULL) {
		printf("PuzzleFile was NULL");
		return(1);
	}

	char currentUnsolvedPuzzle[WIDTH_OF_ONE_PUZZLE];
	char answerOfCurrentPuzzle[WIDTH_OF_ONE_PUZZLE];

	int numberOfPuzzlesSolved = 0;
	int numberOfPuzzleIncorrect = 0;

	// Ask how many puzzles you want to solve
	int numberOfPuzzlesToSolve = 0;
	printf("How many puzzles do you want to solve? (Must be integer between 1-1mil): ");
	scanf("%d", &numberOfPuzzlesToSolve);
	int numberOfPuzzFormatted = numberOfPuzzlesToSolve * WIDTH_OF_UNFINISHED_SOLVED_AND_SPACES;
	printf("\nworking...\n");

	// This loop repeats over each puzzle for however many puzzles the user inputs
	for (int puzzleCounter = 0; puzzleCounter < numberOfPuzzFormatted; puzzleCounter += WIDTH_OF_UNFINISHED_SOLVED_AND_SPACES) {

		fseek(puzzleFile, puzzleCounter, 0);
		fread(currentUnsolvedPuzzle, sizeof(char), WIDTH_OF_ONE_PUZZLE, puzzleFile);
		fseek(puzzleFile, puzzleCounter + WIDTH_OF_ONE_PUZZLE_PLUS_SPACE, 0);
		fread(answerOfCurrentPuzzle, sizeof(char), WIDTH_OF_ONE_PUZZLE, puzzleFile);

		// Declaring arrays, puzzle two dimensions will slowly be changed until it matches answerTwoDimensions
		int currentPuzzleTwoDimensions[9][9];
		int answerTwoDimensions[9][9];
		int possibleNumbersForSpace[9][9][9];
		int snapShot[9][9];

		// Both answers and unsolved puzzles are read into single dimensional arrays, this converts them to two dimensions so that they can be operated on
		// The numbers in the original CSV are also chars, so the arrays are converted into ints
		initializePossibleNumbersArray(possibleNumbersForSpace);
		initializeCurrentPuzzleTwoDimensionsArray(currentPuzzleTwoDimensions, currentUnsolvedPuzzle);
		initializeAnswerOfCurrentPuzzleArray(answerTwoDimensions, answerOfCurrentPuzzle);
		
		int checkedForSinglesCounter = 0;
		
		// current

		do {
			updateCurrentPuzzlePossibleNumbers(currentPuzzleTwoDimensions, possibleNumbersForSpace);
			createSnapshotOfCurrentPuzzle(currentPuzzleTwoDimensions, snapShot);
			insertSinglePossibleNumbers(currentPuzzleTwoDimensions, possibleNumbersForSpace);
			updateCurrentPuzzlePossibleNumbers(currentPuzzleTwoDimensions, possibleNumbersForSpace);
			checkedForSinglesCounter++;
		} while (isSnapshotChanged(currentPuzzleTwoDimensions, snapShot) && checkedForSinglesCounter < 50);

		if (isCurrentPuzzleSolved(currentPuzzleTwoDimensions, answerTwoDimensions))
			numberOfPuzzlesSolved++;
		else {
			searchForNakedPairs(currentPuzzleTwoDimensions, possibleNumbersForSpace);
			do {
				updateCurrentPuzzlePossibleNumbers(currentPuzzleTwoDimensions, possibleNumbersForSpace);
				createSnapshotOfCurrentPuzzle(currentPuzzleTwoDimensions, snapShot);
				searchForNakedPairs(currentPuzzleTwoDimensions, possibleNumbersForSpace);
				insertSinglePossibleNumbers(currentPuzzleTwoDimensions, possibleNumbersForSpace);
				updateCurrentPuzzlePossibleNumbers(currentPuzzleTwoDimensions, possibleNumbersForSpace);
				checkedForSinglesCounter++;
			} while (isSnapshotChanged(currentPuzzleTwoDimensions, snapShot) && checkedForSinglesCounter < 50);

			if (isCurrentPuzzleSolved(currentPuzzleTwoDimensions, answerTwoDimensions)) {
				numberOfPuzzlesSolved++;
			}
			else
			{
			printf("Unfinished or incorrect puzzle || Puzzle number: %d\n", puzzleCounter / 164);
			printArray(currentPuzzleTwoDimensions);
			printf("Possible numbers after searching for naked pairs horizontally multiple times\n");
			printPossibleNumbersArray(possibleNumbersForSpace);
				numberOfPuzzleIncorrect++;
			}

		}
	}

	printf("Number of puzzles solved: %d\n", numberOfPuzzlesSolved);
	printf("Number of puzzles unsolved: %d\n", numberOfPuzzleIncorrect);

	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("cpu_time_used: %lf", cpu_time_used);

	return 0;
}

void initializePossibleNumbersArray(int possibleNumbersForSpace[][9][9]) {
	// Initialize possible numbers array with every number to start
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 1; k < 10; k++) {
				possibleNumbersForSpace[i][j][k - 1] = k;
			}
		}
	}

}
void initializeCurrentPuzzleTwoDimensionsArray(int currentPuzzleTwoDimensions[][9], char currentUnsolvedPuzzle[])
{
	// Load linear array of unsolved puzzle into 2d array
	int rowIndex, columnIndex, increment;
	rowIndex = columnIndex = increment = 0;
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			currentPuzzleTwoDimensions[rowIndex][columnIndex] = ((int)currentUnsolvedPuzzle[increment]) - 48;
			increment++;
		}
	}

}
void initializeAnswerOfCurrentPuzzleArray(int answerTwoDimensions[][9], char answerOfCurrentPuzzle[])
{
	int increment = 0;
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			answerTwoDimensions[rowIndex][columnIndex] = ((int)answerOfCurrentPuzzle[increment]) - 48;
			increment++;
		}
	}

}
void printArray(int anyArray[][9])
{
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			printf("%d ", anyArray[rowIndex][columnIndex]);
		}
		printf("\n");
	}
	printf("\n");

}
void updateCurrentPuzzlePossibleNumbers(int currentPuzzleTwoDimensiosn[][9], int possibleNumbersForSpace[][9][9])
{
	// This iterates through each row and column number and hands it to the functions to check all the possibles in respective rows and columns
	for (int i = 0; i < 9; i++) {
		updatePossibleNumsInColumn(currentPuzzleTwoDimensiosn, possibleNumbersForSpace, i);
		updatePossibleNumsInRow(currentPuzzleTwoDimensiosn, possibleNumbersForSpace, i);
	}

	// These for loops iterate by 3, handing the function the coordinates to the top left square of each subGrid, so that the function checks each sperately
	for (int rowIndex = 0; rowIndex < 9; rowIndex += 3) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex += 3) {
			updatePossibleNumsInSubGrid(currentPuzzleTwoDimensiosn, possibleNumbersForSpace, rowIndex, columnIndex);
		}
	}
}
void updatePossibleNumsInColumn(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int columnIndex)
{
	// looks for spaces that contain numbers in the 
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		if (currentPuzzleTwoDimensions[rowIndex][columnIndex] != 0) {
			int numberAlreadyInColumn = currentPuzzleTwoDimensions[rowIndex][columnIndex];
			for (int everyRowInColumn = 0; everyRowInColumn < 9; everyRowInColumn++) {
				possibleNumbersForSpace[everyRowInColumn][columnIndex][numberAlreadyInColumn - 1] = 0;
			}
			for (int allPossibleNumbers = 0; allPossibleNumbers < 9; allPossibleNumbers++) {
				possibleNumbersForSpace[rowIndex][columnIndex][allPossibleNumbers] = 0;
			}
		}	
	}
}
void updatePossibleNumsInRow(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int rowIndex)
{
	for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
		if (currentPuzzleTwoDimensions[rowIndex][columnIndex] != 0) {
			int numberAlreadyInColumn = currentPuzzleTwoDimensions[rowIndex][columnIndex];
			for (int everyRowInColumn = 0; everyRowInColumn < 9; everyRowInColumn++) {
				possibleNumbersForSpace[rowIndex][everyRowInColumn][numberAlreadyInColumn - 1] = 0;
			}
		}
	}
}
void updatePossibleNumsInSubGrid(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int rowIndex, int columnIndex)
{
	int numberInSubgrid;
	int possNumUpdateYCoord;
	int possNumUpdateXCoord;

	for (int xCoordIterate = columnIndex; xCoordIterate < columnIndex + 3; xCoordIterate++) {
		for (int yCoordIterate = rowIndex; yCoordIterate < rowIndex + 3; yCoordIterate++) {
			if (currentPuzzleTwoDimensions[yCoordIterate][xCoordIterate] != 0) {
				numberInSubgrid = currentPuzzleTwoDimensions[yCoordIterate][xCoordIterate];
				for (possNumUpdateXCoord = columnIndex; possNumUpdateXCoord < columnIndex + 3; possNumUpdateXCoord++) {
					for (possNumUpdateYCoord = rowIndex; possNumUpdateYCoord < rowIndex + 3; possNumUpdateYCoord++) {
						possibleNumbersForSpace[possNumUpdateYCoord][possNumUpdateXCoord][numberInSubgrid - 1] = 0;
					}
				}
			}
		}
	}
}
void createSnapshotOfCurrentPuzzle(int currentPuzzleTwoDimensions[][9], int snapShot[][9])
{
	// This takes a "snapshot" of the current configuration of the loop to compare against later in the function that looks for single digits, if
	// the snapshot is the same after the function that looks for single possible numbers and places them in the puzzle, then other more
	// strategic functions get called
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			snapShot[rowIndex][columnIndex] = currentPuzzleTwoDimensions[rowIndex][columnIndex];
		}
	}
}
void insertSinglePossibleNumbers(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9])
{
	int numberOfPossibleNumbers = 0;
	int onlyNumberPossible = 0;

	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			numberOfPossibleNumbers = 0;
			if (currentPuzzleTwoDimensions[rowIndex][columnIndex] == 0) {
				for (int possibleNums = 0; possibleNums < 9; possibleNums++) {
					if (possibleNumbersForSpace[rowIndex][columnIndex][possibleNums] != 0) {
						numberOfPossibleNumbers++;
						onlyNumberPossible = possibleNumbersForSpace[rowIndex][columnIndex][possibleNums];
					}
					if (numberOfPossibleNumbers > 1) {
						possibleNums = 9;
					}
					if (numberOfPossibleNumbers == 1 && possibleNums == 8) {
						currentPuzzleTwoDimensions[rowIndex][columnIndex] = onlyNumberPossible;
					}
				}
			}
		}
	}
}
bool isSnapshotChanged(int currentPuzzleTwoDimensions[][9], int snapShot[][9])
{
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			if (snapShot[rowIndex][columnIndex] != currentPuzzleTwoDimensions[rowIndex][columnIndex])
				return true;
		}

	}
	return false;
}
void printPossibleNumbersArray(int possibleNumbersForSpace[][9][9])
{
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			// For whatever reason not letting me printf here for the possible numbers
			printf("[");
			for (int possibleNumbers = 0; possibleNumbers < 9; possibleNumbers++) {
				printf("%d", possibleNumbersForSpace[rowIndex][columnIndex][possibleNumbers]);
			}
			printf("]");
		}
		printf("\n");
	}
}
bool isCurrentPuzzleSolved(int currentPuzzleTwoDimensions[][9], int answerTwoDimensions[][9])
{
	int correctNumberCounter = 0;

	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			if (currentPuzzleTwoDimensions[rowIndex][columnIndex] == answerTwoDimensions[rowIndex][columnIndex])
				correctNumberCounter++;
			else
				return false;
		}

	}

	if (correctNumberCounter == 81)
		return true;

	return false;
}
void searchForNakedPairs(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9]) {
	searchForNakedPairsHorizontally(currentPuzzleTwoDimensions, possibleNumbersForSpace);
	searchForNakedPairsVertically(possibleNumbersForSpace);

}
void searchForNakedPairsVertically(int possibleNumbersForSpace[][9][9])
{
	// THIS PARTICULAR FUNCTION IS BACKWARDS IN THE ORDERING OF THE FOR LOOP THAT LOOKS THROUGH THE ARRAY SO BE AWARE AND DON'T USE THIS ONE FOR THE 
	int firstNumInPair, secondNumInPair, firstPairRowIndex, firstPairColumnIndex;
	int numOfPossibleNums = 0;
												
	for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
		for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
			numOfPossibleNums = 0;
			for (int possibleNumIndex = 0; possibleNumIndex < 9; possibleNumIndex++) {
				if (possibleNumbersForSpace[rowIndex][columnIndex][possibleNumIndex] != 0) {
					numOfPossibleNums++;
					switch (numOfPossibleNums) {
						case 0:
							break;
						case 1:
							firstNumInPair = possibleNumIndex + 1;
							firstPairRowIndex = rowIndex;
							firstPairColumnIndex = columnIndex;
							break;
						case 2:
							secondNumInPair = possibleNumIndex + 1;
							break;
						default:
							possibleNumIndex = 9;
							break;
					}

				}
				if (possibleNumIndex == 8 && numOfPossibleNums == 2) {
				searchForSecondNakedPairsVertically(possibleNumbersForSpace, firstPairRowIndex, firstPairColumnIndex, firstNumInPair, secondNumInPair);
				}
			}
		}
	}

}
void searchForSecondNakedPairsVertically (int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairColumnIndex, int firstNumInPair, int secondNumInPair) {
	 
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		if (possibleNumbersForSpace[rowIndex][firstPairColumnIndex][firstNumInPair-1] != 0 && possibleNumbersForSpace[rowIndex][firstPairColumnIndex][secondNumInPair-1] != 0 && rowIndex != firstPairRowIndex) {
			if (areOtherPossibleNumbersEmptyVertically(possibleNumbersForSpace, firstNumInPair, secondNumInPair, firstPairRowIndex, firstPairColumnIndex, rowIndex)) {
				removeNumsInNakedPairFromColumn(possibleNumbersForSpace, firstPairRowIndex, firstNumInPair, secondNumInPair, firstPairColumnIndex, rowIndex);
				removeNumsInNakedPairsFromSubgrid(possibleNumbersForSpace, firstPairRowIndex, firstPairColumnIndex, firstNumInPair, secondNumInPair, firstPairColumnIndex, rowIndex);
			}
		}
	}
}
bool areOtherPossibleNumbersEmptyVertically(int possibleNumbersForSpace[][9][9], int firstNumInPair, int secondNumInPair, int firstPairRowIndex, int firstPairColumnIndex, int rowIndex) {
	for (int possibleNums = 0; possibleNums < 9; possibleNums++) {
		if (possibleNums != firstNumInPair - 1 && possibleNums != secondNumInPair - 1) {
			if (possibleNumbersForSpace[rowIndex][firstPairColumnIndex][possibleNums] != 0) {
				return false;
			}
		}
	}
	return true;
}
void removeNumsInNakedPairFromColumn(int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstNumInPair, int secondNumInPair, int firstPairColumnIndex, int rowIndex) {
	int secondPairRowIndex = rowIndex;
		
		for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
			if (rowIndex!= firstPairRowIndex && rowIndex != secondPairRowIndex) {
				possibleNumbersForSpace[rowIndex][firstPairColumnIndex][firstNumInPair - 1] = 0;
				possibleNumbersForSpace[rowIndex][firstPairColumnIndex][secondNumInPair- 1] = 0;
			}
		}
}
void searchForNakedPairsHorizontally(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9]) {

	int firstNumInPair, secondNumInPair, firstPairRowIndex, firstPairColumnIndex;
	int numOfPossibleNums = 0;
												
	for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			numOfPossibleNums = 0;
			for (int possibleNumIndex = 0; possibleNumIndex < 9; possibleNumIndex++) {
				if (possibleNumbersForSpace[rowIndex][columnIndex][possibleNumIndex] != 0) {
					numOfPossibleNums++;
					switch (numOfPossibleNums) {
						case 0:
							break;
						case 1:
							firstNumInPair = possibleNumIndex + 1;
							firstPairRowIndex = rowIndex;
							firstPairColumnIndex = columnIndex;
							break;
						case 2:
							secondNumInPair = possibleNumIndex + 1;
							break;
						default:
							possibleNumIndex = 9;
							break;
					}

				}
				if (possibleNumIndex == 8 && numOfPossibleNums == 2) {
				searchForSecondNakedPair(currentPuzzleTwoDimensions, possibleNumbersForSpace, firstPairRowIndex, firstPairColumnIndex, firstNumInPair, secondNumInPair);
				}
			}
		}
	}

}
void searchForSecondNakedPair(int currentPuzzleTwoDimensions[][9], int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairColumnIndex, int firstPairsFirstNumber, int firstPairsSecondNumber) {

	 
	for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
		if (possibleNumbersForSpace[firstPairRowIndex][columnIndex][firstPairsFirstNumber-1] != 0 && possibleNumbersForSpace[firstPairRowIndex][columnIndex][firstPairsSecondNumber-1] != 0 && columnIndex != firstPairColumnIndex) {
			if (areOtherPossibleNumbersEmpty(possibleNumbersForSpace, firstPairsFirstNumber, firstPairsSecondNumber, firstPairRowIndex, firstPairColumnIndex, columnIndex)) {
				removeNumsInNakedPairFromRow(possibleNumbersForSpace, firstPairRowIndex, firstPairsFirstNumber, firstPairsSecondNumber, firstPairColumnIndex, columnIndex);
				removeNumsInNakedPairsFromSubgrid(possibleNumbersForSpace, firstPairRowIndex, firstPairColumnIndex, firstPairsFirstNumber, firstPairsSecondNumber, columnIndex, firstPairRowIndex);
			}
		}
	}
}
void removeNumsInNakedPairFromRow(int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairsFirstNumber, int firstPairsSecondNumber, int firstPairColumnIndex, int secondPairColumnIndex) {
		for (int columnIndex = 0; columnIndex < 9; columnIndex++) {
			if (columnIndex != firstPairColumnIndex && columnIndex != secondPairColumnIndex) {
				possibleNumbersForSpace[firstPairRowIndex][columnIndex][firstPairsFirstNumber - 1] = 0;
				possibleNumbersForSpace[firstPairRowIndex][columnIndex][firstPairsSecondNumber - 1] = 0;
			}
		}
}
bool areOtherPossibleNumbersEmpty(int possibleNumbersForSpace[][9][9], int firstPairsFirstNumber, int firstPairsSecondNumber, int firstPairRowIndex, int firstPairColumnIndex, int secondPairsColumnIndex) {
	for (int possibleNums = 0; possibleNums < 9; possibleNums++) {
		if (possibleNums != firstPairsFirstNumber - 1 && possibleNums != firstPairsSecondNumber - 1) {
			if (possibleNumbersForSpace[firstPairRowIndex][secondPairsColumnIndex][possibleNums] != 0) {
				return false;
			}
		}
	}
	return true;
}
void removeNumsInNakedPairsFromSubgrid(int possibleNumbersForSpace[][9][9], int firstPairRowIndex, int firstPairColumnIndex, int firstPairFirstNumber, int FirstPairSecondNumber, int secondPairColumnIndex, int secondPairRowIndex) {
	
	char firstPairSubgrid = findPairsSubgrid(firstPairColumnIndex, firstPairRowIndex);
	char secondPairSubgrid = findPairsSubgrid(secondPairColumnIndex, secondPairRowIndex);

	if (firstPairSubgrid == secondPairSubgrid) {
		
		switch (firstPairSubgrid) {
		case 'a':
			removePossNumsFromSubgrid(0, 0, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'b':	
			removePossNumsFromSubgrid(3, 0, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'c':
			removePossNumsFromSubgrid(6, 0, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'd':
			removePossNumsFromSubgrid(0, 3, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'e':
			removePossNumsFromSubgrid(3, 3, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'f':
			removePossNumsFromSubgrid(6, 3, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'g':
			removePossNumsFromSubgrid(0, 6, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'h':
			removePossNumsFromSubgrid(3, 6, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		case 'i':
			removePossNumsFromSubgrid(6, 6, firstPairColumnIndex, firstPairRowIndex, secondPairColumnIndex, secondPairRowIndex, firstPairFirstNumber, FirstPairSecondNumber, possibleNumbersForSpace);
			break;
		}
	
	}
	return;
}
char findPairsSubgrid(int firstPairColumnIndex, int firstPairRowIndex) {

	int subGridColumn = findColumnOfSubgrid(firstPairColumnIndex);
	int subGridRow = findRowOfSubgrid(firstPairRowIndex);
    
	if (subGridColumn == 1) {
		if(subGridRow == 1)
			return 'a';
		if (subGridRow == 2) 
			return 'd';
		if (subGridRow == 3)
			return 'g';
	}
	if (subGridColumn == 2) {	
		if(subGridRow == 1)
			return 'b';
		if (subGridRow == 2) 
			return 'e';
		if (subGridRow == 3)
			return 'h';
	}
	if (subGridColumn == 3) {
		if (subGridRow == 1)
			return 'c';
		if (subGridRow == 2)
			return 'f';
		if (subGridRow == 3)
			return 'i';
	}
}
int findColumnOfSubgrid(int firstPairColumnIndex) {
	if (firstPairColumnIndex < 3) 
		return 1;
	if (firstPairColumnIndex > 2 && firstPairColumnIndex < 6)
		return 2;
	if (firstPairColumnIndex > 5)
		return 3; 

}
int findRowOfSubgrid(int firstPairRowIndex) {

	if (firstPairRowIndex < 3) 
		return 1;
	if (firstPairRowIndex > 2 && firstPairRowIndex < 6)
		return 2;
	if (firstPairRowIndex > 5)
		return 3; 
}
void removePossNumsFromSubgrid(int startingColumnForSubgrid, int startingRowForSubgrid, int firstPairColumnIndex, int firstPairRowIndex, int secondPairColumnIndex, int secondPairRowIndex, int pairsFirstNumber, int pairsSecondNumber, int possibleNumbersForSpace[][9][9]) {
	// Both digits in the naked pair are removed from the subgrid's possible numbers index, unless loop iterates over the location of either of the naked pairs
	int rowIndexSubgridStart = startingRowForSubgrid;
	int columnIndexSubgridStart = startingColumnForSubgrid; 
	for (startingRowForSubgrid = rowIndexSubgridStart; startingRowForSubgrid < rowIndexSubgridStart + 3; startingRowForSubgrid++) {
		for (startingColumnForSubgrid = columnIndexSubgridStart; startingColumnForSubgrid < columnIndexSubgridStart+ 3; startingColumnForSubgrid++) {
			if ((startingColumnForSubgrid != firstPairColumnIndex && startingRowForSubgrid != firstPairRowIndex) && (startingColumnForSubgrid != secondPairColumnIndex && startingRowForSubgrid != secondPairRowIndex)) {
				possibleNumbersForSpace[startingRowForSubgrid][startingColumnForSubgrid][pairsFirstNumber-1] = 0;
				possibleNumbersForSpace[startingRowForSubgrid][startingColumnForSubgrid][pairsSecondNumber-1] = 0;
			}
		}
	}

	return;
}
