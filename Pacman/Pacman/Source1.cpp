#include <iostream>
#include "pacman.h"
using namespace std;


Array2D maze1={
"#########################",
"# ..........#..........0#",
"#.####.####.#.####.####.#",
"#.####.####.#.####.####.#",
"#.####.####.#.####.####.#",
"#.......................#",
"#.####.##.#####.##.####.#",
"#.####.##..0#...##.####.#",
"#.####.####.#.####.####.#",
"#......####.#.####......#",
"######.##...#...##.######",
"######.##.#####.##.######",
"######.##.#####.##.######",
"######.##.#####.##.######",
"######.##.#####.##.######",
"#......##.......##0.....#",
"#.####.##.## ##.##.####.#",
"#0........#   #.........#",
"######.##.#####.##.######",
"######.##.......##.######",
"######.##.#####.##.######",
"######.##...#...##.######",
"######.####.#.####.######",
"#...........#...........#",
"#.####.####.#.####.####.#",
"#...0#.............#....#",
"####.#.#.#######.#.#.####",
"#......#....#....#.....0#",
"#.#########.#.#########.#",
"#.......................#",
"#########################",
};

Array2D maze2={
"#########################",
"# ......................#",
"#.###.#####.#.#####.###.#",
"#.###.#####.#.#####.###.#",
"#.###.......#.......###.#",
"#.###.##.#######.##.###.#",
"#.###.##....0....##.###.#",
"#.....##.#######.##.....#",
"#.###.##....#....##.###.#",
"#...#.#####.#.#####.#...#",
"###.#.#####.#.#####.#.###",
"#0......................#",
"#####.#.####.####.#.#####",
"#####.#.#       #.#.#####",
"#####.#.  ## ##  .#.#####",
"#####.#.# #   # #.#.#####",
"#####.#.# ##### #.#.#####",
"#.......#       #......0#",
"#.#####.#########.#####.#",
"#.....#.....#.....#.....#",
"#####.#.###.#.###.#.#####",
"#0..#.#.###.0.###.#.#...#",
"#.#.#.#.###.#.###.#.#.#.#",
"#.#.#.#.###.#.###.#.#.#.#",
"#.#.#.......#.......#.#0#",
"#.#.#####.#####.#####.#.#",
"#.#...................#.#",
"#.....######.######.....#",
"#.###.######.######.###.#",
"#.......................#",
"#########################"
};

Array2D maze3={
"#########################",
"#.......#####0###.......#",
"#.#####.....#.#...#.###.#",
"#.#   #.###.#.#.###.#0#.#",
"#.#####..#..........#.#.#",
"#.#......#.#.######.#.#.#",
"#.#.####.#.#....##......#",
"#.#....#.....##.##.####.#",
"#...####.###.##.##....#.#",
"###.#  #.#0#.##.##.####.#",
"###.####.#.........#  #.#",
"##.......#.#.#####.####.#",
"##.#####.###.#.#.#......#",
"##.#...#.....#.#.#.####.#",
"#....#...###.#.#.#....#.#",
"###.####.# #.#.#.#.####.#",
"###......#  .......#  #.#",
"###.####.# #.#####.####.#",
"###....#.###.##.........#",
"###.####.....##.##.###.##",
"#...#  #.###.##.##.#0#.##",
"###.####..#..##.##.#.#.##",
"#........###....##.#.#.##",
"#.#####...#..#####.#.#.##",
"#.#   #.####............#",
"#.#####...#..#.##.#####.#",
"#.#.....######.##.#.#...#",
"#.#0###.#......#......#.#",
"#.#.#.#.#.#.#####.#####.#",
"#...#.....#.............#",
"#########################",

};

Array2D* currMaze;


bool checkwall(int x,int y)
{
	if((*currMaze)[x][y]=='#')
		return true;
	else
		return false;
}

