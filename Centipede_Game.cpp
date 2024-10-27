#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
using namespace std;
using namespace sf;

// DIMENSIONS
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;	  // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// GAMEGRID
int gameGrid[gameRows][gameColumns] = {};

// VARIABLES
const int x = 0;
const int y = 1;
const int exists = 2;
int mushroom_num = 40;					// randomly generates mushrooms between 20 to 30
int res_lim = resolutionY - boxPixelsY; // resolution limit
int mushroomState = 2;					// states of mushroom
bool playerCollided = false;			// if player collides this becomes true
int SCORE = 0;							// for the total score
int maxCentipedes = 7;
int initial_seg_cnt = 14;
int spidercreationnum = 0;
int Spdr_sound_playcount = 0;
int ResLimitY = resolutionY - boxPixelsY * 10, ResLimitX = resolutionX - boxPixelsX;
bool spider_res_limX = false; // used in the drawspider function
bool spider_res_limY = false; // used in the drawspider function
int xtr_mushroom_num = 0;
int centipedecount = 1;
int call_counter = 0;

// FUNCTION DECLARATIONS
void drawPlayer(RenderWindow &window, float player[], Sprite &playerSprite);
void movePlayer(RenderWindow &window, float player[], Sprite &playerSprite);
void moveBullet(float bullet[], Clock &bulletClock);
void drawBullet(RenderWindow &window, float bullet[], Sprite &bulletSprite);
void drawMushrooms(RenderWindow &window, float mushroom[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite);
void CreateMushrooms(float mushroom[][3], float damg_mushroom[][3], float pois_mushrooms[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite);
void Checkbullet_mush_Collision(float mushroom[][3], float damg_mushroom[][3], float bullet[], Sprite &mushroomSprite, Sprite &damg_mushroomSprite, Sprite &bulletSprite);
void CreateCentipede(RenderWindow &window, float ***centipede, Sprite &CentipedeBody, Sprite &CentipedeHead);
void drawCentipede(RenderWindow &window, float ***centipede, float mushroom[][3], float bullet[], Sprite &CentipedeBody, Sprite &CentipedeHead, int centipedelength[]);
void MoveCentipede(RenderWindow &window, float ***centipede, float mushroom[][3], float bullet[], Sprite &CentipedeBody, Sprite &CentipedeHead, char centipedeDirections[], int centipedelength[]);
void DetectBullet_Cent_Collision(RenderWindow &window, float ***centipede, float bullet[], float pois_mushrooms[][3], Sprite &CentipedeBody, Sprite &mushroomSprite, Sprite &pois_mushroomSprite, int centipedelength[], float xtr_mushrooms[][3], char centipedeDirections[], Sprite &CentipedeHead);
void CheckPlayercollisions(RenderWindow &window, float player[], Sprite &playerSprite, float ***centipede, float pois_mushrooms[][3], float spider[], Sprite &deadplayerSprite, Sprite &CentipedeBody, Sound sound2);
void DrawPoisonousMushroom(RenderWindow &window, float ***centipede, float bullet[], float pois_mushrooms[][3], Sprite &CentipedeBody, Sprite &pois_mushroomSprite);
void RegenerateCentipede(RenderWindow &window, float ***&centipede, Sprite &CentipedeBody, Sprite &CentipedeHead, int centipedelength[], char centipedeDirections[], float mushroom[][3], float damg_mushroom[][3], float pois_mushrooms[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite);
void RegenerateMushrooms(float mushroom[][3], float damg_mushroom[][3], float pois_mushrooms[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite);
void DeleteCentipede(RenderWindow &window, float ***centipede, Sprite &CentipedeBody, Sprite &CentipedeHead, int centipedelength[], char centipedeDirections[]);
void CreateSpider(RenderWindow &window, float player[], Sprite &playerSprite, Sprite &SpiderSprite, float spider[], Sound Spidersound);
void DrawSpider(RenderWindow &window, float player[], Sprite &playerSprite, Sprite &SpiderSprite, float spider[], float mushroom[][3], Sound Spidersound);
void bulletspidCollision(RenderWindow &window, float bullet[], Sprite &SpiderSprite, float spider[]);

// MAIN FUNCTION
int main() {
	int cols = 2;
	Clock clock;
	SoundBuffer buffer;
	buffer.loadFromFile("Sounds/fire1.wav");

	Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(30);

	Sound sound2; // when player is destroyed this sound plays
	SoundBuffer buffer2;
	buffer2.loadFromFile("Sounds/death.wav");

	sound2.setBuffer(buffer2);
	sound2.setVolume(30);

	Sound Spidersound;
	SoundBuffer spidersoundbuffer;
	spidersoundbuffer.loadFromFile("Sounds/spider.wav");
	Spidersound.setBuffer(spidersoundbuffer);

	Spidersound.setVolume(20);

	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Centipede", Style::Close | Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(100, 0));

	// Initializing Background Music.
	Music bgMusic;
	bgMusic.openFromFile("Sounds/gigachad.ogg");
	bgMusic.play();
	bgMusic.setVolume(30);

	// Initializing Background.
	Texture backgroundTexture;
	Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Background/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(Color(25, 255, 255, 255 * 0.2)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = resolutionY - boxPixelsY;
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Sprites/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY)); // size of sprite

	// for the dead player sprite
	Sprite deadplayerSprite;
	Texture deadplayerTexture;
	deadplayerTexture.loadFromFile("Sprites/explosion.png");
	deadplayerSprite.setTexture(deadplayerTexture);
	deadplayerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {}; // new bullet comes after first bullet leaves screen, size 3 as x, y, and exists
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = false; // make it false until key x is pressed
	// when bullet hits, bullet[exists] becomes alse

	// initializing mushroom 2D arrays of mushrooms and damaged mushrooms, and textures
	float mushroom[mushroom_num][3] = {};
	float damg_mushroom[mushroom_num][3] = {};
	Sprite mushroomSprite;
	Texture mushroomTexture;
	mushroomTexture.loadFromFile("Sprites/mushroom.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY)); // size of sprite

	float xtr_mushrooms[12][3] = {};

	// damaged mushroom sprite
	Sprite damg_mushroomSprite;
	Texture damg_mushroomTexture;
	damg_mushroomTexture.loadFromFile("Sprites/mushroom.png");
	damg_mushroomSprite.setTexture(damg_mushroomTexture);
	damg_mushroomSprite.setTextureRect(IntRect(boxPixelsX * 2, 0, boxPixelsX, boxPixelsY));

	// poisonous mushroom Sprite
	float pois_mushrooms[12][3];
	Sprite pois_mushroomSprite;
	Texture pois_mushroomTexture;
	pois_mushroomTexture.loadFromFile("Sprites/mushroom.png");
	pois_mushroomSprite.setTexture(pois_mushroomTexture);
	pois_mushroomSprite.setTextureRect(IntRect(0, boxPixelsY, boxPixelsX, boxPixelsY));

	// initializing centipede body and 2d array of centipede
	float ***centipede = new float **[maxCentipedes] {};
	for (int i = 0; i < maxCentipedes; i++)
	{
		centipede[i] = new float *[initial_seg_cnt];
		for (int j = 0; j < initial_seg_cnt; j++)
		{
			centipede[i][j] = new float[3];
		}
	}

	/*CENTIPEDE BODY*/
	Sprite CentipedeBody;
	Texture CentipedeBodyTexture;
	CentipedeBodyTexture.loadFromFile("Sprites/c_body_left_walk.png");
	CentipedeBody.setTexture(CentipedeBodyTexture);
	CentipedeBody.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	// EXTRA ARRAYS TO CHECK DIRECTION AND LENGTH OF EACH CENTIPEDE
	char centipedeDirections[6] = {'L', 'L', 'L', 'L', 'L', 'L'}; // directions of each centipede
	int centipedelength[7] = {initial_seg_cnt, 0, 0, 0, 0, 0, 0}; // to store the length of each centipede
	// centipede head texture
	Sprite CentipedeHead;
	Texture CentipedeHeadTexture;
	CentipedeHeadTexture.loadFromFile("Sprites/c_head_left_walk.png");
	CentipedeHead.setTexture(CentipedeHeadTexture);
	CentipedeHead.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	Clock bulletClock;
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Sprites/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	// for the spider
	float spider[3] = {};
	Sprite SpiderSprite;
	Texture SpiderTexture;
	SpiderSprite.setTexture(SpiderTexture);
	SpiderTexture.loadFromFile("Sprites/spider_and_score.png");
	SpiderSprite.setTextureRect(IntRect(240, 0, 60, 60));

	Font font;
	font.loadFromFile("Font/OpenSans-ExtraBoldItalic.ttf");
	Text scoreText;

	scoreText.setFont(font);
	scoreText.setString("Score: " + std::to_string(SCORE));
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(boxPixelsX, res_lim - boxPixelsY);

	// CREATE MUSHROOMS AND CENTIPEDE INITIALLY
	CreateMushrooms(mushroom, damg_mushroom, pois_mushrooms, mushroomSprite, damg_mushroomSprite);
	CreateCentipede(window, centipede, CentipedeBody, CentipedeHead);
	while (window.isOpen())
	{
		call_counter++;
		window.draw(backgroundSprite); // background
		drawMushrooms(window, mushroom, mushroomSprite, damg_mushroomSprite);
		drawPlayer(window, player, playerSprite);

		// for the score
		scoreText.setString("Score: " + std::to_string(SCORE));

		// increments a variable spidercreationnum before creating another spider

		if (!spider[exists])
		{
			spidercreationnum++;
		}
		int random = rand() % 1000 + rand() % 1000 + 750;
		if (spidercreationnum % random == 0 && !spider[exists]) // create spider after spidercreationnum % 1000 = 0
		{
			CreateSpider(window, player, playerSprite, SpiderSprite, spider, Spidersound);
		}

		// draw spider and move it
		DrawSpider(window, player, playerSprite, SpiderSprite, spider, mushroom, Spidersound);
		window.draw(scoreText);

		Spdr_sound_playcount++;
		if (Spdr_sound_playcount % 100 == 0 && spider[exists])
		{
			Spidersound.play();
		}
		int count = 0;
		for (int k = 0; k < centipedecount; k++)
		{
			if (!centipede[k][0][exists])
			{
				count++;
			}
		}

		if (count == centipedecount || centipede[0][0][y] > resolutionY - boxPixelsY * 3)
		{
			RegenerateCentipede(window, centipede, CentipedeBody, CentipedeHead, centipedelength, centipedeDirections, mushroom, damg_mushroom, pois_mushrooms, mushroomSprite, damg_mushroomSprite);
		}

		if (call_counter % 15 == 0)
		{
			MoveCentipede(window, centipede, mushroom, bullet, CentipedeBody, CentipedeHead, centipedeDirections, centipedelength);
		}
		DetectBullet_Cent_Collision(window, centipede, bullet, pois_mushrooms, CentipedeBody, mushroomSprite, pois_mushroomSprite, centipedelength, xtr_mushrooms, centipedeDirections, CentipedeHead);
		drawCentipede(window, centipede, mushroom, bullet, CentipedeBody, CentipedeHead, centipedelength);

		if (bullet[exists] == true) // detecting movement and drawing of bullet, and all collisions except centipede
		{

			moveBullet(bullet, bulletClock);
			drawBullet(window, bullet, bulletSprite);
			Checkbullet_mush_Collision(mushroom, damg_mushroom, bullet, mushroomSprite, damg_mushroomSprite, bulletSprite);

			bulletspidCollision(window, bullet, SpiderSprite, spider);
		}

		/*draw poisonous mushrooms*/

		if (!playerCollided)
		{
			movePlayer(window, player, playerSprite);
		}
		/*check
		player collisions with centipede*/
		CheckPlayercollisions(window, player, playerSprite, centipede, pois_mushrooms, spider, deadplayerSprite, CentipedeBody, sound2);
		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			if (bullet[exists] == false)
			{

				bullet[exists] = true;
				sound.play();
				bullet[x] = player[x];
				bullet[y] = player[y] - 10;
			}
		}

		Event e;
		while (window.pollEvent(e))
		{

			if (e.type == Event::Closed)
			{
				for (int i = 0; i < maxCentipedes; i++)
				{
					for (int j = 0; j < initial_seg_cnt; j++)
					{
						delete[] centipede[i][j];
					}
					delete[] centipede[i];
				}
				delete[] centipede;
				return 0;
			}
		}
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //

void drawPlayer(RenderWindow &window, float player[], Sprite &playerSprite)
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void movePlayer(RenderWindow &window, float player[], Sprite &playerSprite)
{
	const float deltaTime = 1.0f / 60.0f;
	float delta_T = deltaTime;
	float playerspeed = 120.0f;
	float PlayerSpeed = playerspeed * delta_T;

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		if (player[x] < (resolutionX - boxPixelsX))
		{

			player[x] += PlayerSpeed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{

			player[y] += PlayerSpeed / 1.414; // so that if both are pressed speed remains the same throughout and doesnt increase. 1.41 is sqrt(2)
		}

		// if(abs(player[x] - mushroom[]))
	}
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		if (player[x] > 0)
		{

			player[x] -= PlayerSpeed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{

			player[y] += PlayerSpeed / 1.414;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		if (player[y] > resolutionY - (boxPixelsY * 5))
		{

			player[y] -= PlayerSpeed;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
	{

		if (player[y] < (resolutionY - boxPixelsX))
		{

			player[y] += PlayerSpeed;
		}
	}
}

void moveBullet(float bullet[], Clock &bulletClock)
{
	if (bulletClock.getElapsedTime().asMilliseconds() < 4) // y -=10 every 20 milliseconds, do this for centipede as well
		return;

	bulletClock.restart();
	bullet[y] -= 10;			// y is decreasing as the bullet goes up so -10 gap given to move it up
	if (bullet[y] < -32)		// -32 instead of -30 for proper animation
		bullet[exists] = false; // when the bullet reaches the top of screen
}
void drawBullet(RenderWindow &window, float bullet[], Sprite &bulletSprite)
{
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}
void CreateMushrooms(float mushroom[][3], float damg_mushroom[][3], float pois_mushrooms[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite)
{

	for (int i = 0; i < mushroom_num; i++)
	{
		int mposX = rand() % (resolutionX - boxPixelsX);
		int mposY = rand() % (resolutionY - boxPixelsY * 2) - (boxPixelsY * 2);

		mushroom[i][x] = mposX;
		mushroom[i][y] = mposY;
	}

	for (int i = 0; i < mushroom_num; i++)
	{
		mushroom[i][mushroomState] = 1; // initializing the undamaged mushroom state which is 1
	}
}
void drawMushrooms(RenderWindow &window, float mushroom[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite)
{

	for (int i = 0; i < mushroom_num; i++)
	{
		if (mushroom[i][mushroomState] == 1) // if undamaged
		{
			mushroomSprite.setPosition(mushroom[i][x], mushroom[i][y]);
			window.draw(mushroomSprite);
		}
		else if (mushroom[i][mushroomState] == 2) // if damaged state
		{
			damg_mushroomSprite.setPosition(mushroom[i][x], mushroom[i][y]);
			window.draw(damg_mushroomSprite);
		}
		else if (mushroom[i][mushroomState] == 3) // if destroyed
		{

			mushroom[i][x] = -1;
			mushroom[i][y] = -1; // out of resolution
		}
	}
}

void Checkbullet_mush_Collision(float mushroom[][3], float damg_mushroom[][3], float bullet[], Sprite &mushroomSprite, Sprite &damg_mushroomSprite, Sprite &bulletSprite)
{

	for (int i = 0; i < mushroom_num; i++)
	{

		if (bullet[y] + 10 < mushroom[i][y] && abs(bullet[x] - mushroom[i][x]) < 20)
		{
			bullet[exists] = false;

			if (mushroom[i][mushroomState] == 1)
			{
				mushroom[i][mushroomState] = 2;
			}
			else if (mushroom[i][mushroomState] == 2)
			{

				mushroom[i][mushroomState] = 3;
				SCORE++;
			}
		}
	}
}

void CreateCentipede(RenderWindow &window, float ***centipede, Sprite &CentipedeBody, Sprite &CentipedeHead)
{
	int j = rand() % 10 + 1;
	centipede[0][0][x] = resolutionX - boxPixelsX;
	centipede[0][0][y] = boxPixelsY;
	centipede[0][0][exists] = true;
	CentipedeHead.setPosition(centipede[0][0][x], centipede[0][0][y]);
	for (int i = 1; i < initial_seg_cnt; i++)
	{
		centipede[0][i][x] = resolutionX - boxPixelsX;
		centipede[0][i][y] = boxPixelsY * j; // make it spawn randomly
		if (centipede[0][0][exists])
		{
			centipede[0][i][exists] = true; // edit
		}

		CentipedeBody.setPosition(centipede[0][i][x], centipede[0][i][y]);
	}
}
void drawCentipede(RenderWindow &window, float ***centipede, float mushroom[][3], float bullet[], Sprite &CentipedeBody, Sprite &CentipedeHead, int centipedelength[])
{
	for (int k = 0; k < centipedecount; k++)
	{
		if (centipede[k][0][exists])
		{
			CentipedeHead.setPosition(centipede[k][0][x], centipede[k][0][y]);
			window.draw(CentipedeHead); // if it exists draw head of centipede
		}
		if (centipede[k][0][exists])
		{
			for (int i = 1; i < centipedelength[k]; i++)
			{

				CentipedeBody.setPosition(centipede[k][i][x], centipede[k][i][y]);
				window.draw(CentipedeBody); // draw rest of the segments if they exist
			}
		}
	}
}
void MoveCentipede(RenderWindow &window, float ***centipede, float mushroom[][3], float bullet[], Sprite &CentipedeBody, Sprite &CentipedeHead, char centipedeDirections[], int centipedelength[])
{
	const float deltaTime = 1.0f / 100.0f;					  // delta time
	bool iscollisiondetected = false;						  // if this becomes true centipede moves down and changes direction
	bool resolution_limit_reached;							  // to check if it reaches the bottom of the screen
	float centipedespeed = 2600.0f;							  // speed
	float CentipedeSpeed = centipedespeed * deltaTime;		  // overall speed
	float head_posX, head_posY;								  // we only detect the x and y positions of the head, the rest of segments follow through
	int left_wall = 0, right_wall = resolutionX - boxPixelsX; // collision points (end of resolution)

	// loop for each centipede that is made from fragmenting (intially one)
	for (int k = 0; k < centipedecount; k++)
	{
		head_posX = centipede[k][0][x]; // storing x position of centipede
		head_posY = centipede[k][0][y]; // storing y position of centipede

		// loop for checking centipede collision with mushroom
		for (int i = 0; i < 30; i++)
		{
			if (abs(head_posX - mushroom[i][x]) < boxPixelsX && abs(head_posY - mushroom[i][y]) < 20) // if coordinates match
			{

				iscollisiondetected = true; // collision detected
				mushroom[i][mushroomState] = 3;
			}
		}
		// collisions
		if (head_posX <= left_wall) // if centipede reaches left side of the window
		{
			if (head_posY >= res_lim) // checks if the centipede has reached the bottom of the window
			{
				res_lim = resolutionY - boxPixelsY * 5;
				resolution_limit_reached = true;
			}
			iscollisiondetected = true; // collision detected
		}
		if (head_posX >= right_wall && centipedeDirections[k] == 'R') // if it hits right side of the window
		{
			if (head_posY >= res_lim) // checks if the centipede has reached the bottom of the window
			{
				res_lim = resolutionY - boxPixelsY * 6; // resolution limit becomes point where player area ends (from the top)
				resolution_limit_reached = true;
			}
			iscollisiondetected = true; // collision detected
		}
		if (iscollisiondetected) // if collision is detected
		{

			centipede[k][0][y] += boxPixelsY; // centipede moves down

			if (centipedeDirections[k] == 'L') // if it is moving left
			{
				centipedeDirections[k] = 'R'; // make its direction right
			}
			else if (centipedeDirections[k] == 'R') // if it is moving right
			{
				centipedeDirections[k] = 'L'; // make its direction left
			}

			iscollisiondetected = false; // make it false to avoid repeating the movement
		}

		if (resolution_limit_reached) // resolution limit reached
		{
			centipede[k][0][y] -= boxPixelsY * 2; // moves centipede up and makes it hover through the player area
		}

		if (head_posY <= res_lim) // if it reaches the upper limit of player area
		{
			res_lim = resolutionY - boxPixelsY; // resolution limit becomes the bottom of the window again
			resolution_limit_reached = false;
		}

		if (centipedeDirections[k] == 'L')
		{
			centipede[k][0][x] -= CentipedeSpeed; // movement of centipede when it moves left
		}
		else if (centipedeDirections[k] == 'R')
		{
			centipede[k][0][x] += CentipedeSpeed; // movement of centipede when it moves right
		}

		// set the position of the head after all these are checked
		centipede[k][1][x] = head_posX; // previously stored position of centipede is stored in the next segment (x coordinate)
		centipede[k][1][y] = head_posY; // y coordinate

		for (int i = centipedelength[k] - 2; i > 0; i--) // do this for each segment after it so that the rest of the segments move along and follow the head
		{

			centipede[k][i + 1][x] = centipede[k][i][x]; // copy x position
			centipede[k][i + 1][y] = centipede[k][i][y]; // copy y position
			// CentipedeBody.setPosition(centipede[k][i][x], centipede[k][i][y]); // finally set the positions of the segments
		}
	}
}

void DetectBullet_Cent_Collision(RenderWindow &window, float ***centipede, float bullet[], float pois_mushrooms[][3], Sprite &CentipedeBody, Sprite &mushroomSprite, Sprite &pois_mushroomSprite, int centipedelength[], float xtr_mushrooms[][3], char centipedeDirections[], Sprite &CentipedeHead)
{
	float seg_posX, seg_posY;
	int hits = 0;
	int temp = 0;
	for (int k = 0; k < centipedecount; k++)
	{
		for (int i = 1; i < centipedelength[k] - 1; i++)
		{
			if (centipede[k][i][exists] && abs(bullet[x] - centipede[k][i][x]) < boxPixelsX / 2 && abs(bullet[y] - centipede[k][i][y]) < 10)
			{
				bullet[exists] = false;
				hits++;
				centipede[k][i][exists] = false;
				centipedelength[k] -= i;

				centipedecount++;
				centipedelength[k + 1] = i;
				centipede[k + 1][0][exists] = true;
				centipede[k + 1][0][x] = centipede[k][0][x];
				centipede[k + 1][0][y] = centipede[k][0][y];

				for (int l = 1; l < centipedelength[k] - 1; l++)
				{
					centipede[k + 1][l][x] = centipede[k][l][x];
					centipede[k + 1][l][y] = centipede[k][l][y];
				}

				if (centipedeDirections[k] == 'L')
				{
					centipedeDirections[k + 1] = 'R';
				}
				else
				{
					centipedeDirections[k + 1] = 'L';
				}
				if (centipede[k][0][y] > resolutionY - boxPixelsY * 5)
				{
					seg_posX = centipede[k][i][x];
					seg_posY = centipede[k][i][y];
					pois_mushrooms[i - 1][x] = seg_posX;
					pois_mushrooms[i - 1][y] = seg_posY;
				}

				for (int k = 0; k < centipedecount; k++)
				{
					if (centipedelength[k] < 2)
					{
						temp++;
					}
				}
				if (temp == centipedecount)
				{
					for (int k = 0; k < centipedecount; k++)
					{
						for (int i = 0; i < centipedelength[k]; i++)
						{
							centipede[k][i][exists] = false;
						}
					}
				}

				break;

				// Fragment the centipede
			}
		}
		if (abs(bullet[x] - centipede[k][0][x]) < boxPixelsX / 2 && abs(bullet[y] - centipede[k][0][y]) < 10)
		{
			centipede[k][0][exists] = false;
			if (!centipede[k][0][exists])
			{
				for (int i = 0; i < centipedelength[k]; i++)
				{
					centipede[k][i][exists] = false;
				}
			}

			bullet[exists] = false;
			break;
		}
	}
}

void RegenerateCentipede(RenderWindow &window, float ***&centipede, Sprite &CentipedeBody, Sprite &CentipedeHead, int centipedelength[], char centipedeDirections[], float mushroom[][3], float damg_mushroom[][3], float pois_mushrooms[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite)
{

	for (int i = 0; i < maxCentipedes; i++)
	{
		for (int j = 0; j < initial_seg_cnt; j++)
		{
			delete[] centipede[i][j];
		}
		delete[] centipede[i];
	}
	delete[] centipede;

	// Reallocate memory for the 3D array
	centipede = new float **[maxCentipedes] {};
	for (int i = 0; i < maxCentipedes; i++)
	{
		centipede[i] = new float *[initial_seg_cnt];
		for (int j = 0; j < initial_seg_cnt; j++)
		{
			centipede[i][j] = new float[3];
		}
	}
	RegenerateMushrooms(mushroom, damg_mushroom, pois_mushrooms, mushroomSprite, damg_mushroomSprite);
	for (int i = 0; i < mushroom_num; i++)
	{
		mushroom[i][mushroomState] = 1;
	}
	centipedecount = 1;
	centipedelength[0] = initial_seg_cnt;
	centipedeDirections[0] = 'L';
	CreateCentipede(window, centipede, CentipedeBody, CentipedeHead);
}

void RegenerateMushrooms(float mushroom[][3], float damg_mushroom[][3], float pois_mushrooms[][3], Sprite &mushroomSprite, Sprite &damg_mushroomSprite)
{
	CreateMushrooms(mushroom, damg_mushroom, pois_mushrooms, mushroomSprite, damg_mushroomSprite);
}

void DrawPoisonousMushroom(RenderWindow &window, float ***centipede, float bullet[], float pois_mushrooms[][3], Sprite &CentipedeBody, Sprite &pois_mushroomSprite)
{
	for (int i = 0; i < initial_seg_cnt; i++)
	{

		pois_mushroomSprite.setPosition(pois_mushrooms[i][x], pois_mushrooms[i][y]);
		window.draw(pois_mushroomSprite);
	}
}

void CreateSpider(RenderWindow &window, float player[], Sprite &playerSprite, Sprite &SpiderSprite, float spider[], Sound SpiderSound)
{
	int j = rand() % 20;
	spider[x] = j * boxPixelsX;
	spider[y] = resolutionX - boxPixelsX;
	spider[exists] = true;

	SpiderSprite.setPosition(spider[x], spider[y]);
}
void DrawSpider(RenderWindow &window, float player[], Sprite &playerSprite, Sprite &SpiderSprite, float spider[], float mushroom[][3], Sound Spidersound)
{
	const float deltaTime = 1.0f / 100.0f;
	bool checkSpidermush_collision = false;
	float spiderspeedX = 100.0f * deltaTime;
	float spiderspeedY = 100.0f * deltaTime;

	if (spider[y] >= ResLimitY)
	{
		spider[y] -= spiderspeedY;
		if (spider[y] == ResLimitY)
		{
			spider_res_limY = true;
			ResLimitY = resolutionY - boxPixelsY;
		}
	}

	if (spider_res_limY)
	{
		spider[y] += spiderspeedY;
		if (spider[y] == ResLimitY)
		{
			spider_res_limY = false;
			ResLimitY = resolutionY - boxPixelsY * 15;
		}
	}

	if (spider[x] <= ResLimitX)
	{
		spider[x] += spiderspeedX;
		if (spider[x] == ResLimitX)
		{
			spider_res_limX = true;
			ResLimitX = 0;
		}
	}
	if (spider_res_limX)
	{
		spider[x] -= spiderspeedX;
		if (spider[x] == ResLimitX)
		{
			spider_res_limX = false;
			ResLimitX = resolutionX - boxPixelsX;
		}
	}
	SpiderSprite.setScale(1.5f, 1.5f);

	SpiderSprite.setPosition(spider[x], spider[y]);
	if (spider[exists])
	{
		window.draw(SpiderSprite);
	}

	// for (int i = 0; i < mushroom_num; i++)
	// {
	//     if ((abs(spider[x] - mushroom[i][x]) < boxPixelsX) && (abs(spider[y] - mushroom[i][y]) < boxPixelsY / 3))
	//     {
	//         mushroom[i][mushroomState] = 3;
	//     }
	// }
}
void bulletspidCollision(RenderWindow &window, float bullet[], Sprite &SpiderSprite, float spider[])
{
	if (abs(spider[x] - bullet[x]) < boxPixelsX * 2 && abs(spider[y] - bullet[y]) < boxPixelsY * 2)
	{
		if (spider[exists])
		{
			SCORE += 300;
		}
		spider[exists] = false;
		bullet[exists] = false;
	}
}

void CheckPlayercollisions(RenderWindow &window, float player[], Sprite &playerSprite, float ***centipede, float pois_mushrooms[][3], float spider[], Sprite &deadplayerSprite, Sprite &CentipedeBody, Sound sound2)
{
	if (spider[exists])
	{
		if ((abs(player[x] - spider[x]) < boxPixelsX) && (abs(player[y] - spider[y]) < boxPixelsX))
		{

			playerCollided = true;
			deadplayerSprite.setPosition(player[x], player[y]);
			deadplayerSprite.setTextureRect(IntRect(boxPixelsX, 0, boxPixelsX, boxPixelsY));
			window.draw(deadplayerSprite);
			sound2.play();
			sleep(milliseconds(1500));
			window.close();
			return;
		}
	}
}
