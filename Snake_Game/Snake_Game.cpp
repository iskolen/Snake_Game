#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>

using namespace std;

struct pointField 
{
	int x, y;
};

struct appleField 
{
	pointField coordinat;
	const char symbol = '@';
};
appleField apple = { 2, 3 };

struct snakeField
{
	vector <pointField> body;
	const char symbol = '#';
};
snakeField snake = { {{1, 2}, {1, 3}} };


const char fieldBackground = '*';
const int fieldX = 8, fieldY = 4;
const char UP = 'w', DOWN = 's', RIGHT = 'd', LEFT = 'a';
char dir = 'd';
char key;
bool gameOver = false;
vector <vector <char>>field(fieldY, std::vector <char>(fieldX, fieldBackground));
int saveLastX = 0, saveLastY = 0;
int delay = 600;
int score = 0;

void SetCur(int x, int y)// Функция для очищения поля, вместо system("cls")
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int getRandomNumber(int min, int max)// Получение рандомного числа
{
	srand(time(NULL));
	int num = min + rand() % (max - min + 1);

	return num;
}

void printField()// Печать поля 
{
	SetCur(0, 0);
	for (auto &line : field) 
	{
		for (auto &element : line) 
		{
			cout << element << ' ';
		}
		cout << endl;
	}
}

void placeApple()//Ставим яблоко на поле
{
	field[apple.coordinat.x][apple.coordinat.y] = apple.symbol;
}

void placeSnake()// Ставим змейку на поле
{
	for (int i = 0; i < snake.body.size(); i++) 
	{
		field[snake.body[i].x][snake.body[i].y] = snake.symbol;
	}
}

void direct(pointField &newPoint, int &saveX, int &saveY, int &saveX1, int &saveY1, char Pos, int Move, char Direct)// Обработка движения хвоста
{
	if (dir == Direct)
	{
		for (int i = 0; i < snake.body.size(); i++)
		{
			if (i + 1 == snake.body.size())
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
					if (Pos == 'x') {
						snake.body[i].x = snake.body[i].x + Move;
					}
					else {
						snake.body[i].y = snake.body[i].y + Move;
					}
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

void move()// Передвижение змейки
{
	int saveX = 0, saveY = 0, saveX1 = 0, saveY1 = 0;
	pointField newPoint;
	direct(newPoint, saveX, saveY, saveX1, saveY1, 'x', -1, UP);
	direct(newPoint, saveX, saveY, saveX1, saveY1, 'y', -1, LEFT);
	direct(newPoint, saveX, saveY, saveX1, saveY1, 'x', 1, DOWN);
	direct(newPoint, saveX, saveY, saveX1, saveY1, 'y', 1, RIGHT);
}

void collisionWall()// Столкновение со стеной
{
	if (snake.body[0].x > (fieldY - 1) || snake.body[0].y > (fieldX - 1) || snake.body[0].x < 0 || snake.body[0].y < 0)
	{
		gameOver = true;
	}
}

void collisionSnake()// Если змейка столкнулась с телом
{
	for (int i = 1; i < snake.body.size(); i++)
	{
		if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y)
		{
			gameOver = true;
		}
	}
}

void eatApple()// Съедание яблока
{
	pointField newCoord = {};
	newCoord = { saveLastX, saveLastY };
	snake.body.insert(end(snake.body), newCoord);
	score++;
}

bool isApple()// Проверка на то, находится ли голова змейки, на месте яблока
{
	if ((snake.body[0].x == apple.coordinat.x) && (snake.body[0].y == apple.coordinat.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void appleСreation()// Генерация позиции яблока так, чтобы оно не находилось внутри змейки
{
	while (1) 
	{
		int samePosition = 0;
		apple.coordinat = { getRandomNumber(0, (fieldY - 1)), getRandomNumber(0, (fieldX - 1)) };
		for (int i = 0; i < snake.body.size(); i++)
		{
			if ((apple.coordinat.x == snake.body[i].x) && (apple.coordinat.y == snake.body[i].y)) 
			{
				samePosition += 1;
			}
		}
		if (samePosition == 0) {
			break;
		}
	}
}

void isGameOver()// Конец игры - вывод на поле "GameOver" 
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

void isWin()// Конец игры - вывод на поле "WIN!"
{
	field[1][2] = 'W';
	field[1][3] = 'I';
	field[1][4] = 'N';
	field[1][5] = '!';
}

void scoring()
{
	cout << "    Счёт: " << score;
}

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	placeSnake();
	placeApple();
	printField();
	while (gameOver != true || snake.body.size() != 32)
	{
		if (_kbhit())
		{
			key = _getch();
			if ((key == UP && dir != DOWN) || (key == DOWN && dir != UP) || (key == LEFT && dir != RIGHT) || (key == RIGHT && dir != LEFT)) 
			{
				dir = key;
				move();
			}
			else 
			{
				move();
			}
		}
		else 
		{
			move();
		}
		if (isApple()) 
		{
			eatApple();
			appleСreation();
			placeApple();
		}
		collisionWall();
		if (gameOver == true)
			break;
		collisionSnake();
		if (gameOver == true)
			break;
		placeSnake();
		printField();
		scoring();
		Sleep(delay);
	}
	if (gameOver == true)
	{
		isGameOver();
		printField();
	}
	else
	{
		isWin();
		printField();
	}
}