#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "pacman.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#pragma warning(disable:4996)
using namespace sf;
using namespace std;

int pace = 250;
int  lives = 3, levels = 1, livesPL1 = 3, livesPL2 = 3;
int G1 = 600, G2 = 800, G3 = 1000, G4 = 500;
int counter1 = 0, counter2 = 1, counter3 = 1; //movement of ghosts
int selectedmode = -1, page = 2, character = 0, character2 = 0, choice = 0, firstChoice = 0, characterIndex = 0;
//select mode classic=0 1=twoplayers 
//page=2 selectmode page=3 select character
//choice=mainmenu buttons
//firstchoice=pages,0=mainmenu,1=leaderboard,2=credits,3=help
//charcterindex=by5zn fih al character 3shan yrsm 3liha box aswd lma y5tar tany character
int maxGameScore, score = 0, score1 = 100, score2 = 120, score3 = 100, highscore = 0, scorePLAYER1 = 0, scorePLAYER2 = 0;
//al scores msh hnst5dhma 3shan bnst5dm al dots
sf::String playerInput; //lazm nehawl mn string to text
sf::Text playerText;
string arr[7] = { "Thor.png","ironman.png","CaptainAmerica.png","Hulk.png","spiderman.png","WarMachine.png" };
bool soundON = true, YouWin = true, YouLose = true, start = false;
bool startGame = false, enterName = true, soundDuration = true, twoplayers = true;
int pacrotaion = 0, pac2rotation = 0;
sf::Vector2f pacScale(1, 1), pac2Scale(1, 1);
sf::Keyboard::Key direction; //direction pacman
sf::Keyboard::Key lastDirection, lastDirectionPL1, lastDirectionPL2;
vector <HIGH_SCORES> topScores; //al vector dh zy al string w by3ml save  ll name w al score
sf::Event evnt;
Position pacPos, ghost1, ghost2, ghost3, ghost4, pac2Pos;
int currPage = 1;
int dots = 0, dots1 = 296, dots2 = 314, dots3 = 335;

int moveplayer(int, int, int, int);
int moveplayer1(int, int, int, int);
int moveplayer2(int, int, int, int);
void resetMaze();
void resetMazeArray(void);
void drawBox(sf::RenderWindow& window, int, int, int, int);
//void drawWall(sf::RenderWindow &window);
void movePacman(sf::Keyboard::Key);
void movePacman1(sf::Keyboard::Key);
void movePacman2(sf::Keyboard::Key);
void ghostmovement1(Position*);
void ghostmovement2(Position*);
void ghostmovement3(Position*);
void ghostmovement4(Position*);
void chooseMode(sf::RenderWindow&, int&);
void drawMaze(sf::RenderWindow&);
void selectMode(sf::Keyboard::Key, int&);
void drawPage(sf::RenderWindow&, int, int);
void chooseCharacter(sf::RenderWindow&, int&, int);
void selectCharacter(sf::Keyboard::Key, int&, int);
void loadScore(vector <HIGH_SCORES>&);
void saveScore(vector <HIGH_SCORES>&, String, int);
void drawLeaderboard(sf::RenderWindow&);
void drawNameAndScore(vector <HIGH_SCORES>&, sf::RenderWindow&);
void drawMain(sf::RenderWindow&);
void selected(sf::Keyboard::Key keyPress, int&);
void loseLive(void);
void creditPage(sf::RenderWindow&);
void help(sf::RenderWindow&);
void message(string, sf::RenderWindow&);
void highscorePage(sf::RenderWindow&);
bool isHighScore(int score);
void EnterName(sf::RenderWindow&);
void resetGame();
void ghostmovement5(Position* ghost);
void ghostmovement6(Position* ghost);
void ghostmovement7(Position* ghost);
void ghostmovement8(Position* ghost);

int main()
{
	sf::Music music;
	music.openFromFile("baby shark.wav");
	music.play();

	sf::RenderWindow window(sf::VideoMode(2000, 1200), "Pacman", sf::Style::Close | sf::Style::Titlebar);
	sf::RenderWindow page2(sf::VideoMode(600, 300), "Choose Mode", sf::Style::Titlebar);
	sf::RenderWindow messageWindow(sf::VideoMode(400, 200), "MESSAGE", sf::Style::Titlebar);
	sf::RenderWindow ScoreWindow(sf::VideoMode(600, 300), "ENTER YOUR NAME", sf::Style::Titlebar);
	sf::RenderWindow mainMenuWindow(sf::VideoMode(1920, 1080), "ANGRY-MAN", sf::Style::Fullscreen);

	sf::RectangleShape photo(sf::Vector2f(1920.0f, 1080.0f));
	photo.setPosition(0, 0);
	sf::Texture Wallpaper;
	Wallpaper.loadFromFile("wallpaper.png");
	photo.setTexture(&Wallpaper);

	loadScore(topScores);

	lastDirection = sf::Keyboard::Y; //bndilo ay haga 3'ir right w left 3shan lma ybtdi ykon wa2f f al awl
	lastDirectionPL1 = sf::Keyboard::Y;
	lastDirectionPL2 = sf::Keyboard::Y;

	pacPos.x = 1;
	pacPos.y = 1;
	pac2Pos.x = 23;
	pac2Pos.y = 29;
	ghost1.x = 12; //level 1
	ghost1.y = 16;
	ghost2.x = 12;
	ghost2.y = 17;
	ghost3.x = 11;
	ghost3.y = 17;
	ghost4.x = 13;
	ghost4.y = 17;
	currMaze = &maze1;
	maxGameScore = score1;
	dots = dots1;

	sf::Thread t1(&ghostmovement1, &ghost1);
	sf::Thread t2(&ghostmovement2, &ghost2);
	sf::Thread t3(&ghostmovement3, &ghost3);
	sf::Thread t4(&ghostmovement4, &ghost4);
	sf::Thread t5(&movePacman1, evnt.key.code);
	sf::Thread t6(&ghostmovement5, &ghost1);
	sf::Thread t7(&ghostmovement6, &ghost2);
	sf::Thread t8(&ghostmovement7, &ghost3);
	sf::Thread t9(&ghostmovement8, &ghost4);
	mainMenuWindow.requestFocus();

	while (window.isOpen()) //window dih window al game
	{
		while (window.pollEvent(evnt))
		{
			if (evnt.type == evnt.Closed)
				window.close();
			else if (evnt.type == evnt.KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Escape)
				{
					music.play();
					resetMazeArray();
					resetGame();
					currPage = 1;
					mainMenuWindow.requestFocus();
				}
				else if (evnt.key.code == sf::Keyboard::P && start) //start bt5lih yehark al ghosts w al pacman ya ama true aw false
				{
					start = false;
					messageWindow.clear(sf::Color::Black);
					message(" GAME PAUSED", messageWindow);
					currPage = 5;
					messageWindow.requestFocus();
				}
				else
				{
				}
				messageWindow.display();
			}
		}
		if (start && selectedmode == 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				sf::sleep(sf::milliseconds(pace));
				lastDirection = evnt.key.code;
				movePacman(evnt.key.code);
			}
			else
			{
				sf::sleep(sf::milliseconds(pace));
				movePacman(lastDirection);
			}
		}
		else if (start && selectedmode == 1)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				sf::sleep(sf::milliseconds(pace));
				lastDirectionPL1 = evnt.key.code;
				movePacman1(evnt.key.code);
			}
			else
			{
				sf::sleep(sf::milliseconds(pace));
				movePacman1(lastDirectionPL1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				sf::sleep(sf::milliseconds(pace));

				lastDirectionPL2 = evnt.key.code;
				movePacman2(evnt.key.code);
			}
			else
			{
				sf::sleep(sf::milliseconds(pace));
				movePacman2(lastDirectionPL2);
			}
		}
		while (mainMenuWindow.pollEvent(evnt))
		{
			if (evnt.type == evnt.KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Escape)
				{
					ScoreWindow.close();
					music.pause();
					messageWindow.close();
					window.close();
					page2.close();
					mainMenuWindow.close();
				}
				else if (evnt.key.code == sf::Keyboard::S)
				{
					soundON = !soundON; //bool w m3naha ano lw true y5lih false w lw falsw y5lih true
					if (soundON)
						music.play();
					else
						music.pause();
				}
				else if (evnt.key.code == sf::Keyboard::Return && choice == 0) //return=enter
				{
					currPage = 2; //page2=choosemode
					page2.requestFocus();
				}
				else if (evnt.key.code == sf::Keyboard::B && firstChoice == 1)
				{
					firstChoice--;
					currPage = 1;
					mainMenuWindow.clear(sf::Color::Black);
				}
				else if (evnt.key.code == sf::Keyboard::B && firstChoice == 2)
				{
					//firstChoice--;
					firstChoice -= 2;
					mainMenuWindow.clear(sf::Color::Black);
				}
				else if (evnt.key.code == sf::Keyboard::B && firstChoice == 3)
				{
					//firstChoice--;
					firstChoice -= 3;
					mainMenuWindow.clear(sf::Color::Black);
				}
				else if (evnt.key.code == sf::Keyboard::Return && choice == 4)
				{
					ScoreWindow.close();
					music.pause();
					page2.close();
					window.close();
					messageWindow.close();
					mainMenuWindow.close();
				}
				else if (evnt.key.code == sf::Keyboard::Return && choice == 1)
				{
					if (firstChoice == 0)
					{
						firstChoice++;
					}
				}
				else if (evnt.key.code == sf::Keyboard::Return && choice == 2)
				{
					if (firstChoice == 0)
					{
						firstChoice += 2;
					}
				}
				else if (evnt.key.code == sf::Keyboard::Return && choice == 3)
				{
					if (firstChoice == 0)
					{
						firstChoice += 3;
					}
				}

				else
					selected(evnt.key.code, choice);
			}
			if (firstChoice == 0)
			{
				mainMenuWindow.clear(sf::Color::Black);
				mainMenuWindow.draw(photo);
				drawMain(mainMenuWindow);
			}
			else if (firstChoice == 1)
			{
				mainMenuWindow.clear(sf::Color::Black);
				mainMenuWindow.draw(photo);
				drawNameAndScore(topScores, mainMenuWindow);
				drawLeaderboard(mainMenuWindow);
			}
			else if (firstChoice == 2)
			{
				mainMenuWindow.clear(sf::Color::Black);
				mainMenuWindow.draw(photo);
				creditPage(mainMenuWindow);
			}
			else if (firstChoice == 3)
			{
				mainMenuWindow.clear(sf::Color::Black);
				mainMenuWindow.draw(photo);
				help(mainMenuWindow);
			}
			mainMenuWindow.display();
		}

		while (page2.pollEvent(evnt))
		{
			if (evnt.type == evnt.KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Return)
				{
					if (page == 2)
					{
						page2.setPosition(sf::Vector2i(300, 150));
						page2.setSize(sf::Vector2u(750, 400)); //ykbr al page
						page++; //3shan ytl3 choose character
						if (selectedmode == -1)
							selectedmode = 0;
						if (selectedmode == 0)
						{
							t1.launch();
							t2.launch();
							t3.launch();
							t4.launch();
						}
						else if (selectedmode == 1)
						{
							ghost1.x = 12;
							ghost1.y = 16;
							ghost2.x = 8;
							ghost2.y = 16;
							ghost3.x = 12;
							ghost3.y = 13;
							ghost4.x = 10;
							ghost4.y = 19;
							dots = dots3;

							t5.launch();
							t6.launch();
							t7.launch();
							t8.launch();
							t9.launch();
						}
					}
					else if (page == 3)
					{
						if (selectedmode == 0)	//classic
						{
							//resetGame();
							//currPage = 4;
							window.requestFocus();
							start = true;
							startGame = true;
							music.pause();
						}
						else if (selectedmode == 1)	//2 players
						{
							if (characterIndex == 0 && character == 0)
								character2++;
							characterIndex++;
							if (characterIndex == 2)
							{
								currPage = 4;
								window.requestFocus();
								start = true;
								startGame = true;
								levels = 3;
								currMaze = &maze3;
								music.pause();
							}
						}
					}
				}
				else if (page == 2)
					selectMode(evnt.key.code, selectedmode);
				else if (page == 3) //select character
				{
					if (characterIndex == 0)
						selectCharacter(evnt.key.code, character, -1); //-1 3shan msh hykarn haga b haga
					else if (characterIndex == 1)
						selectCharacter(evnt.key.code, character2, character);
				}
			}
		}
		while (messageWindow.pollEvent(evnt)) //message window bytb3tlha you win you lose next level game paused
		{
			if (evnt.type == evnt.KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::P)
				{
					currPage = 4;
					window.requestFocus();
					start = true;
				}
			}
		}
		while (ScoreWindow.pollEvent(evnt))
		{
			if (evnt.type == evnt.KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Return)
				{
					saveScore(topScores, playerInput, score);
					music.play();
					resetMazeArray();
					resetGame();
					playerInput = "";
					mainMenuWindow.requestFocus();
					ScoreWindow.clear(sf::Color::Black);
				}
				//playerInput="";
			}

			else if (evnt.type == sf::Event::TextEntered)
			{
				sf::Font font;
				font.loadFromFile("gamefont.ttf");
				playerText.setFont(font);
				playerText.setCharacterSize(20);
				playerText.setFillColor(sf::Color::Yellow);
				playerText.setStyle(sf::Text::Bold);
				playerText.setPosition(60, 170);
				if (evnt.text.unicode == 8 && playerInput.getSize() > 0) //8 =backspace ascii code
				{
					playerInput.erase(playerInput.getSize() - 1, playerInput.getSize());
				}
				else if (evnt.text.unicode < 128 && evnt.text.unicode != 13 && evnt.text.unicode != 8) //128=heroof bs
					//13=enter
					playerInput += (char)evnt.text.unicode;
				playerText.setString(playerInput);
				ScoreWindow.draw(playerText);
			}
		}
		drawPage(page2, page, characterIndex);
		page2.display();
		if (enterName) //entername=bool
		{
			highscorePage(ScoreWindow);
		}
		if (startGame)
			drawMaze(window);
		window.display();
		ScoreWindow.display(); //scorewindow bta3t enter your name

		if (selectedmode == 1 && twoplayers && (/*scorePLAYER1+scorePLAYER2>=score3*/dots <= 0 || livesPL1 <= 0 || livesPL2 <= 0))
		{
			if (twoplayers && (scorePLAYER1 > scorePLAYER2 || livesPL2 <= 0))
			{
				start = false;
				twoplayers = false;
				messageWindow.clear(sf::Color::Black);
				message("PLAYER 1 WINS", messageWindow);
				currPage = 5;
				messageWindow.requestFocus();
				messageWindow.display();
				resetMazeArray();
				resetGame();
				sf::sleep(sf::milliseconds(5000));
				currPage = 1;
				mainMenuWindow.requestFocus();
			}
			else if (twoplayers && (scorePLAYER2 > scorePLAYER1 || livesPL1 <= 0))
			{
				start = false;
				twoplayers = false;
				messageWindow.clear(sf::Color::Black);
				message("PLAYER 2 WINS", messageWindow);
				currPage = 5;
				messageWindow.requestFocus();
				messageWindow.display();
				sf::sleep(sf::milliseconds(5000));
				resetMazeArray();
				resetGame();
				currPage = 1;
				mainMenuWindow.requestFocus();
			}
			resetGame();
			evnt.key.code = sf::Keyboard::Z;
		}

		if (dots <= 0) //if(score>=maxGameScore)
		{
			if (levels >= 3 && YouWin) //if(score==score1+score2+score3 && YouWin)
			{
				YouWin = false;
				messageWindow.clear(sf::Color::Black);
				message("    YOU WIN  ", messageWindow);
				currPage = 5;
				messageWindow.requestFocus();
			}
			else if (levels < 3) //else if (score==score1 || score==score1+score2)
			{
				start = false;
				messageWindow.clear(sf::Color::Black);
				message("  NEXT LEVEL", messageWindow);
				currPage = 5;
				messageWindow.requestFocus();
			}
			sf::Music musicwin;
			musicwin.openFromFile("Victory.wav");
			if (levels >= 3 && soundDuration) //if(score==score1+score2+score3 && soundDuration)
			{
				soundDuration = false;
				musicwin.play();
			}
			sf::Music musiclevel;
			musiclevel.openFromFile("win.wav");
			if (levels < 3) //if(score==score1 || score==score1+score2)
				musiclevel.play();
			levels++;
			messageWindow.display();
			sf::sleep(sf::milliseconds(5000));
			resetMaze();
			currPage = 4;
			window.requestFocus();
			start = true;
			if (levels > 3 && isHighScore(score)) //if(score==score1+score2+score3 && isHighScore(score))
				//is highscore bykarn al score dh b al 5 score al ablo
			{
				enterName = false;
				currPage = 6;
				ScoreWindow.requestFocus();
			}
		}
		else if (lives <= 0 && YouLose)
		{
			sf::Music musiclose;
			musiclose.openFromFile("lai lai.wav");
			musiclose.play();
			YouLose = false;
			messageWindow.clear(sf::Color::Black);
			message("   YOU LOSE", messageWindow);
			currPage = 5;
			messageWindow.requestFocus();
			messageWindow.display();
			sf::sleep(sf::milliseconds(9000));
			if (isHighScore(score)) //if(score==score1+score2+score3 && isHighScore(score))
			{
				enterName = false;
				currPage = 6;
				ScoreWindow.requestFocus();
			}
			else
			{
				resetMazeArray();
				resetGame();
				currPage = 1;
				mainMenuWindow.requestFocus();
			}
		}
	}

	return 0;
}

void drawBox(sf::RenderWindow& window, int x, int y, int w, int l)
{
	sf::RectangleShape box(sf::Vector2f(w, l));
	box.setPosition(x, y);
	box.setFillColor(sf::Color::Black);
	window.draw(box);
}
void drawMaze(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);
	sf::Font font;
	font.loadFromFile("gamefont.ttf");
	char ss[20];

	drawBox(window, 120, 100, 20 * 26, 20 * 31);	// draw black box for the whole maze
	//position,m2as
	string s;
	if (levels == 1)
		s = "level1.png";
	else if (levels == 2)
		s = "level2.png";
	else if (levels >= 3)
		s = "level3.png";
	sf::Texture mazeShape;
	mazeShape.loadFromFile(s);
	sf::Sprite sprite; //sprite 3shan arsm sora
	sprite.setTexture(mazeShape);
	sprite.setPosition(120, 100);
	window.draw(sprite);

	//int ndots=0;
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			if (pacPos.x == j && pacPos.y == i)	// draw pacman
			{
				string text = arr[character];
				sf::Texture pacmanshape;
				pacmanshape.loadFromFile(text);
				sf::Sprite sprite;
				sprite.setTexture(pacmanshape);
				sprite.setPosition(j * 20 + 130, i * 20 + 110);
				sprite.setOrigin(12, 12);
				sprite.setRotation(pacrotaion);
				sprite.setScale(pacScale.x, pacScale.y);
				window.draw(sprite);

			}
			else if (selectedmode == 1 && pac2Pos.x == j && pac2Pos.y == i)	// draw pacman
			{
				string text = arr[character2];
				sf::Texture pacmanshape;
				pacmanshape.loadFromFile(text);
				sf::Sprite sprite;
				sprite.setTexture(pacmanshape);
				sprite.setPosition(j * 20 + 130, i * 20 + 110);
				sprite.setOrigin(12, 12);
				sprite.setRotation(pac2rotation);
				sprite.setScale(pac2Scale.x * -1, pac2Scale.y);
				window.draw(sprite);

			}
			else if ((ghost1.x == j && ghost1.y == i) ||	// draw ghosts
				(ghost2.x == j && ghost2.y == i) ||
				(ghost3.x == j && ghost3.y == i))

			{
				string text = "Loki.png";
				sf::Texture ghost1shape;
				ghost1shape.loadFromFile(text);
				sf::Sprite sprite;
				sprite.setTexture(ghost1shape);
				sprite.setPosition(j * 20 + 130, i * 20 + 110);
				sprite.setOrigin(12, 16);
				window.draw(sprite);
			}
			else if ((ghost4.x == j && ghost4.y == i))
			{
				string text = "thanos.png";
				sf::Texture ghost4shape;
				ghost4shape.loadFromFile(text);
				sf::Sprite sprite;
				sprite.setTexture(ghost4shape);
				sprite.setPosition(j * 20 + 130, i * 20 + 110);
				sprite.setOrigin(12, 16);
				window.draw(sprite);
			}
			else if ((*currMaze)[i][j] == '.')
			{
				//ndots++;
				sf::CircleShape dot(2);
				dot.setPosition(j * 20 + 8 + 120, i * 20 + 8 + 100);
				window.draw(dot);
			}
			else if ((*currMaze)[i][j] == '0')
			{
				//ndots++;

				sf::Texture coinshape;
				coinshape.loadFromFile("cooin.png");
				sf::Sprite sprite;
				sprite.setTexture(coinshape);
				sprite.setPosition(j * 20 + 120, i * 20 + 100);
				window.draw(sprite);
			}
		}
	}
	if (selectedmode == 0)
	{
		if (levels <= 3)
		{
			drawBox(window, 200, 50, 130, 50);
			sprintf_s(ss, "LEVEL %i", levels);  // write level number ...
			//ss=chararray lazm 3shan ttb3 variable adih ll array dh %mkan al variable al yethat
			sf::Text level;
			level.setFont(font);
			level.setString(ss);
			level.setFillColor(sf::Color::White);
			level.setStyle(sf::Text::Bold);
			level.setPosition(140, 50);
			window.draw(level);
		}

		sf::Text myscore;
		myscore.setFont(font);
		myscore.setString("YOUR SCORE ");
		myscore.setFillColor(sf::Color::White);
		myscore.setStyle(sf::Text::Bold);
		myscore.setPosition(700, 280);
		window.draw(myscore);

		drawBox(window, 1000, 260, 120, 120);
		sprintf_s(ss, "%i", score);
		sf::Text myscoree;
		myscoree.setString(ss);
		myscoree.setFont(font);
		myscoree.setFillColor(sf::Color::White);
		myscoree.setStyle(sf::Text::Bold);
		myscoree.setCharacterSize(30);
		myscoree.setPosition(1000, 278);
		window.draw(myscoree);

		sf::Text maxScore;
		maxScore.setFont(font);
		maxScore.setString("HIGHSCORE ");
		maxScore.setFillColor(sf::Color::White);
		maxScore.setStyle(sf::Text::Bold);
		maxScore.setPosition(400, 50);
		window.draw(maxScore);

		drawBox(window, 650, 30, 120, 50);
		sprintf_s(ss, "%i", highscore);
		sf::Text highs;
		highs.setString(ss);
		highs.setFont(font);
		highs.setFillColor(sf::Color::White);
		highs.setStyle(sf::Text::Bold);
		highs.setCharacterSize(30);
		highs.setPosition(670, 45);
		window.draw(highs);

		sf::Text live;
		live.setFont(font);
		live.setString("LIVES ");
		live.setFillColor(sf::Color::White);
		live.setStyle(sf::Text::Bold);
		live.setPosition(700, 350);
		window.draw(live);

		if (lives == 2)
		{
			drawBox(window, 980, 350, 40, 40); //arsm box aswd 3la al b3do
		}
		else if (lives == 1)
		{
			drawBox(window, 940, 350, 80, 40);
		}
		else if (lives == 0)
		{
			drawBox(window, 900, 350, 120, 40);
		}

		for (int i = 0; i < lives; i++)
		{
			drawBox(window, i * 40 + 900, 350, 40, 40);
			sf::Texture Lives;
			Lives.loadFromFile(arr[character]);
			sf::Sprite sprite;
			sprite.setTexture(Lives);
			sprite.setPosition(i * 40 + 900, 350);
			window.draw(sprite);
		}
	}
	else if (selectedmode == 1)
	{
		sf::Text scorePL1;
		scorePL1.setFont(font);
		scorePL1.setString("SCORE PLAYER 1 ");
		scorePL1.setFillColor(sf::Color::White);
		scorePL1.setCharacterSize(20);
		scorePL1.setStyle(sf::Text::Bold);
		scorePL1.setPosition(700, 180);
		window.draw(scorePL1);

		drawBox(window, 995, 160, 120, 120);
		sprintf_s(ss, "%i", scorePLAYER1);
		sf::Text player1;
		player1.setString(ss);
		player1.setFont(font);
		player1.setFillColor(sf::Color::White);
		player1.setStyle(sf::Text::Bold);
		player1.setCharacterSize(20);
		player1.setPosition(995, 178);
		window.draw(player1);

		sf::Text live1;
		live1.setFont(font);
		live1.setString("LIVES PLAYER 1 ");
		live1.setFillColor(sf::Color::White);
		live1.setStyle(sf::Text::Bold);
		live1.setCharacterSize(20);
		live1.setPosition(700, 250);
		window.draw(live1);

		for (int i = 0; i < livesPL1; i++)
		{
			drawBox(window, i * 40 + 950, 245, 40, 40);
			sf::Texture LivesPL;
			LivesPL.loadFromFile(arr[character]);
			sf::Sprite sprite;
			sprite.setTexture(LivesPL);
			sprite.setPosition(i * 40 + 950, 245);
			window.draw(sprite);
		}
		if (livesPL1 == 2)
		{
			drawBox(window, 1030, 245, 40, 40);
		}
		else if (livesPL1 == 1)
		{
			drawBox(window, 990, 245, 80, 40);
		}
		else if (livesPL1 == 0)
		{
			drawBox(window, 950, 245, 120, 40);
		}

		sf::Text scorePL2;
		scorePL2.setFont(font);
		scorePL2.setString("SCORE PLAYER 2 ");
		scorePL2.setFillColor(sf::Color::White);
		scorePL2.setCharacterSize(20);
		scorePL2.setStyle(sf::Text::Bold);
		scorePL2.setPosition(700, 380);
		window.draw(scorePL2);

		drawBox(window, 995, 360, 120, 120);
		sprintf_s(ss, "%i", scorePLAYER2);
		sf::Text player2;
		player2.setString(ss);
		player2.setFont(font);
		player2.setFillColor(sf::Color::White);
		player2.setStyle(sf::Text::Bold);
		player2.setCharacterSize(20);
		player2.setPosition(995, 380);
		window.draw(player2);

		sf::Text live2;
		live2.setFont(font);
		live2.setString("LIVES PLAYER 2 ");
		live2.setFillColor(sf::Color::White);
		live2.setStyle(sf::Text::Bold);
		live2.setCharacterSize(20);
		live2.setPosition(700, 450);
		window.draw(live2);

		for (int i = 0; i < livesPL2; i++)
		{
			drawBox(window, i * 40 + 950, 445, 40, 40);
			sf::Texture LivePL;
			LivePL.loadFromFile(arr[character2]);
			sf::Sprite sprite;
			sprite.setTexture(LivePL);
			sprite.setPosition(i * 40 + 950, 445);
			window.draw(sprite);
		}

		if (livesPL2 == 2)
		{
			drawBox(window, 1010, 445, 40, 40);
		}
		else if (livesPL2 == 1)
		{
			drawBox(window, 980, 445, 80, 40);
		}
		else if (livesPL2 == 0)
		{
			drawBox(window, 950, 445, 120, 40);
		}
	}
	sf::Text pause;
	pause.setFont(font);
	pause.setString("PRESS P TO PAUSE ");
	pause.setCharacterSize(10);
	pause.setFillColor(sf::Color::White);
	pause.setStyle(sf::Text::Bold);
	pause.setPosition(1160, 730);
	window.draw(pause);

	sf::Text exit;
	exit.setFont(font);
	exit.setString("PRESS ESC TO EXIT ");
	exit.setFillColor(sf::Color::White);
	exit.setCharacterSize(10);
	exit.setStyle(sf::Text::Bold);
	exit.setPosition(1160, 700);
	window.draw(exit);

}
void resetMaze(void) {
	if (levels == 2 && lives > 0)
	{
		lastDirection = sf::Keyboard::Y;
		currMaze = &maze2;
		dots = dots2;
		pacPos.x = 1;
		pacPos.y = 1;
		ghost1.x = 12;
		ghost1.y = 14;
		ghost2.x = 12;
		ghost2.y = 15;
		ghost3.x = 11;
		ghost3.y = 15;
		ghost4.x = 13;
		ghost4.y = 15;
		maxGameScore += score2;
		pacrotaion = 0; //nlfha b angle 90
		pacScale = sf::Vector2f(1, 1); //reflection
	}
	else if (levels == 3 && lives > 0)
	{
		lastDirection = sf::Keyboard::Y;
		currMaze = &maze3;
		dots = dots3;
		pacPos.x = 1;
		pacPos.y = 1;
		ghost1.x = 12;
		ghost1.y = 16;
		ghost2.x = 8;
		ghost2.y = 16;
		ghost3.x = 12;
		ghost3.y = 13;
		ghost4.x = 10;
		ghost4.y = 19;
		maxGameScore += score3;
		pacrotaion = 0;
		pacScale = sf::Vector2f(1, 1);
	}
}
int moveplayer(int x, int y, int x2, int y2)
{
	int ret = 0;
	if ((*currMaze)[y2][x2] != '#')
	{
		if ((*currMaze)[y2][x2] == ' ')
		{
			ret = 1;
		}
		else if ((*currMaze)[y2][x2] == '.') {  //CHECK DOT..
			(*currMaze)[y2][x2] = 'D';
			score = score + 10;
			dots--;
			ret = 2;
		}
		else if ((*currMaze)[y2][x2] == '0') {   //CHECK COIN..
			(*currMaze)[y2][x2] = 'O';
			score = score + 100;
			dots--;
			ret = 3;
		}
		else if ((x2 == ghost1.x && y2 == ghost1.y) ||  //CHECK GHOST
			(x2 == ghost2.x && y2 == ghost2.y) ||
			(x2 == ghost3.x && y2 == ghost3.y) ||
			(x2 == ghost4.x && y2 == ghost4.y))
		{
			lives--;
			loseLive();
			sf::sleep(sf::milliseconds(pace));
			ret = 4;
		}
		pacPos.x = x2;
		pacPos.y = y2;
	}
	return ret;
}
void movePacman(sf::Keyboard::Key keyCode) {

	static sf::Keyboard::Key previous = sf::Keyboard::Right;
	int x2, y2;
	int direction = 0;
	if (lives == 0 || dots <= 0/*score >= maxGameScore*/)
		return;
	if (keyCode == sf::Keyboard::Down) {    //MOVE DOWN..

		direction = sf::Keyboard::Down;
		y2 = pacPos.y + 1;
		if (moveplayer(pacPos.x, pacPos.y, pacPos.x, y2))
		{
			pacrotaion = 90;
			if (previous == sf::Keyboard::Right)
			{
				pacrotaion = 90;
				pacScale = sf::Vector2f(1, 1);
			}
			else if (previous == sf::Keyboard::Left)
			{
				pacrotaion = 90;
				pacScale = sf::Vector2f(1, -1);
			}
			else if (previous == sf::Keyboard::Up)
			{
				pacScale.y *= -1;
			}
			previous = sf::Keyboard::Down;
		}
	}
	else if (keyCode == sf::Keyboard::Up) {    //MOVE UP
		direction = sf::Keyboard::Up;
		y2 = pacPos.y - 1;
		if (moveplayer(pacPos.x, pacPos.y, pacPos.x, y2))
		{
			pacrotaion = -90;
			if (previous == sf::Keyboard::Right)
			{
				pacrotaion = -90;
				pacScale = sf::Vector2f(1, 1);
			}
			else if (previous == sf::Keyboard::Left)
			{
				pacrotaion = -90;
				pacScale = sf::Vector2f(1, -1);
			}
			else if (previous == sf::Keyboard::Down)
			{
				pacScale.y *= -1;
			}
			previous = sf::Keyboard::Up;
		}
	}
	else if (keyCode == sf::Keyboard::Right) {     //MOVE RIGHT
		direction = sf::Keyboard::Right;
		x2 = pacPos.x + 1;
		if (moveplayer(pacPos.x, pacPos.y, x2, pacPos.y))
		{
			pacrotaion = 0;
			{
				pacrotaion = 0;
				pacScale = sf::Vector2f(1, 1);
			}
			previous = sf::Keyboard::Right;
		}
	}
	else if (keyCode == sf::Keyboard::Left) {      //MOVE LEFT
		direction = sf::Keyboard::Left;
		x2 = pacPos.x - 1;
		if (moveplayer(pacPos.x, pacPos.y, x2, pacPos.y))
		{
			pacrotaion = 0;
			{
				pacrotaion = 0;
				pacScale = sf::Vector2f(-1, 1);
			}
			previous = sf::Keyboard::Left;
		}
	}

}
void ghostmovement1(Position* ghost)
{
	sf::sleep(sf::milliseconds(3000));
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	if (levels == 2)
		G1 = 550;
	else if (levels == 3)
		G1 = 500;
	int  direction, up, down, right, left;
	if (levels == 1)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up)	//Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G1));
			ghost->y--;
		}
	}
	else if (levels == 2)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up)	//Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G1));
			ghost->y--;
		}
	}
	else if (levels == 3)
	{
		direction = 2, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right)	//Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G1));
			ghost->x++;
		}
	}

	srand(time(0));
	while (startGame && lives > 0 && levels <= 3)
	{
		if (counter1 % 2 == 0)
		{
			if (pacPos.y == ghost1.y)
			{
				if (pacPos.x < ghost1.x)
				{
					if (checkwall(ghost1.y, ghost1.x - 1) == false)
						direction = left;
					else if (checkwall(ghost1.y - 1, ghost1.x) == false)
						direction = up;
					else if (checkwall(ghost1.y + 1, ghost1.x) == false)
						direction = down;
					else
						direction = right;
				}
				else if (pacPos.x > ghost1.x)
				{
					if (checkwall(ghost1.y, ghost1.x + 1) == false)
						direction = right;
					else if (checkwall(ghost1.y - 1, ghost1.x) == false)
						direction = up;
					else if (checkwall(ghost1.y + 1, ghost1.x) == false)
						direction = down;
					else
						direction = left;
				}

			}
			if (pacPos.x == ghost1.x) {
				if (pacPos.y < ghost1.y)
				{
					if (checkwall(ghost1.y - 1, ghost1.x) == false)
						direction = up;
					else if (checkwall(ghost1.y, ghost1.x + 1) == false)
						direction = right;
					else if (checkwall(ghost1.y, ghost1.x - 1) == false)
						direction = left;
					else
						direction = down;
				}
				else if (pacPos.y > ghost1.y) {
					if (checkwall(ghost1.y + 1, ghost1.x) == false)
						direction = down;
					else if (checkwall(ghost1.y, ghost1.x + 1) == false)
						direction = right;
					else if (checkwall(ghost1.y, ghost1.x - 1) == false)
						direction = left;
					else
						direction = up;
				}
			}
			else if (pacPos.y < ghost1.y)
			{
				if (checkwall(ghost1.y - 1, ghost1.x) == false)
					direction = up;
				else if (checkwall(ghost1.y, ghost1.x - 1) == false)
					direction = left;
				else if (checkwall(ghost1.y, ghost1.x + 1) == false)
					direction = right;
				else
					direction = down;
			}
			else if (pacPos.y > ghost1.y)
			{
				if (checkwall(ghost1.y + 1, ghost1.x) == false)
					direction = down;
				else if (checkwall(ghost1.y, ghost1.x - 1) == false)
					direction = left;
				else if (checkwall(ghost1.y, ghost1.x + 1) == false)
					direction = right;
				else
					direction = up;
			}
			else if (pacPos.x < ghost1.x)
			{
				if (checkwall(ghost1.y, ghost1.x - 1) == false)
					direction = left;
				else if (checkwall(ghost1.y - 1, ghost1.x) == false)
					direction = up;
				else if (checkwall(ghost1.y + 1, ghost1.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pacPos.x > ghost1.x)
			{
				if (checkwall(ghost1.y, ghost1.x + 1) == false)
					direction = right;
				else if (checkwall(ghost1.y - 1, ghost1.x) == false)
					direction = up;
				else if (checkwall(ghost1.y + 1, ghost1.x) == false)
					direction = down;
				else
					direction = left;

			}
			counter1++;
		}
		else
		{
			direction = rand() % 4;
			if (direction == up && checkwall(ghost1.y - 1, ghost1.x) == false)
				counter1++;
			else if (direction == down && checkwall(ghost1.y + 1, ghost1.x) == false)
				counter1++;
			else if (direction == left && checkwall(ghost1.y, ghost1.x - 1) == false)
				counter1++;
			else if (direction == right && checkwall(ghost1.y, ghost1.x + 1) == false)
				counter1++;
		}
		while (lives > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G1));
			ghost->y--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}

		}
		while (lives > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			if (levels == 1)
			{
				if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			if (levels == 2)
			{
				if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			sf::sleep(sf::milliseconds(G1));
			ghost->y++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G1));
			ghost->x++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G1));
			ghost->x--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void ghostmovement2(Position* ghost)
{
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	if (levels == 2)
		G2 = 600;
	else if (levels == 3)
		G2 = 500;
	int  direction, up, down, right, left;
	if (levels == 1)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G2));
			ghost->y--;
		}
	}
	else if (levels == 2)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G2));
			ghost->y--;
		}
	}
	else if (levels == 3)
	{
		direction = 2, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G2));
			ghost->x++;
		}
	}
	srand(time(0));
	while (startGame && lives > 0 && levels <= 3)
	{
		if (counter2 % 3 != 0)
		{
			if (pacPos.y == ghost2.y)
			{
				if (pacPos.x < ghost2.x)
				{
					if (checkwall(ghost2.y, ghost2.x - 1) == false)
						direction = left;
					else if (checkwall(ghost2.y - 1, ghost2.x) == false)
						direction = up;
					else if (checkwall(ghost2.y + 1, ghost2.x) == false)
						direction = down;
					else
						direction = right;
				}
				else if (pacPos.x > ghost2.x)
				{
					if (checkwall(ghost2.y, ghost2.x + 1) == false)
						direction = right;
					else if (checkwall(ghost2.y - 1, ghost2.x) == false)
						direction = up;
					else if (checkwall(ghost2.y + 1, ghost2.x) == false)
						direction = down;
					else
						direction = left;
				}
			}
			if (pacPos.x == ghost2.x) {
				if (pacPos.y < ghost2.y)
				{
					if (checkwall(ghost2.y - 1, ghost2.x) == false)
						direction = up;
					else if (checkwall(ghost2.y, ghost2.x + 1) == false)
						direction = right;
					else if (checkwall(ghost2.y, ghost2.x - 1) == false)
						direction = left;
					else
						direction = down;
				}
				else if (pacPos.y > ghost2.y) {
					if (checkwall(ghost2.y + 1, ghost2.x) == false)
						direction = down;
					else if (checkwall(ghost2.y, ghost2.x + 1) == false)
						direction = right;
					else if (checkwall(ghost2.y, ghost2.x - 1) == false)
						direction = left;
					else
						direction = up;
				}
			}
			else if (pacPos.y < ghost2.y)
			{
				if (checkwall(ghost2.y - 1, ghost2.x) == false)
					direction = up;
				else if (checkwall(ghost2.y, ghost2.x - 1) == false)
					direction = left;
				else if (checkwall(ghost2.y, ghost2.x + 1) == false)
					direction = right;
				else
					direction = down;
			}
			else if (pacPos.y > ghost2.y)
			{
				if (checkwall(ghost2.y + 1, ghost2.x) == false)
					direction = down;
				else if (checkwall(ghost2.y, ghost2.x - 1) == false)
					direction = left;
				else if (checkwall(ghost2.y, ghost2.x + 1) == false)
					direction = right;
				else
					direction = up;
			}
			else if (pacPos.x < ghost2.x)
			{
				if (checkwall(ghost2.y, ghost2.x - 1) == false)
					direction = left;
				else if (checkwall(ghost2.y - 1, ghost2.x) == false)
					direction = up;
				else if (checkwall(ghost2.y + 1, ghost2.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pacPos.x > ghost2.x)
			{
				if (checkwall(ghost2.y, ghost2.x + 1) == false)
					direction = right;
				else if (checkwall(ghost2.y - 1, ghost2.x) == false)
					direction = up;
				else if (checkwall(ghost2.y + 1, ghost2.x) == false)
					direction = down;
				else
					direction = left;
			}
			counter2++;
		}
		else
		{
			direction = rand() % 4;
			if (direction == up && checkwall(ghost2.y - 1, ghost2.x) == false)
				counter2++;
			else if (direction == down && checkwall(ghost2.y + 1, ghost2.x) == false)
				counter2++;
			else if (direction == left && checkwall(ghost2.y, ghost2.x - 1) == false)
				counter2++;
			else if (direction == right && checkwall(ghost2.y, ghost2.x + 1) == false)
				counter2++;
		}
		while (lives > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G2));
			ghost->y--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			if (levels == 1)
			{
				if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			if (levels == 2)
			{
				if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			sf::sleep(sf::milliseconds(G2));
			ghost->y++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G2));
			ghost->x++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G2));
			ghost->x--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void ghostmovement3(Position* ghost)
{
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	if (levels == 2)
		G3 = 900;
	else if (levels == 3)
		G3 = 800;
	int  direction, up, down, right, left;
	if (levels == 1)
	{
		direction = 2, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
			direction = up;
		}
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y--;
		}
	}
	else if (levels == 2)
	{
		direction = 2, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
			direction = up;
		}
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y--;
		}
	}
	else if (levels == 3)
	{
		direction = 1, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y + 1, ghost->x) == false && direction == down && start) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y++;
			direction = right;
		}
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
		}
	}
	srand(time(0));
	while (startGame && lives > 0 && levels <= 3)
	{
		if (counter3 % 4 != 0)
		{
			if (pacPos.y == ghost3.y)
			{
				if (pacPos.x < ghost3.x)
				{
					if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else
						direction = right;
				}
				else if (pacPos.x > ghost3.x)
				{
					if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else
						direction = left;
				}
			}
			if (pacPos.x == ghost1.x) {
				if (pacPos.y < ghost3.y)
				{
					if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else
						direction = down;
				}
				else if (pacPos.y > ghost3.y) {
					if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else
						direction = up;
				}
			}
			else if (pacPos.y < ghost3.y)
			{
				if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else
					direction = down;
			}
			else if (pacPos.y > ghost3.y)
			{
				if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else
					direction = up;
			}
			else if (pacPos.x < ghost3.x)
			{
				if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pacPos.x > ghost3.x)
			{
				if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else
					direction = left;
			}
			counter3++;
		}
		else
		{
			direction = rand() % 4;
			if (direction == up && checkwall(ghost3.y - 1, ghost3.x) == false)
				counter3++;
			else if (direction == down && checkwall(ghost3.y + 1, ghost3.x) == false)
				counter3++;
			else if (direction == left && checkwall(ghost3.y, ghost3.x - 1) == false)
				counter3++;
			else if (direction == right && checkwall(ghost3.y, ghost3.x + 1) == false)
				counter3++;
		}
		while (lives > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				musiclose.play();
				loseLive();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			if (levels == 1)
			{
				if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			if (levels == 2)
			{
				if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			sf::sleep(sf::milliseconds(G3));
			ghost->y++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G3));
			ghost->x--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				lives--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void ghostmovement4(Position* ghost)
{
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	if (levels == 2)
		G4 = 400;
	else if (levels == 3)
		G4 = 300;
	int  direction, up, down, right, left;
	if (levels == 1)
	{
		direction = 3, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y, ghost->x - 1) == false && direction == left && start) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x--;
			direction = up;
		}
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
		}
	}
	else if (levels == 2)
	{
		direction = 3, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y, ghost->x - 1) == false && direction == left && start) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x--;
			direction = up;
		}
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
		}
	}
	else if (levels == 3)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
			direction = right;
		}
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x++;
		}
	}
	srand(time(0));
	while (startGame && lives > 0 && levels <= 3)
	{
		if (pacPos.y == ghost4.y)
		{
			if (pacPos.x < ghost4.x)
			{
				if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pacPos.x > ghost4.x)
			{
				if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else
					direction = left;
			}
		}
		if (pacPos.x == ghost4.x) {
			if (pacPos.y < ghost4.y)
			{
				if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else
					direction = down;
			}
			else if (pacPos.y > ghost4.y) {
				if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else
					direction = up;
			}
		}
		else if (pacPos.y < ghost4.y)
		{
			if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else
				direction = down;
		}
		else if (pacPos.y > ghost4.y)
		{
			if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else
				direction = up;
		}
		else if (pacPos.x < ghost4.x)
		{
			if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else
				direction = right;
		}
		else if (pacPos.x > ghost4.x)
		{
			if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else
				direction = left;
		}
		while (lives > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (score >= 100)
				{
					score -= 100;
					loseLive();
					break;
				}
				else
				{
					lives--;
					loseLive();
					musiclose.play();
				}
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (score >= 100)
				{
					score -= 100;
					loseLive();
					break;
				}
				else
				{
					lives--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			if (levels == 1)
			{
				if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			if (levels == 2)
			{
				if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
				{
					if (counter1 % 2 == 0)
						direction = right;
					else
						direction = left;
					break;
				}
			}
			sf::sleep(sf::milliseconds(G4));
			ghost->y++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (score >= 100)
				{
					score -= 100;
					loseLive();
					break;
				}
				else
				{
					lives--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (lives > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G4));
			ghost->x--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (score >= 100)
				{
					score -= 100;
					loseLive();
					musiclose.play();
					break;
				}
				else
				{
					lives--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void drawPage(sf::RenderWindow& page2, int page, int characterIndex)
{
	page2.clear(sf::Color::Black);
	if (page == 2)
		chooseMode(page2, selectedmode);
	else if (page == 3)
	{
		if (characterIndex == 0)
			chooseCharacter(page2, character, -1);
		else if (characterIndex == 1)
			chooseCharacter(page2, character2, character);
	}
}
void chooseCharacter(sf::RenderWindow& page2, int& chracter, int excludedcharacter) {
	sf::Font font;
	font.loadFromFile("gamefont.ttf");
	sf::Text mode;
	mode.setFont(font);
	string s = "CHOOSE CHARACTER";
	if (selectedmode == 1)
	{
		if (characterIndex == 0)
			s += " 1";
		else if (characterIndex == 1)
			s += " 2";
	}
	mode.setString(s);
	mode.setFillColor(sf::Color::Yellow);
	mode.setStyle(sf::Text::Bold);
	mode.setPosition(130, 25);
	page2.draw(mode);

	for (int i = 0; i < 6; i++)
	{
		if (i == excludedcharacter) //bta3 -1 aw character
		{
			drawBox(page2, i * 90 + 45, 120, 50, 50);
			continue;
		}
		sf::RectangleShape box(sf::Vector2f(50, 50));
		box.setFillColor(sf::Color::Black);
		box.setOutlineThickness(5);
		if (chracter == i)
			box.setOutlineColor(sf::Color::Red);
		else
			box.setOutlineColor(sf::Color::Black);
		box.setPosition(i * 90 + 45, 120);
		page2.draw(box);

		string text = arr[i];
		sf::Texture pacmanshape;
		pacmanshape.loadFromFile(text);
		sf::Sprite sprite;
		sprite.setTexture(pacmanshape);
		sprite.setPosition(i * 90 + 70, 145);
		sprite.setOrigin(12, 12);
		page2.draw(sprite);

	}

}
void selectCharacter(sf::Keyboard::Key keycode, int& selectedcharacter, int excludedcharacter)
{
	if (keycode == sf::Keyboard::Right && selectedcharacter < 5)
	{
		selectedcharacter++;
		if (excludedcharacter == selectedcharacter)
			if (selectedcharacter < 5)
				selectedcharacter++;
			else
				selectedcharacter--;
	}
	else if (keycode == sf::Keyboard::Left && selectedcharacter > 0)
	{
		selectedcharacter--;
		if (excludedcharacter == selectedcharacter)
			if (selectedcharacter > 0)
				selectedcharacter--;
			else
				selectedcharacter++;
	}
}
void chooseMode(sf::RenderWindow& page2, int& selectedmode)
{
	sf::Font font;
	font.loadFromFile("gamefont.ttf");
	sf::Text mode;
	mode.setFont(font);
	mode.setString("CHOOSE MODE ");
	mode.setFillColor(sf::Color::Yellow);
	mode.setStyle(sf::Text::Bold);
	mode.setPosition(155, 25);
	page2.draw(mode);

	sf::RectangleShape box(sf::Vector2f(200, 100));
	box.setOutlineThickness(5);
	box.setFillColor(sf::Color::Black);
	if (selectedmode == 1)
		box.setOutlineColor(sf::Color::White);
	else
		box.setOutlineColor(sf::Color::Red);
	box.setPosition(60, 120);
	page2.draw(box);

	sf::RectangleShape box1(sf::Vector2f(200, 100));
	box1.setOutlineThickness(5);
	box1.setFillColor(sf::Color::Black);
	if (selectedmode == 1)
		box1.setOutlineColor(sf::Color::Red);
	else
		box1.setOutlineColor(sf::Color::White);
	box1.setPosition(340, 120);
	page2.draw(box1);

	sf::Text classic;
	classic.setFont(font);
	classic.setString("CLASSIC ");
	if (selectedmode == 1)
		classic.setFillColor(sf::Color::White);
	else
		classic.setFillColor(sf::Color::Red);
	classic.setStyle(sf::Text::Bold);
	classic.setPosition(80, 150);
	page2.draw(classic);

	sf::Text multi;
	multi.setFont(font);
	multi.setString("TWO ");
	if (selectedmode == 1)
		multi.setFillColor(sf::Color::Red);
	else
		multi.setFillColor(sf::Color::White);
	multi.setStyle(sf::Text::Bold);
	multi.setPosition(400, 137);
	page2.draw(multi);

	sf::Text player;
	player.setFont(font);
	player.setString("PLAYERS ");
	if (selectedmode == 1)
		player.setFillColor(sf::Color::Red);
	else
		player.setFillColor(sf::Color::White);;
	player.setStyle(sf::Text::Bold);
	player.setPosition(360, 172);
	page2.draw(player);


}
void selectMode(sf::Keyboard::Key keycode, int& selectedmode)
{
	if (keycode == sf::Keyboard::Right)
		selectedmode = 1;
	else if (keycode == sf::Keyboard::Left)
		selectedmode = 0;
}
void loadScore(vector <HIGH_SCORES>& topScores)
{
	HIGH_SCORES compare;
	ifstream infile("thefile.txt");
	string line;
	while (getline(infile, line)) //check fi space wla la
	{
		size_t pos = line.find('@');
		if (pos != string::npos)
		{
			strcpy_s(compare.name, line.substr(0, pos).c_str());
			string s = line.substr(pos + 1);
			sscanf_s(s.c_str(), "%i", &compare.score);
			topScores.push_back(compare);
		}

	}
	if (topScores.size() > 0)
		highscore = topScores[0].score;

	infile.close();
}
void saveScore(vector <HIGH_SCORES>& topScores, String Input, int score5)
{
	HIGH_SCORES high_score;
	strcpy_s(high_score.name, Input.toAnsiString().c_str());
	high_score.score = score5;

	bool inserted = false;
	int n = topScores.size();
	for (int i = 0; i < n; i++)
	{
		if (topScores[i].score <= score5)
		{
			topScores.insert(topScores.begin() + i, high_score);
			inserted = true;
			break;
		}
	}
	if (!inserted)
		topScores.push_back(high_score);
	n++;
	if (n > 5)
		topScores.pop_back();

	FILE* obj = fopen("thefile.txt", "w");
	for (int i = 0; i < n; i++)
	{
		fprintf(obj, "%s@", topScores[i].name);
		fprintf(obj, "%i\n", topScores[i].score);
	}
	highscore = topScores[0].score;
	fclose(obj);
}
void drawLeaderboard(sf::RenderWindow& leaderborad)
{
	char ss[20];

	sf::Font font;
	font.loadFromFile("CaviarDreams.ttf");
	sf::Text hscores;
	hscores.setFont(font);
	hscores.setString("HIGH SCORES ");
	hscores.setCharacterSize(100);
	hscores.setFillColor(sf::Color::Yellow);
	hscores.setStyle(sf::Text::Bold);
	hscores.setPosition(300, 40);
	leaderborad.draw(hscores);

	sf::Text name;
	name.setFont(font);
	name.setString("NAME ");
	name.setCharacterSize(50);
	name.setFillColor(sf::Color::Red);
	name.setStyle(sf::Text::Bold);
	name.setPosition(220, 180);
	leaderborad.draw(name);

	sf::Text scoree;
	scoree.setFont(font);
	scoree.setString("SCORE ");
	scoree.setCharacterSize(50);
	scoree.setFillColor(sf::Color::Red);
	scoree.setStyle(sf::Text::Bold);
	scoree.setPosition(900, 180);
	leaderborad.draw(scoree);

	sf::Text back;
	back.setFont(font);
	back.setString("PRESS B TO BACK ");
	back.setCharacterSize(18);
	back.setFillColor(sf::Color::White);
	back.setStyle(sf::Text::Bold);
	back.setPosition(1100, 720);
	leaderborad.draw(back);

}
void drawNameAndScore(vector <HIGH_SCORES>& topScores, sf::RenderWindow& leaderborad)
{
	char line[100];
	sf::Font font;
	font.loadFromFile("CaviarDreams.ttf");
	for (int i = 0; i < topScores.size(); i++)
	{
		sprintf_s(line, "%i       %s", i + 1, topScores[i].name);
		sf::Text linee;
		linee.setFont(font);
		linee.setString(line);
		linee.setCharacterSize(50);
		linee.setFillColor(sf::Color::White);
		linee.setStyle(sf::Text::Bold);
		linee.setPosition(60, i * 70 + 250);
		leaderborad.draw(linee);

		sprintf_s(line, "%i", topScores[i].score);
		sf::Text scr;
		scr.setFont(font);
		scr.setString(line);
		scr.setCharacterSize(50);
		scr.setFillColor(sf::Color::White);
		scr.setStyle(sf::Text::Bold);
		scr.setPosition(920, i * 70 + 250);
		leaderborad.draw(scr);
	}
}
void drawMain(sf::RenderWindow& mainMenu) {

	sf::Font font;
	font.loadFromFile("gamefont.ttf");

	sf::Text title;
	title.setFont(font);
	title.setString("PACMAN");
	title.setFillColor(sf::Color::Yellow);
	title.setStyle(sf::Text::Bold);
	title.setPosition(400, 70);
	title.setCharacterSize(80);
	mainMenu.draw(title);

	sf::Text title2;
	title2.setFont(font);
	title2.setString("THE SECOND AVENGER");
	title2.setFillColor(sf::Color::Yellow);
	title2.setStyle(sf::Text::Bold);
	title2.setPosition(300, 180);
	title2.setCharacterSize(40);
	mainMenu.draw(title2);

	sf::Text play;
	play.setFont(font);
	play.setString("Play");
	if (choice == 0) {
		play.setFillColor(sf::Color::Red);
	}
	else {
		play.setFillColor(sf::Color::White);
	}
	play.setStyle(sf::Text::Bold);
	play.setPosition(100, 250);
	play.setCharacterSize(60);
	mainMenu.draw(play);

	sf::Text leaderboard;
	leaderboard.setFont(font);
	leaderboard.setString("Leaderboard");
	if (choice == 1)
	{
		leaderboard.setFillColor(sf::Color::Red);
	}
	else
	{
		leaderboard.setFillColor(sf::Color::White);
	}
	leaderboard.setStyle(sf::Text::Bold);
	leaderboard.setPosition(100, 350);
	leaderboard.setCharacterSize(60);
	mainMenu.draw(leaderboard);

	sf::Text credits;
	credits.setFont(font);
	credits.setString("Credits");
	if (choice == 2) {
		credits.setFillColor(sf::Color::Red);
	}
	else {
		credits.setFillColor(sf::Color::White);
	}
	credits.setStyle(sf::Text::Bold);
	credits.setPosition(100, 450);
	credits.setCharacterSize(60);
	mainMenu.draw(credits);

	sf::Text help;
	help.setFont(font);
	help.setString("Help");
	if (choice == 3) {
		help.setFillColor(sf::Color::Red);
	}
	else {
		help.setFillColor(sf::Color::White);
	}
	help.setStyle(sf::Text::Bold);
	help.setPosition(100, 550);
	help.setCharacterSize(60);
	mainMenu.draw(help);

	sf::Text exit;
	exit.setFont(font);
	exit.setString("Exit");
	if (choice == 4) {
		exit.setFillColor(sf::Color::Red);
	}
	else {
		exit.setFillColor(sf::Color::White);
	}
	exit.setStyle(sf::Text::Bold);
	exit.setPosition(100, 650);
	exit.setCharacterSize(60);
	mainMenu.draw(exit);

	sf::Text sound;
	sound.setFont(font);
	sound.setString("PRESS S TO ENABLE or DISABLE SOUND ");
	sound.setCharacterSize(10);
	if (soundON)
		sound.setFillColor(sf::Color::Red);
	else
		sound.setFillColor(sf::Color::White);
	sound.setStyle(sf::Text::Bold);
	sound.setPosition(1000, 720);
	mainMenu.draw(sound);

}
void selected(sf::Keyboard::Key keyPress, int& choice) {
	if (keyPress == sf::Keyboard::Down && choice < 4) {
		choice++;
	}
	else if (keyPress == sf::Keyboard::Up && choice > 0) {
		choice--;
	}
}
void loseLive(void)
{

	if (levels == 1)
	{
		pacPos.x = 1;
		pacPos.y = 1;
		ghost1.x = 12;
		ghost1.y = 16;
		ghost2.x = 12;
		ghost2.y = 17;
		ghost3.x = 11;
		ghost3.y = 17;
		ghost4.x = 13;
		ghost4.y = 17;
		pacrotaion = 0;
		pacScale = sf::Vector2f(1, 1);
	}
	else if (levels == 2)
	{
		pacPos.x = 1;
		pacPos.y = 1;
		ghost1.x = 12;
		ghost1.y = 14;
		ghost2.x = 12;
		ghost2.y = 15;
		ghost3.x = 11;
		ghost3.y = 15;
		ghost4.x = 13;
		ghost4.y = 15;
		pacrotaion = 0;
		pacScale = sf::Vector2f(1, 1);
	}
	else if (levels == 3)
	{
		pacPos.x = 1;
		pacPos.y = 1;
		ghost1.x = 11;
		ghost1.y = 16;
		ghost2.x = 10;
		ghost2.y = 16;
		ghost3.x = 10;
		ghost3.y = 15;
		ghost4.x = 10;
		ghost4.y = 17;
		pacrotaion = 0;
		pacScale = sf::Vector2f(1, 1);
	}
}
void creditPage(sf::RenderWindow& credits)
{
	sf::Font font;
	font.loadFromFile("CaviarDreams.ttf");
	sf::Text credit;
	credit.setFont(font);
	credit.setString("CREDITS ");
	credit.setCharacterSize(100);
	credit.setFillColor(sf::Color::Yellow);
	credit.setStyle(sf::Text::Bold);
	credit.setPosition(450, 50);
	credits.draw(credit);

	sf::Text line1;
	line1.setFont(font);
	line1.setString("Game inspired by Pacman Original");
	line1.setCharacterSize(40);
	line1.setFillColor(sf::Color::White);
	line1.setStyle(sf::Text::Bold);
	line1.setPosition(100, 200);
	credits.draw(line1);

	sf::Text line2;
	line2.setFont(font);
	line2.setString("Made by : Omar Khaled / Monica Milad");
	line2.setCharacterSize(40);
	line2.setFillColor(sf::Color::White);
	line2.setStyle(sf::Text::Bold);
	line2.setPosition(100, 300);
	credits.draw(line2);

	sf::Text line3;
	line3.setFont(font);
	line3.setString("Kirollos Isaac / Andrew Atef");
	line3.setCharacterSize(40);
	line3.setFillColor(sf::Color::White);
	line3.setStyle(sf::Text::Bold);
	line3.setPosition(330, 350);
	credits.draw(line3);

	sf::Text line4;
	line4.setFont(font);
	line4.setString("Muhammed Ahmed / Moamen Muhammed");
	line4.setCharacterSize(40);
	line4.setFillColor(sf::Color::White);
	line4.setStyle(sf::Text::Bold);
	line4.setPosition(330, 400);
	credits.draw(line4);

	sf::Text line7;
	line7.setFont(font);
	line7.setString("Date Released :  9 / 4 / 2019");
	line7.setCharacterSize(40);
	line7.setFillColor(sf::Color::White);
	line7.setStyle(sf::Text::Bold);
	line7.setPosition(100, 500);
	credits.draw(line7);

	sf::Text line8;
	line8.setFont(font);
	line8.setString("Game Version : 1.0.0");
	line8.setCharacterSize(40);
	line8.setFillColor(sf::Color::White);
	line8.setStyle(sf::Text::Bold);
	line8.setPosition(100, 550);
	credits.draw(line8);

	sf::Text line9;
	line9.setFont(font);
	line9.setString("Enjoyed Playing ? ");
	line9.setCharacterSize(40);
	line9.setFillColor(sf::Color::White);
	line9.setStyle(sf::Text::Bold);
	line9.setPosition(450, 600);
	credits.draw(line9);

	sf::Text line10;
	line10.setFont(font);
	line10.setString("Please leave a review, it helps for better improvements.");
	line10.setCharacterSize(40);
	line10.setFillColor(sf::Color::White);
	line10.setStyle(sf::Text::Bold);
	line10.setPosition(100, 650);
	credits.draw(line10);

	sf::Text line11;
	line11.setFont(font);
	line11.setString("Thanks for playing !");
	line11.setCharacterSize(40);
	line11.setFillColor(sf::Color::Green);
	line11.setStyle(sf::Text::Bold);
	line11.setPosition(100, 700);
	credits.draw(line11);

	sf::Font font1;
	font1.loadFromFile("gamefont.ttf");
	sf::Text back;
	back.setFont(font1);
	back.setString("PRESS B TO BACK");
	back.setCharacterSize(10);
	back.setFillColor(sf::Color::White);
	back.setStyle(sf::Text::Bold);
	back.setPosition(1100, 720);
	credits.draw(back);

}
void help(sf::RenderWindow& pageHelp) {

	sf::Font font;
	font.loadFromFile("CaviarDreams.ttf");

	sf::Text title;
	title.setFont(font);
	title.setString("HELP");
	title.setFillColor(sf::Color::Yellow);
	title.setCharacterSize(100);
	title.setStyle(sf::Text::Bold);
	title.setPosition(500, 50);
	pageHelp.draw(title);

	sf::Text line1;
	line1.setFont(font);
	line1.setString("- Move pacman using keyboard arrows.");
	line1.setFillColor(sf::Color::White);
	line1.setCharacterSize(40);
	line1.setStyle(sf::Text::Bold);
	line1.setPosition(50, 200);
	pageHelp.draw(line1);

	sf::Text line2;
	line2.setFont(font);
	line2.setString("- Increase score by eating pac-dots and coins.");
	line2.setFillColor(sf::Color::White);
	line2.setCharacterSize(40);
	line2.setStyle(sf::Text::Bold);
	line2.setPosition(50, 250);
	pageHelp.draw(line2);

	sf::Text line3;
	line3.setFont(font);
	line3.setString("- By touching ghosts you lose one life.");
	line3.setFillColor(sf::Color::White);
	line3.setCharacterSize(40);
	line3.setStyle(sf::Text::Bold);
	line3.setPosition(50, 300);
	pageHelp.draw(line3);

	sf::Text line4;
	line4.setFont(font);
	line4.setString("- Game is over when all lives are lost.");
	line4.setFillColor(sf::Color::White);
	line4.setCharacterSize(40);
	line4.setStyle(sf::Text::Bold);
	line4.setPosition(50, 350);
	pageHelp.draw(line4);

	sf::Text line9;
	line9.setFont(font);
	line9.setString("- Only Thanos can decrease your score.");
	line9.setFillColor(sf::Color::White);
	line9.setCharacterSize(40);
	line9.setStyle(sf::Text::Bold);
	line9.setPosition(50, 400);
	pageHelp.draw(line9);

	sf::Text line5;
	line5.setFont(font);
	line5.setString("Two Players Mode :");
	line5.setFillColor(sf::Color::Green);
	line5.setCharacterSize(40);
	line5.setStyle(sf::Text::Bold);
	line5.setPosition(50, 450);
	pageHelp.draw(line5);

	sf::Text line6;
	line6.setFont(font);
	line6.setString("- Player 1 moves using arrows, player 2 moves using (W, A, S, D).");
	line6.setFillColor(sf::Color::White);
	line6.setCharacterSize(40);
	line6.setStyle(sf::Text::Bold);
	line6.setPosition(50, 500);
	pageHelp.draw(line6);

	sf::Text line7;
	line7.setFont(font);
	line7.setString("- Player with higher score can eat the one with lower score.");
	line7.setFillColor(sf::Color::White);
	line7.setCharacterSize(40);
	line7.setStyle(sf::Text::Bold);
	line7.setPosition(50, 550);
	pageHelp.draw(line7);

	sf::Text line8;
	line8.setFont(font);
	line8.setString("- Player with higher score wins the game.");
	line8.setFillColor(sf::Color::White);
	line8.setCharacterSize(40);
	line8.setStyle(sf::Text::Bold);
	line8.setPosition(50, 600);
	pageHelp.draw(line8);

	sf::Font fontt;
	fontt.loadFromFile("gamefont.ttf");

	sf::Text back;
	back.setFont(fontt);
	back.setString("PRESS B TO BACK");
	back.setFillColor(sf::Color::White);
	back.setCharacterSize(10);
	back.setStyle(sf::Text::Bold);
	back.setPosition(1100, 720);
	pageHelp.draw(back);
}
void message(string msg, sf::RenderWindow& page3)
{
	page3.clear(sf::Color::Black);
	sf::Font font;
	font.loadFromFile("gamefont.ttf");

	sf::Text nextLevel;
	nextLevel.setFont(font);
	nextLevel.setString(msg);
	nextLevel.setCharacterSize(40);
	nextLevel.setFillColor(sf::Color::White);
	nextLevel.setStyle(sf::Text::Bold);
	nextLevel.setPosition(40, 80);
	page3.draw(nextLevel);
}
void highscorePage(sf::RenderWindow& page4)
{
	page4.clear(sf::Color::Black);
	sf::Font font;
	font.loadFromFile("gamefont.ttf");

	sf::Text enterName;
	enterName.setFont(font);
	enterName.setString("ENTER YOUR NAME");
	enterName.setCharacterSize(40);
	enterName.setFillColor(sf::Color::White);
	enterName.setStyle(sf::Text::Bold);
	enterName.setPosition(85, 50);
	page4.draw(enterName);
}
bool isHighScore(int score5)
{
	if (score5 == 0)
		return false;
	if (topScores.size() < 5)
		return true;

	for (int i = 0; i < topScores.size(); i++)
	{
		if (topScores[i].score <= score5)
		{
			return true;
		}
	}
	return false;
} // function to tell if this score is a high score
void resetMazeArray(void)
{
	for (int i = 0; i < 31; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			if (maze1[i][j] == 'D')
				maze1[i][j] = '.';
			if (maze2[i][j] == 'D')
				maze2[i][j] = '.';
			if (maze3[i][j] == 'D')
				maze3[i][j] = '.';

			if (maze1[i][j] == 'O')
				maze1[i][j] = '0';
			if (maze2[i][j] == 'O')
				maze2[i][j] = '0';
			if (maze3[i][j] == 'O')
				maze3[i][j] = '0';
		}
	}
}
void resetGame(void)
{
	dots = 0, dots1 = 296, dots2 = 314, dots3 = 335;
	pacPos.x = 1;
	pacPos.y = 1;
	if (selectedmode == 0)
	{
		ghost1.x = 12;
		ghost1.y = 16;
		ghost2.x = 12;
		ghost2.y = 17;
		ghost3.x = 11;
		ghost3.y = 17;
		ghost4.x = 13;
		ghost4.y = 17;
		currMaze = &maze1;
		maxGameScore = score1;
		levels = 1;
		dots = dots1;
	}
	else if (selectedmode == 1)
	{
		ghost1.x = 10;
		ghost1.y = 15;
		ghost2.x = 10;
		ghost2.y = 16;
		ghost3.x = 10;
		ghost3.y = 17;
		ghost4.x = 11;
		ghost4.y = 16;
		levels = 3;
		currMaze = &maze3;
		dots = dots3;
	}
	lives = 3;
	G1 = 600, G2 = 800, G3 = 1000, G4 = 500;
	counter1 = 0, counter2 = 1, counter3 = 1;
	selectedmode = 0, page = 2, character = 0, choice = 0, firstChoice = 0;
	score = 0, score1 = 100, score2 = 120, score3 = 100;
	soundON = true, YouWin = true, YouLose = true, start = false;
	startGame = false, enterName = true, soundDuration = true, twoplayers = true;
	characterIndex = 0;
	pacrotaion = 0;
	pacScale = sf::Vector2f(1, 1);
	lastDirection = sf::Keyboard::Y;
	character2 = 0;
	scorePLAYER1 = scorePLAYER2 = 0, livesPL1 = 3, livesPL2 = 3;
	lastDirectionPL1 = sf::Keyboard::Y;
	lastDirectionPL2 = sf::Keyboard::Y;
	pac2Pos.x = 23;
	pac2Pos.y = 29;
}
int moveplayer1(int x, int y, int x2, int y2)
{
	int ret = 0;
	if ((*currMaze)[y2][x2] != '#') {
		if ((*currMaze)[y2][x2] == ' ') {
			ret = 1;
		}
		else if ((*currMaze)[y2][x2] == '.') {  //CHECK DOT..
			(*currMaze)[y2][x2] = 'D';
			scorePLAYER1 = scorePLAYER1 + 10;
			dots--;
			ret = 2;
		}
		else if ((*currMaze)[y2][x2] == '0') {   //CHECK COIN..
			(*currMaze)[y2][x2] = 'O';
			scorePLAYER1 = scorePLAYER1 + 100;
			dots--;
			ret = 3;
		}
		else if ((x2 == ghost1.x && y2 == ghost1.y) ||  //CHECK GHOST
			(x2 == ghost2.x && y2 == ghost2.y) ||
			(x2 == ghost3.x && y2 == ghost3.y) ||
			(x2 == ghost4.x && y2 == ghost4.y))
		{
			livesPL1--;
			loseLive();
			sf::sleep(sf::milliseconds(pace));
			ret = 4;
		}
		pacPos.x = x2;
		pacPos.y = y2;
	}
	return ret;
}
int moveplayer2(int x, int y, int x2, int y2)
{
	int ret = 0;
	if ((*currMaze)[y2][x2] != '#') {
		if ((*currMaze)[y2][x2] == ' ') {
			ret = 1;
		}
		else if ((*currMaze)[y2][x2] == '.') {  //CHECK DOT..
			(*currMaze)[y2][x2] = 'D';
			scorePLAYER2 = scorePLAYER2 + 10;
			dots--;
			ret = 2;
		}
		else if ((*currMaze)[y2][x2] == '0') {   //CHECK COIN..
			(*currMaze)[y2][x2] = 'O';
			scorePLAYER2 = scorePLAYER2 + 100;
			dots--;
			ret = 3;
		}
		else if ((x2 == ghost1.x && y2 == ghost1.y) ||  //CHECK GHOST
			(x2 == ghost2.x && y2 == ghost2.y) ||
			(x2 == ghost3.x && y2 == ghost3.y) ||
			(x2 == ghost4.x && y2 == ghost4.y))
		{
			livesPL2--;
			loseLive();
			sf::sleep(sf::milliseconds(pace));
			ret = 4;
		}
		pac2Pos.x = x2;
		pac2Pos.y = y2;
	}
	return ret;
}
void movePacman1(sf::Keyboard::Key keyCode)
{
	static sf::Keyboard::Key previous = sf::Keyboard::Right;
	int x2, y2;
	int direction = 0;
	if (livesPL1 == 0 || livesPL2 == 0)
		return;
	if (keyCode == sf::Keyboard::Down) {    //MOVE DOWN..

		direction = sf::Keyboard::Down;
		y2 = pacPos.y + 1;
		if (moveplayer1(pacPos.x, pacPos.y, pacPos.x, y2))
		{
			pacrotaion = 90;
			if (previous == sf::Keyboard::Right)
			{
				pacrotaion = 90;
				pacScale = sf::Vector2f(1, 1);
			}
			else if (previous == sf::Keyboard::Left)
			{
				pacrotaion = 90;
				pacScale = sf::Vector2f(1, -1);
			}
			else if (previous == sf::Keyboard::Up)
			{
				pacScale.y *= -1;
			}
			previous = sf::Keyboard::Down;
		}
	}
	else if (keyCode == sf::Keyboard::Up) {    //MOVE UP
		direction = sf::Keyboard::Up;
		y2 = pacPos.y - 1;
		if (moveplayer1(pacPos.x, pacPos.y, pacPos.x, y2))
		{
			pacrotaion = -90;
			if (previous == sf::Keyboard::Right)
			{
				pacrotaion = -90;
				pacScale = sf::Vector2f(1, 1);
			}
			else if (previous == sf::Keyboard::Left)
			{
				pacrotaion = -90;
				pacScale = sf::Vector2f(1, -1);
			}
			else if (previous == sf::Keyboard::Down)
			{
				pacScale.y *= -1;
			}
			previous = sf::Keyboard::Up;
		}
	}
	else if (keyCode == sf::Keyboard::Right) {     //MOVE RIGHT
		direction = sf::Keyboard::Right;
		x2 = pacPos.x + 1;
		if (moveplayer1(pacPos.x, pacPos.y, x2, pacPos.y))
		{
			pacrotaion = 0;
			{
				pacrotaion = 0;
				pacScale = sf::Vector2f(1, 1);
			}

			previous = sf::Keyboard::Right;
		}
	}
	else if (keyCode == sf::Keyboard::Left) {      //MOVE LEFT
		direction = sf::Keyboard::Left;
		x2 = pacPos.x - 1;
		if (moveplayer1(pacPos.x, pacPos.y, x2, pacPos.y))
		{
			pacrotaion = 0;
			{
				pacrotaion = 0;
				pacScale = sf::Vector2f(-1, 1);
			}

			previous = sf::Keyboard::Left;
		}
	}

}
void movePacman2(sf::Keyboard::Key keyCode)
{
	static sf::Keyboard::Key previous = sf::Keyboard::D;
	int x2, y2;
	int direction = 0;
	if (livesPL1 == 0 || livesPL2 == 0)
		return;
	if (keyCode == 's' || keyCode == 'S') {    //MOVE S..

		direction = sf::Keyboard::S;
		y2 = pac2Pos.y + 1;
		if (moveplayer2(pac2Pos.x, pac2Pos.y, pac2Pos.x, y2))
		{
			pac2rotation = 90;
			if (previous == sf::Keyboard::D)
			{
				pac2rotation = 90;
				pac2Scale = sf::Vector2f(1, 1);
			}
			else if (previous == sf::Keyboard::A)
			{
				pac2rotation = 90;
				pac2Scale = sf::Vector2f(1, -1);
			}
			else if (previous == sf::Keyboard::W)
			{
				pac2Scale.y *= -1;
			}
			previous = sf::Keyboard::S;
		}
	}
	else if (keyCode == 'w' || keyCode == 'W') {    //MOVE W
		direction = sf::Keyboard::W;
		y2 = pac2Pos.y - 1;
		if (moveplayer2(pac2Pos.x, pac2Pos.y, pac2Pos.x, y2))
		{
			pac2rotation = -90;
			if (previous == sf::Keyboard::D)
			{
				pac2rotation = -90;
				pac2Scale = sf::Vector2f(1, 1);
			}
			else if (previous == sf::Keyboard::A)
			{
				pac2rotation = -90;
				pac2Scale = sf::Vector2f(1, -1);
			}
			else if (previous == sf::Keyboard::S)
			{
				pac2Scale.y *= -1;
			}
			previous = sf::Keyboard::W;
		}
	}
	else if (keyCode == 'd' || keyCode == 'D') {     //MOVE D
		direction = sf::Keyboard::D;
		x2 = pac2Pos.x + 1;
		if (moveplayer2(pac2Pos.x, pac2Pos.y, x2, pac2Pos.y))
		{
			pac2rotation = 0;
			{
				pac2rotation = 0;
				pac2Scale = sf::Vector2f(1, 1);
			}

			previous = sf::Keyboard::D;
		}
	}
	else if (keyCode == 'a' || keyCode == 'A') {      //MOVE A
		direction = sf::Keyboard::A;
		x2 = pac2Pos.x - 1;
		if (moveplayer2(pac2Pos.x, pac2Pos.y, x2, pac2Pos.y))
		{
			pac2rotation = 0;
			{
				pac2rotation = 0;
				pac2Scale = sf::Vector2f(-1, 1);
			}

			previous = sf::Keyboard::A;
		}
	}

}
void ghostmovement7(Position* ghost)
{
	levels = 3;
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	if (levels == 3)
		G3 = 800;
	int  direction, up, down, right, left;
	if (levels == 3)
	{
		direction = 1, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y + 1, ghost->x) == false && direction == down && start) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y++;
			direction = right;
			break;
		}
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
			break;
		}
	}
	srand(time(0));
	while (startGame && livesPL1 > 0 && livesPL2 > 0)
	{
		if (counter3 % 4 != 0)
		{
			if (pacPos.y == ghost3.y)
			{
				if (pacPos.x < ghost3.x)
				{
					if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else
						direction = right;
				}
				else if (pacPos.x > ghost3.x)
				{
					if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else
						direction = left;
				}
			}
			if (pacPos.x == ghost1.x) {
				if (pacPos.y < ghost3.y)
				{
					if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else
						direction = down;
				}
				else if (pacPos.y > ghost3.y) {
					if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else
						direction = up;
				}
			}
			else if (pacPos.y < ghost3.y)
			{
				if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else
					direction = down;
			}
			else if (pacPos.y > ghost3.y)
			{
				if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else
					direction = up;
			}
			else if (pacPos.x < ghost3.x)
			{
				if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pacPos.x > ghost3.x)
			{
				if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else
					direction = left;
			}
			counter3++;
		}
		else
		{
			direction = rand() % 4;
			if (direction == up && checkwall(ghost3.y - 1, ghost3.x) == false)
				counter3++;
			else if (direction == down && checkwall(ghost3.y + 1, ghost3.x) == false)
				counter3++;
			else if (direction == left && checkwall(ghost3.y, ghost3.x - 1) == false)
				counter3++;
			else if (direction == right && checkwall(ghost3.y, ghost3.x + 1) == false)
				counter3++;
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				livesPL1--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				livesPL1--;
				musiclose.play();
				loseLive();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				livesPL1--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G3));
			ghost->x--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				livesPL1--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void ghostmovement8(Position* ghost)
{
	levels = 3;
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	if (levels == 3)
		G4 = 700;
	int  direction, up, down, right, left;
	if (levels == 3)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
			direction = right;
			break;
		}
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x++;
			break;
		}
	}
	srand(time(0));
	while (startGame && livesPL2 > 0 && livesPL1 > 0 && levels <= 3)
	{
		if (pacPos.y == ghost4.y)
		{
			if (pacPos.x < ghost4.x)
			{
				if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pacPos.x > ghost4.x)
			{
				if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else
					direction = left;
			}
		}
		if (pacPos.x == ghost4.x) {
			if (pacPos.y < ghost4.y)
			{
				if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else
					direction = down;
			}
			else if (pacPos.y > ghost4.y) {
				if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else
					direction = up;
			}
		}
		else if (pacPos.y < ghost4.y)
		{
			if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else
				direction = down;
		}
		else if (pacPos.y > ghost4.y)
		{
			if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else
				direction = up;
		}
		else if (pacPos.x < ghost4.x)
		{
			if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else
				direction = right;
		}
		else if (pacPos.x > ghost4.x)
		{
			if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else
				direction = left;
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (scorePLAYER1 >= 100)
				{
					scorePLAYER1 -= 100;
					loseLive();
					break;
				}
				else
				{
					livesPL1--;
					loseLive();
					musiclose.play();
				}
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (scorePLAYER1 >= 100)
				{
					scorePLAYER1 -= 100;
					loseLive();
					break;
				}
				else
				{
					livesPL1--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			//if (levels == 1)
			//{
			//	if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
			//	{
			//	if (counter1 % 2 == 0)
			//	direction = right;
			//	else
			//	direction = left;
			//	break;
			//	}
			//}
			//if (levels == 2)
			//{
			//	if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
			//	{
			//	if (counter1 % 2 == 0)
			//	direction = right;
			//	else
			//	direction = left;
			//	break;
			//	}
			//}
			sf::sleep(sf::milliseconds(G4));
			ghost->y++;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (scorePLAYER1 >= 100)
				{
					scorePLAYER1 -= 100;
					loseLive();
					break;
				}
				else
				{
					livesPL1--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G4));
			ghost->x--;
			if (ghost->x == pacPos.x && ghost->y == pacPos.y)	//If it meets pacman...
			{
				if (scorePLAYER1 >= 100)
				{
					scorePLAYER1 -= 100;
					loseLive();
					musiclose.play();
					break;
				}
				else
				{
					livesPL1--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void ghostmovement5(Position* ghost)
{
	levels = 3;
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	if (levels == 3)
		G3 = 600;
	int  direction, up, down, right, left;

	if (levels == 3)
	{
		direction = 1, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y + 1, ghost->x) == false && direction == down && start) //Getting out of the middle box...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y++;
			direction = right;
		}
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
		}
	}
	srand(time(0));
	while (startGame && livesPL1 > 0 && livesPL2 > 0)
	{
		if (counter3 % 4 != 0)
		{
			if (pac2Pos.y == ghost3.y)
			{
				if (pac2Pos.x < ghost3.x)
				{
					if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else
						direction = right;
				}
				else if (pac2Pos.x > ghost3.x)
				{
					if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else
						direction = left;
				}
			}
			if (pac2Pos.x == ghost1.x) {
				if (pac2Pos.y < ghost3.y)
				{
					if (checkwall(ghost3.y - 1, ghost3.x) == false)
						direction = up;
					else if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else
						direction = down;
				}
				else if (pac2Pos.y > ghost3.y) {
					if (checkwall(ghost3.y + 1, ghost3.x) == false)
						direction = down;
					else if (checkwall(ghost3.y, ghost3.x + 1) == false)
						direction = right;
					else if (checkwall(ghost3.y, ghost3.x - 1) == false)
						direction = left;
					else
						direction = up;
				}
			}
			else if (pac2Pos.y < ghost3.y)
			{
				if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else
					direction = down;
			}
			else if (pac2Pos.y > ghost3.y)
			{
				if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else
					direction = up;
			}
			else if (pac2Pos.x < ghost3.x)
			{
				if (checkwall(ghost3.y, ghost3.x - 1) == false)
					direction = left;
				else if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pac2Pos.x > ghost3.x)
			{
				if (checkwall(ghost3.y, ghost3.x + 1) == false)
					direction = right;
				else if (checkwall(ghost3.y - 1, ghost3.x) == false)
					direction = up;
				else if (checkwall(ghost3.y + 1, ghost3.x) == false)
					direction = down;
				else
					direction = left;
			}
			counter3++;
		}
		else
		{
			direction = rand() % 4;
			if (direction == up && checkwall(ghost3.y - 1, ghost3.x) == false)
				counter3++;
			else if (direction == down && checkwall(ghost3.y + 1, ghost3.x) == false)
				counter3++;
			else if (direction == left && checkwall(ghost3.y, ghost3.x - 1) == false)
				counter3++;
			else if (direction == right && checkwall(ghost3.y, ghost3.x + 1) == false)
				counter3++;
		}
		while (livesPL1 > 0 && livesPL2 > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->y--;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				livesPL2--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL1 > 0 && livesPL2 > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G3));
			ghost->x++;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				livesPL2--;
				musiclose.play();
				loseLive();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL1 > 0 && livesPL2 > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			//if (levels == 1)
			//{
			//	if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
			//	{
			//	if (counter1 % 2 == 0)
			//	direction = right;
			//	else
			//	direction = left;
			//	break;
			//	}
			//}
			//if (levels == 2)
			//{
			//	if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
			//	{
			//	if (counter1 % 2 == 0)
			//	direction = right;
			//	else
			//	direction = left;
			//	break;
			//	}
			//}
			sf::sleep(sf::milliseconds(G3));
			ghost->y++;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				livesPL2--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL1 > 0 && livesPL2 > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G3));
			ghost->x--;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				livesPL2--;
				loseLive();
				musiclose.play();
				break;
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}
void ghostmovement6(Position* ghost)
{
	levels = 3;
	sf::Music musiclose;
	musiclose.openFromFile("loselife.wav");
	sf::sleep(sf::milliseconds(3000));
	/*if (levels == 2)
	G4 = 260;*/
	if (levels == 3)
		G4 = 500;
	int  direction, up, down, right, left;
	if (levels == 3)
	{
		direction = 0, up = 0, down = 1, right = 2, left = 3;
		while (checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
			direction = right;
		}
		while (checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x++;
		}
	}
	srand(time(0));
	while (startGame && livesPL1 > 0 && livesPL2 > 0)
	{
		if (pac2Pos.y == ghost4.y)
		{
			if (pac2Pos.x < ghost4.x)
			{
				if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else
					direction = right;
			}
			else if (pac2Pos.x > ghost4.x)
			{
				if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else
					direction = left;
			}
		}
		if (pac2Pos.x == ghost4.x) {
			if (pac2Pos.y < ghost4.y)
			{
				if (checkwall(ghost4.y - 1, ghost4.x) == false)
					direction = up;
				else if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else
					direction = down;
			}
			else if (pac2Pos.y > ghost4.y) {
				if (checkwall(ghost4.y + 1, ghost4.x) == false)
					direction = down;
				else if (checkwall(ghost4.y, ghost4.x + 1) == false)
					direction = right;
				else if (checkwall(ghost4.y, ghost4.x - 1) == false)
					direction = left;
				else
					direction = up;
			}
		}
		else if (pac2Pos.y < ghost4.y)
		{
			if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else
				direction = down;
		}
		else if (pac2Pos.y > ghost4.y)
		{
			if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else
				direction = up;
		}
		else if (pac2Pos.x < ghost4.x)
		{
			if (checkwall(ghost4.y, ghost4.x - 1) == false)
				direction = left;
			else if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else
				direction = right;
		}
		else if (pac2Pos.x > ghost4.x)
		{
			if (checkwall(ghost4.y, ghost4.x + 1) == false)
				direction = right;
			else if (checkwall(ghost4.y - 1, ghost4.x) == false)
				direction = up;
			else if (checkwall(ghost4.y + 1, ghost4.x) == false)
				direction = down;
			else
				direction = left;
		}
		while (livesPL1 > 0 && livesPL2 > 0 && checkwall(ghost->y - 1, ghost->x) == false && direction == up && start)	//Moving up...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->y--;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				if (scorePLAYER2 >= 100)
				{
					scorePLAYER2 -= 100;
					loseLive();
					break;
				}
				else
				{
					livesPL2--;
					loseLive();
					musiclose.play();
				}
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y, ghost->x + 1) == false && direction == right && start)	//Moving right...
		{
			sf::sleep(sf::milliseconds(G4));
			ghost->x++;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				if (scorePLAYER2 >= 100)
				{
					scorePLAYER2 -= 100;
					loseLive();
					break;
				}
				else
				{
					livesPL2--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y + 1, ghost->x) == false && direction == down && start)	//Moving down...
		{
			//if (levels == 1)
			//{
			//	if (ghost->y == 15 && ghost->x == 12)  //Prevent from entering the middle box...
			//	{
			//	if (counter1 % 2 == 0)
			//	direction = right;
			//	else
			//	direction = left;
			//	break;
			//	}
			//}
			//if (levels == 2)
			//{
			//	if (ghost->y == 13 && ghost->x == 12)  //Prevent from entering the middle box...
			//	{
			//	if (counter1 % 2 == 0)
			//	direction = right;
			//	else
			//	direction = left;
			//	break;
			//	}
			//}
			sf::sleep(sf::milliseconds(G4));
			ghost->y++;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				if (scorePLAYER2 >= 100)
				{
					scorePLAYER2 -= 100;
					loseLive();
					break;
				}
				else
				{
					livesPL2--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y][ghost->x - 1] != '#' || (*currMaze)[ghost->y][ghost->x + 1] != '#')	//Change direction...
			{
				break;
			}
		}
		while (livesPL2 > 0 && livesPL1 > 0 && checkwall(ghost->y, ghost->x - 1) == false && direction == left && start)	//Moving left...
		{
			if (levels == 3) // prevent from entering the middle box...
			{
				if (ghost->y == 16 && ghost->x == 12)
				{
					for (int h = 0; h < 3; ++h)
					{
						sf::sleep(sf::milliseconds(G1));
						ghost->y++;
					}
					break;
				}
			}
			sf::sleep(sf::milliseconds(G4));
			ghost->x--;
			if (ghost->x == pac2Pos.x && ghost->y == pac2Pos.y)	//If it meets pacman...
			{
				if (scorePLAYER2 >= 100)
				{
					scorePLAYER2 -= 100;
					loseLive();
					musiclose.play();
					break;
				}
				else
				{
					livesPL2--;
					loseLive();
					musiclose.play();
					break;
				}
			}
			if ((*currMaze)[ghost->y - 1][ghost->x] != '#' || (*currMaze)[ghost->y + 1][ghost->x] != '#')	//Change direction...
			{
				break;
			}
		}
	}
}