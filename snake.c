#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define BOARD_SIZE 30
#define MAX_LENGTH (BOARD_SIZE * BOARD_SIZE)

typedef struct {
    int x;
    int y;
} segment;

void sleep_ms(int milliseconds) {
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void gameBoard(int appleX, int appleY, segment snake[], int *snakeLength)
{
	clear();
	int board[BOARD_SIZE][BOARD_SIZE * 2] = {0};

    for (int i = 0; i < *snakeLength; i++) {
        if (i == 0)
            board[snake[i].x][snake[i].y] = 3;
        else {
            board[snake[i].x][snake[i].y] = 4;
        }
    }

	for (int i = 0; i < BOARD_SIZE * 2; i++) {
		board[0][i] = 1;
		board[BOARD_SIZE - 1][i] = 1;
	}

	for (int i = 1; i < BOARD_SIZE - 1; i++) {
		board[i][0] = 1;
		board[i][(BOARD_SIZE * 2) - 1] = 1;
	}

	board[appleY][appleX] = 2;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0 ; j < BOARD_SIZE * 2; j++)
			if (board[i][j] == 0) {
                mvprintw(i, j, " ");
            } else if (board[i][j] == 1) {
                mvprintw(i, j, "#");
            } else if (board[i][j] == 2) {
                mvprintw(i, j, "O");
            } else if (board[i][j] == 3) {
                mvprintw(i, j, "$");
            } else if (board[i][j] == 4) {
                mvprintw(i, j, "*");
            }
		
		printf("\n");
	}
}

int newApple(int *appleX, int *appleY)
{
	*appleX = rand() % 59;
    *appleY = rand() % 29;
	if (*appleX == 0)
		(*appleX)++;
    else if (*appleY == 0)
        (*appleY)++;
}

void updateSnakePosition(int direction, segment snake[], int *snakeLength) {
    for (int i = *snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    switch (direction) {
        case 0:
            snake[0].x--;
            break;
        case 1:
            snake[0].y++;
            break;
        case 2:
            snake[0].x++;
            break;
        case 3:
            snake[0].y--;
            break;
    }
}

int snakeDirection(int currentDirection) {
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            if (currentDirection != 2) return 0;
            break;
        case KEY_RIGHT:
            if (currentDirection != 3) return 1;
            break;
        case KEY_DOWN:
            if (currentDirection != 0) return 2;
            break;
        case KEY_LEFT:
            if (currentDirection != 1) return 3;
            break;
    }
    return currentDirection; 
}

int main(void) {
    srand(time(NULL));
    int appleX, appleY;
    newApple(&appleX, &appleY);

    segment snake[MAX_LENGTH];
    int snakeLength = 1;
    snake[0].x = 15;
    snake[0].y = 30;

    int direction = 1; 
    bool gameOver = false;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);  
    nodelay(stdscr, TRUE); 

    while (!gameOver) {
        sleep_ms(100);
        direction = snakeDirection(direction);
        updateSnakePosition(direction, snake, &snakeLength);
        if (snake[0].x == BOARD_SIZE - 1 || snake[0].x == 0 || snake[0].y == (BOARD_SIZE * 2) - 1 || snake[0].y == 0)
            gameOver = true;

        for (int i = 1; i < snakeLength; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
                gameOver = true;
        }

        if (snake[0].x == appleY && snake[0].y == appleX) {
            newApple(&appleX, &appleY);
            snakeLength++;
        }


        gameBoard(appleX, appleY, snake, &snakeLength);
        
        refresh();
    }
    endwin();

    printf("\nGame Over !\n");
    printf("Your score is : %d\n\n", snakeLength);
    return 0;
}