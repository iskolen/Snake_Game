#include <queue>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;


const char fieldBackground = '*';
const int fieldX = 8, fieldY = 8;
int snakeX;
int snakeY;
vector<vector<char>>field(fieldY, vector<char>(fieldX, fieldBackground));
vector<char>snakeVector(snakeX, snakeY);

bool gameOver = false;
bool eatApple = false;
enum eDirection { LEFT, RIGHT, UP, DOWN, STOP };
eDirection dir;


int getRandomNumber(int min, int max)
{
	srand(time(NULL));
	int num = min + rand() % (max - min + 1);

	return num;
}



void printField()
{
	system("cls");
	for (auto &line : field) {
		for (auto &element : line) {
			cout << element << ' ';
		}
		cout << '\n';
	}
}



struct pointField
{
	int x;
	int y;
};
struct appleField
{
	pointField coordinat;
	const char symbol = '@';
};
pointField point;
appleField apple;



void placeApple()
{
	apple.coordinat = { getRandomNumber(0, (fieldY - 1)), getRandomNumber(0, (fieldX - 1)) };

	if (snake.coordinat.x == apple.coordinat.x && snake.coordinat.y == apple.coordinat.y)
	{
		while (snake.coordinat.x == apple.coordinat.x && snake.coordinat.y == apple.coordinat.y)
		{
			apple.coordinat = { getRandomNumber(0, (fieldY - 1)), getRandomNumber(0, (fieldX - 1)) };;
		}
	}

	for (int i = 0; i < field.size(); i++)
	{
		for (int j = 0; j < field[i].size(); j++)
		{
			if ((i == apple.coordinat.x) && (j == apple.coordinat.y))
			{
				field[i][j] = apple.symbol;
			}
		}
	}
}



struct snakeField
{
	pointField coordinat;
	int head;
	int body;
	int length;
	const char symbolHead = 'O';
	const char symbolBody = '#';
};
snakeField snake;

void placeSnake()
{
	for (int i = 0; i < field.size(); i++)
	{
		for (int j = 0; j < field[i].size(); j++)
		{
			if ((i == snake.coordinat.x) && (j == snake.coordinat.y))
			{
				for (int l = j; l < (j + snake.length); l++)
				{
					field[i][l] = snake.symbolHead;
				}
			}
		}
	}
}

/*void placeSnake()
{
	for (int i = 0; i < field.size(); i++)
	{
		for (int j = 0; j < field[i].size(); j++)
		{
			if ((i == snake.coordinat.x) && (j == snake.coordinat.y))
			{
				field[i][j] = snake.symbol;
			}
		}
	}
}*/

void collisionWall()
{
	if (snake.coordinat.x > (fieldY - 1) || snake.coordinat.y > (fieldX - 1) || snake.coordinat.x < 0 || snake.coordinat.y < 0)
	{
		gameOver = true;
	}
}

void removeSnake()
{
	for (int i = 0; i < field.size(); i++)
	{
		for (int j = 0; j < field[i].size(); j++)
		{
			if ((i == snake.coordinat.x) && (j == snake.coordinat.y))
			{
				field[i][j] = fieldBackground;
			}
		}
	}
}

void removeApple()
{
	for (int i = 0; i < field.size(); i++)
	{
		for (int j = 0; j < field[i].size(); j++)
		{
			if ((i == apple.coordinat.x) && (j == apple.coordinat.y))
			{
				field[i][j] = fieldBackground;
			}
		}
	}
}


void isGameOver()
{
	if (gameOver == true)
	{
		field[1][0] = 'G';
		field[1][1] = 'a';
		field[1][2] = 'm';
		field[1][3] = 'e';
		field[1][4] = 'O';
		field[1][5] = 'v';
		field[1][6] = 'e';
		field[1][7] = 'r';
	}
}


void moveSnake()
{
	switch (dir)
	{
	case LEFT:
		removeSnake();
		snake.coordinat.y--;
		break;
	case RIGHT:
		removeSnake();
		snake.coordinat.y++;
		break;
	case UP:
		removeSnake();
		snake.coordinat.x--;
		break;
	case DOWN:
		removeSnake();
		snake.coordinat.x++;
		break;
	case STOP:
		break;
	}
}

void pressKey()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			removeSnake();
			snake.coordinat.y--;
			dir = LEFT;
			break;
		case 'd':
			removeSnake();
			snake.coordinat.y++;
			dir = RIGHT;
			break;
		case 'w':
			removeSnake();
			snake.coordinat.x--;
			dir = UP;
			break;
		case 's':
			removeSnake();
			snake.coordinat.x++;
			dir = DOWN;
			break;
		}
	}
}

void isEatApple()
{
	if (eatApple == true)
	{
		placeApple();
		eatApple = false;
	}
}

int main() {
	snake.coordinat = { 2, 2 };
	snake.length = 1;
	dir = STOP;
	placeApple();
	while (gameOver != true)
	{
		placeSnake();
		printField();
		moveSnake();
		pressKey();
		if (snake.coordinat.x == apple.coordinat.x && snake.coordinat.y == apple.coordinat.y)
		{
			eatApple = true;
			snake.length += 1;
		}
		isEatApple();
		collisionWall();
		Sleep(500);
	}
	isGameOver();
	removeApple();
	printField();
}