#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int revealed;
    int mine;
    int value;
} Cell;

void initializeBoard(Cell **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j].revealed = 0;
            board[i][j].mine = 0;
            board[i][j].value = rand() % 10;  // Random value between 0 and 9
        }
    }
}

void placeMines(Cell **board, int size, int numMines) {
    srand(time(0));
    for (int i = 0; i < numMines; i++) {
        int x = rand() % size;
        int y = rand() % size;
        if (board[x][y].mine == 0) {
            board[x][y].mine = 1;
            board[x][y].value = 0;  // Ensure mines have a value of 0
        } else {
            i--;  // Retry if a mine is already placed here
        }
    }
}

void printBoard(Cell **board, int size, int reveal) {
    printf("   ");
    for (int i = 0; i < size; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("%d: ", i);
        for (int j = 0; j < size; j++) {
            if (reveal || board[i][j].revealed) {
                if (board[i][j].mine) {
                    printf("* ");
                } else {
                    printf("%d ", board[i][j].value);
                }
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int revealCell(Cell **board, int size, int x, int y, int *score) {
    if (x < 0 || x >= size || y < 0 || y >= size || board[x][y].revealed) {
        return 0;
    }

    board[x][y].revealed = 1;

    if (board[x][y].mine) {
        return -1;  // Hit a mine
    }

    *score += board[x][y].value;

    return 1;
}

void playSkipMine() {
    int size, numMines;
    printf("Enter the size of the board: ");
    scanf("%d", &size);
    printf("Enter the number of mines: ");
    scanf("%d", &numMines);

    Cell **board = malloc(size * sizeof(Cell *));
    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(Cell));
    }

    srand(time(0));  // Seed the random number generator
    initializeBoard(board, size);
    placeMines(board, size, numMines);

    int gameOver = 0;
    int cellsToReveal = size * size - numMines;
    int revealedCells = 0;
    int score = 0;

    while (!gameOver && revealedCells < cellsToReveal) {
        printBoard(board, size, 0);
        printf("Enter the coordinates to reveal (row col): ");
        int x, y;
        scanf("%d %d", &x, &y);
        
        int result = revealCell(board, size, x, y, &score);
        if (result == -1) {
            gameOver = 1;
            printf("You hit a mine! Game over.\n");
        } else {
            revealedCells++;
            if (revealedCells == cellsToReveal) {
                printf("Congratulations! You revealed all safe cells.\n");
            }
        }
    }

    printBoard(board, size, 1);  // Reveal the entire board at the end
    printf("Your final score is: %d\n", score);

    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

void playGuessNumberGame(int numPlayers) {
    int secretNumber, guess, attempts[numPlayers];
    int currentPlayer, minAttempts = 1000, winner;
    int winnersCount = 0; // Track number of players with minimum attempts

    printf("Welcome to the Number Guessing Game!\n");

    for (currentPlayer = 0; currentPlayer < numPlayers; currentPlayer++) {
        attempts[currentPlayer] = 0;
        srand(time(0) + currentPlayer); // Seed the random number generator uniquely for each player
        secretNumber = rand() % 100 + 1;
        printf("Player %d's turn:\n", currentPlayer + 1);
        do {
            printf("Enter your guess: ");
            scanf("%d", &guess);
            attempts[currentPlayer]++;

            if (guess > secretNumber) {
                printf("Too high! Try again.\n");
            } else if (guess < secretNumber) {
                printf("Too low! Try again.\n");
            } else {
                printf("Congratulations Player %d! You guessed the number in %d attempts.\n", currentPlayer + 1, attempts[currentPlayer]);
                if (attempts[currentPlayer] < minAttempts) {
                    minAttempts = attempts[currentPlayer];
                    winner = currentPlayer + 1;
                    winnersCount = 1;
                } else if (attempts[currentPlayer] == minAttempts) {
                    winnersCount++;
                }
            }
        } while (guess != secretNumber);
    }

    if (winnersCount > 1) {
        printf("\nIt's a draw! Multiple players guessed the number in %d attempts.\n", minAttempts);
    } else {
        printf("\nPlayer %d is the winner with %d attempts!\n", winner, minAttempts);
    }
}

int main() {
    int userChoice;

    printf("List Of Games\n");
    printf("1. Skipmine\n");
    printf("2. Guess it\n");
    printf("Enter the game number you want to play: ");
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        playSkipMine();
    } else if (userChoice == 2) {
        int numPlayers;
        printf("Enter the number of players: ");
        scanf("%d", &numPlayers);
        playGuessNumberGame(numPlayers);
    } else {
        printf("Invalid choice. Please restart the program and select a valid option.\n");
    }

    return 0;
}
