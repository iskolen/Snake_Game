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
int saveLastX = 0, saveLastY = 0;

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

struct snakeField
{
	vector <pointField> body;
	const char symbol = '#';
};
snakeField snake;

void placeApple(int snakeX, int snakeY)//Ставим яблоко на поле
{
	apple.coordinat = { getRandomNumber(0, (fieldY - 1)), getRandomNumber(0, (fieldX - 1)) };

	if (snakeX == apple.coordinat.x && snakeY == apple.coordinat.y)// Условие для того, чтобы яболоко не появлялось на месте змейки
	{
		while (snakeX == apple.coordinat.x && snakeY == apple.coordinat.y)
		{
			apple.coordinat = { getRandomNumber(0, (fieldY - 1)), getRandomNumber(0, (fieldX - 1)) };;
		}
	}

	field[apple.coordinat.x][apple.coordinat.y] = apple.symbol;
}

void placeSnake()// Ставим змейку на поле
{
	for (int s = 0; s < snake.body.size(); s++)
	{
		for (int i = 0; i < field.size(); i++)
		{
			for (int j = 0; j < field[i].size(); j++)
			{
				if ((i == snake.body[s].x) && (j == snake.body[s].y))
				{
					field[i][j] = snake.symbol;
				}
			}
		}
	}
}

void collisionWall()// Столкновение со стеной
{
	if (snake.body[0].x > (fieldY - 1) || snake.body[0].y > (fieldX - 1) || snake.body[0].x < 0 || snake.body[0].y < 0)
	{
		gameOver = true;
	}
}

void removeSnake()// Удаление змейки
{
	for (int s = 0; s < snake.body.size(); s++)
	{
		for (int i = 0; i < field.size(); i++)
		{
			for (int j = 0; j < field[i].size(); j++)
			{
				if ((i == snake.body[s].x) && (j == snake.body[s].y))
				{
					field[i][j] = fieldBackground;
				}
			}
		}
	}
}

void removeApple()// Удаление яблока
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


void isGameOver()// Конец игры - вывод на поле "GameOver" 
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

void moveSnake()// Передвижение змейки
{
	switch (dir)
	{
	case LEFT:
		removeSnake();
		snake.body[0].y--;
		break;
	case RIGHT:
		removeSnake();
		snake.body[0].y++;
		break;
	case UP:
		removeSnake();
		snake.body[0].x--;
		break;
	case DOWN:
		removeSnake();
		snake.body[0].x++;
		break;
	case STOP:
		break;
	}
}

void pressKey()// Нажатие на клавиши
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			removeSnake();
			snake.body[0].y--;
			dir = LEFT;
			move();
			break;
		case 'd':
			removeSnake();
			snake.body[0].y++;
			dir = RIGHT;
			move();
			break;
		case 'w':
			removeSnake();
			snake.body[0].x--;
			dir = UP;
			move();
			break;
		case 's':
			removeSnake();
			snake.body[0].x++;
			dir = DOWN;
			move();
			break;
		}
	}
}

void isEatApple()// Если голова находиться на яблоке, тогда яблоко сьедено
{
	if (eatApple == true)
	{

		placeApple(snake.body[0].x, snake.body[0].y);
		eatApple = false;
	}
}

void setupValues()// Установка начальных значений
{
	snake.body = { { 2, 2 } };
	dir = STOP;
	placeApple(snake.body[0].x, snake.body[0].y);
}

void move()
{
	int saveX = 0, saveY = 0, saveX1 = 0, saveY1 = 0;
	pointField newPoint;
	if (dir == UP)
	{
		for (int i = 0; i < snake.body.size(); i++)
		{
			if (i + 1 == snake.body.size())// для конца змейки
			{
				saveLastX = snake.body[i].x;
				saveLastY = snake.body[i].y;
				field[snake.body[i].x][snake.body[i].y] = '*';
				snake.body.erase(end(snake.body) - 1);
				snake.body.insert(end(snake.body), newPoint);
			}
			else
			{
				if (i == 0)
				{
					saveX = snake.body[i].x;
					saveY = snake.body[i].y;
					newPoint = { saveX, saveY };
					snake.body[i].x = snake.body[i].x - 1;
				}
				else
				{
					saveX1 = snake.body[i].x;
					saveY1 = snake.body[i].y;
					snake.body[i].x = saveX;
					snake.body[i].y = saveY;
					saveX = saveX1;
					saveY = saveY1;
					newPoint = { saveX, saveY };
				}
			}
		}
	}
	if (dir == LEFT)
	{
		for (int i = 0; i < snake.body.size(); i++)
		{
			if (i + 1 == snake.body.size())// для конца змейки
			{
				saveLastX = snake.body[i].x;
				saveLastY = snake.body[i].y;
				field[snake.body[i].x][snake.body[i].y] = '*';
				snake.body.erase(end(snake.body) - 1);
				snake.body.insert(end(snake.body), newPoint);
			}
			else
			{
				if (i == 0)
				{
					saveX = snake.body[i].x;
					saveY = snake.body[i].y;
					newPoint = { saveX, saveY };
					snake.body[i].y = snake.body[i].y - 1;
				}
				else
				{
					saveX1 = snake.body[i].x;
					saveY1 = snake.body[i].y;
					snake.body[i].x = saveX;
					snake.body[i].y = saveY;
					saveX = saveX1;
					saveY = saveY1;
					newPoint = { saveX, saveY };
				}
			}
		}
	}
	if (dir == DOWN)
	{
		for (int i = 0; i < snake.body.size(); i++)
		{
			if (i + 1 == snake.body.size())// для конца змейки
			{
				saveLastX = snake.body[i].x;
				saveLastY = snake.body[i].y;
				field[snake.body[i].x][snake.body[i].y] = '*';
				snake.body.erase(end(snake.body) - 1);
				snake.body.insert(end(snake.body), newPoint);
			}
			else
			{
				if (i == 0)
				{
					saveX = snake.body[i].x;
					saveY = snake.body[i].y;
					newPoint = { saveX, saveY };
					snake.body[i].x = snake.body[i].x + 1;
				}
				else
				{
					saveX1 = snake.body[i].x;
					saveY1 = snake.body[i].y;
					snake.body[i].x = saveX;
					snake.body[i].y = saveY;
					saveX = saveX1;
					saveY = saveY1;
					newPoint = { saveX, saveY };
				}
			}
		}
	}
	if (dir == RIGHT)
	{
		for (int i = 0; i < snake.body.size(); i++)
		{
			if (i + 1 == snake.body.size())// для конца змейки
			{
				saveLastX = snake.body[i].x;
				saveLastY = snake.body[i].y;
				field[snake.body[i].x][snake.body[i].y] = '*';
				snake.body.erase(end(snake.body) - 1);
				snake.body.insert(end(snake.body), newPoint);
			}
			else
			{
				if (i == 0)
				{
					saveX = snake.body[i].x;
					saveY = snake.body[i].y;
					newPoint = { saveX, saveY };
					snake.body[i].y = snake.body[i].y + 1;
				}
				else
				{
					saveX1 = snake.body[i].x;
					saveY1 = snake.body[i].y;
					snake.body[i].x = saveX;
					snake.body[i].y = saveY;
					saveX = saveX1;
					saveY = saveY1;
					newPoint = { saveX, saveY };
				}
			}
		}
	}
}
int main() {
	setupValues();
	while (gameOver != true)
	{
		placeSnake();
		printField();
		moveSnake();
		pressKey();
		if (snake.body[0].x == apple.coordinat.x && snake.body[0].y == apple.coordinat.y)
		{
			eatApple = true;
		}
		isEatApple();
		collisionWall();
		Sleep(600);
	}
	removeApple();
	isGameOver();
	printField();
}