#include <queue>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;


const char fieldBackground = '*';
const int fieldX = 8, fieldY = 4;
vector<vector<char>>field(fieldY, vector<char>(fieldX, fieldBackground));


void printField() 
{
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
	apple.coordinat = { 2, 6 };
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
	int length;
	const char symbol = '#';
};
snakeField snake;

void placeSnake()
{
	snake.length = 3;
	snake.coordinat = { 2, 2 };

	for (int i = 0; i < field.size(); i++)
	{
		for (int j = 0; j < field[i].size(); j++)
		{
			if ((i == snake.coordinat.x) && (j == snake.coordinat.y))
			{
				for (int l = j; l < (j + snake.length); l++)
				{
					field[i][l] = snake.symbol;
				}
			}
		}
	}
}



int main() {
	placeApple();
	placeSnake();
	printField();
}