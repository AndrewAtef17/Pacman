#pragma once

typedef char Array2D[31][26]; //New type

extern Array2D maze1;
extern Array2D maze2;
extern Array2D maze3;
extern Array2D* currMaze;

struct Position
{
	int x,y;
};

struct HIGH_SCORES
{
	char name[100];
	int score;
};

bool checkwall(int,int);
