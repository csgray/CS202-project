// world.cpp
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170409
// Updated: 20170412
//
// Definitions and constructor for World class

#include "constants.h"
#include "World.h"
#include "Ship.h"
#include "Enemy.h"
#include "Photon.h"
#include "Loader.h"

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Color;
using sf::Vector2;
using sf::Keyboard;
using sf::CircleShape;
#include <SFML/Audio.hpp>
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::sin;
using std::floor;

// Setup for random real number generator for _stars
std::random_device World::ranDev;
std::mt19937 World::rng = std::mt19937(ranDev());

std::uniform_real_distribution<float> World::starDist(0.0,(float)WIDTH);
std::uniform_real_distribution<float> World::enemyStartingVel(-10.,10.);
std::uniform_real_distribution<float> World::optimalPlayerDist(HEIGHT / 2, HEIGHT - 100);

std::uniform_int_distribution<int> World::randomInt(-1000, 1000);
std::uniform_int_distribution<int> World::starBrightness(100, 255);
std::uniform_int_distribution<int> World::randomSplitNumber(3, 5);

std::uniform_real_distribution<float> World::randomWidht(20, WIDTH - 20);
std::uniform_real_distribution<float> World::randomHeight(20, HEIGHT - 20);

///////////////////////////STAR FUNCTIONS/////////////////////////////////
// Creates a new star
void World::makeStar(int startingHeight)
{
	// Star color
    Color starColor(255,255,255,starBrightness(rng));
	sf::Uint8 alpha = starColor.a;

	// Sets star size, shape, and color
    StarShape* newStar= new StarShape((float)STAR_HEIGHT * (alpha / 255.0F), 6);
    newStar->setFillColor(starColor);

    //Makes a new star with a random position along with of screen
    float starX = starDist(rng);
    newStar->setPosition(starX, (float)startingHeight);

	//Add star into the world
	_stars.push_back(newStar);
}

// Fills screen with stars
void World::populateInitialStars()
{
    for(int h = 0; h < HEIGHT; ++h)
	{
        //This is finicky
        if(!(h % (STAR_SPAWN_RATE*3)))
        {
            makeStar(h);
        }
	}
}

// Move the stars down the screen and deletes them if they fall off
// Also makes stars according to the spawn rate
void World::updateStars()
{
    // Move all the stars down
    for(int star = _stars.size() - 1; star >= 0; --star)
	{
        _stars[star]->move(0.0, (float)BACKGROUND_SPEED);

        // Move stars to the top with a random width if they reaches the bottom
        if(_stars[star]->getPosition().y > HEIGHT + STAR_HEIGHT)
		{
            _stars[star]->setPosition(starDist(rng), 0);
        }
    }
}
///////////////////////////END STAR FUNCTIONS///////////////////////////

//////////////////////////WEAPON FUNCTIONS//////////////////////////////
// Moves bullets up the screen and deletes them if they fall off
void World::updateBullets()
{
    // Loops through the bullets vector backwards
    for(int bullet = _bullets.size() - 1; bullet >= 0; --bullet)
	{
        // Checks if the bullet is off the screen
        if(_bullets[bullet]->getPosition().x < 0)
		{
            // Removes the bullet if so
            delete _bullets[bullet];
            _bullets.erase(_bullets.begin() + bullet);
        }

        // Moves bullets up if not off the screen
        else
		{
            _bullets[bullet]->move(_bullets[bullet]->_direction.x, _bullets[bullet]->_direction.y);
        }
    }
}

void World::updatePhotons()
{
    // Loops through the photons vector backwards
	for(int photon = _photons.size() - 1; photon >= 0; --photon)
	{
		// Checks if the photon is off the screen
		if(_photons[photon]->getPosition().x < 0)
		{
            // Removes the photon if so
			delete _photons[photon];
            _photons.erase(_photons.begin() + photon);
        }

		// Moves photons up in wave pattern if not off screen
		else
		{
            _photons[photon]->_moveCounter += PHOTON_FREQUENCY;
            float xDiff = 3*sin(_photons[photon]->_moveCounter);
            float yDiff = -PHOTON_SPEED;
            _photons[photon]->movePhoton(xDiff, yDiff);
        }
    }
}
//////////////////////END WEAPON FUNCTIONS//////////////////////////////

//////////////////////SHIP FUNCTIONS////////////////////////////////////
// Returns if ship is bounded by the edges of the game window
vector<bounds> World::onBound(const Ship & playerShip)
{
    Vector2<float> pos = playerShip.getPosition();
    vector <bounds> playerScope ={EMPTY,EMPTY,EMPTY,EMPTY};

	// Ship x and y-coordinates
    float shipX = pos.x;
    float shipY = pos.y;

    // On right bound
    if(shipX >= WIDTH-(SHIP_RADIUS*2))
        playerScope[0]= RIGHT;
    // On left bound
    if(shipX < SHIP_RADIUS/8)
        playerScope[1] = LEFT;
    // On upper bound
    if(shipY < SHIP_RADIUS/2)
        playerScope[2] = UPPER;
    // On lower bound
    if(shipY > HEIGHT - (SHIP_RADIUS*2))
        playerScope[3]= LOWER;

    return playerScope;
}
//////////////////////////END SHIP FUNCTIONS/////////////////////////

////////////////////////ENEMY FUNCTIONS/////////////////////////////

// Creates first wave of _enemies
void World::makeInitEnemies()
{
    static int numSeekers = 0;
    static int numWanderers = 1;
    static int numFollowers = 1;

	// Makes a seeker at a random width at the top of the screen
    for(int seeker = 0; seeker < numSeekers; ++seeker)
         _enemies.push_back(make_seeker());

	// Makes a wanderer at a random width at the top of the screen
    for(int wanderer = 0; wanderer < numWanderers; ++wanderer)
        _enemies.push_back(make_wanderer());

	//Makes a seeker at a random width at the top of the screen
    for(int follower = 0; follower < numFollowers; ++follower)
        _enemies.push_back(make_follower());

    //Spawn life up powerup
    _powerups.push_back(new Powerup(Vector2f(randomWidht(rng),randomHeight(rng)), LIFE_UP));
    //Spawn reload up powerup
    _powerups.push_back(new Powerup(Vector2f(randomWidht(rng),randomHeight(rng)), RELOAD_UP));

    //Add to the number of enemies spawned next round
    numSeekers++;
    numWanderers++;
    numFollowers++;
}

void World::updateEnemies()
{
    // Loop through all the enemies
    for(int e = _enemies.size() - 1; e >= 0; --e)
	{
        _enemies[e]->update(*this);

        //If dead
        if (_enemies[e]->_hp <= 0 || //Destroyed
            _enemies[e]->getPosition().y > HEIGHT) // Off the screen
        {
            Vector2f pos = _enemies[e]->getPosition();

            if (_enemies[e]->_hp <= 0)
            {
                // Add to the score
                _playerShip._playerScore+=25;

                // Split the wanderers into seekers
                if(_enemies[e]->_traits._bulletDodgeForce == 0 &&
                   _enemies[e]->_traits._seekTargetForce)
                {
                    // Make a random amount between 3 and 6
                    int splitNum = randomSplitNumber(rng);

                    // Splits the wander
                    for(int i = 0; i < splitNum; i++)
                    {
                        Enemy * newSeeker = make_seeker();
                        newSeeker->setPosition(pos);
                        _enemies.push_back(newSeeker);
                    }

                }
            }

            delete _enemies[e];
            _enemies.erase(_enemies.begin()+e);
        }
    }

	// Spawns a new wave if all enemies are destroyed
    if (_enemies.size() <= 0)
    {
        makeInitEnemies();
    }
}
////////////////////////END ENEMY FUNCTIONS/////////////////////////

// Constructor
World::World() : Screens(),
                 _playerShip(Ship())


{
    populateInitialStars();
    //bg sound object
    load_buffer(_bgBuffer,"resources/sound/bgMusic.wav");
    _bgSound.setBuffer(_bgBuffer);
    _bgSound.setLoop(true);
    _bgSound.setPitch(.7);
    _bgSound.play();
}

// Cleans up all the memory
World::~World()
{
    for(size_t i = 0; i < _bullets.size(); ++i)
        delete _bullets[i];

	for(size_t i = 0; i < _photons.size(); ++i)
        delete _photons[i];

	for(size_t i = 0; i < _stars.size(); ++i)
        delete _stars[i];

	for(size_t i = 0; i < _enemies.size(); ++i)
        delete _enemies[i];
}

// Updates all the entities in the game world
void World::update()
{
    _playerShip.update(*this);
    updateStars();
    updateBullets();
    updatePhotons();
    updateEnemies();
}

// Draws all the entities to the sfml window
void World::show(sf::RenderWindow &gameScreen)
{
    // !!!NTF: Find a way to just loop through all the entities and draw them
    //         instead of having separate loops
	for (const auto & s : _stars)
		gameScreen.draw(*s);

	for (const auto & b : _bullets)
       gameScreen.draw(*b);

	for (const auto & p : _photons)
        gameScreen.draw(*p);

	for (const auto & e : _enemies)
        gameScreen.draw(*e);

    for(const auto & u : _powerups)
        gameScreen.draw(*u);


    gameScreen.draw(_playerShip);

    if (!_playerShip._playerIsDead)
	{
        // Draw ship
		gameScreen.draw(_playerShip);

		// Draw health bar
        gameScreen.draw(_playerShip._hpBar._currentHealthBar);
        gameScreen.draw(_playerShip._hpBar._maxHealthBar);

		// Draw photon reload bar
        gameScreen.draw(_playerShip._photonReloadBar._currentHealthBar);
        gameScreen.draw(_playerShip._photonReloadBar._maxHealthBar);

		// Draw laser reload bar
        gameScreen.draw(_playerShip._laserReloadBar._currentHealthBar);
        gameScreen.draw(_playerShip._laserReloadBar._maxHealthBar);
    }
    if (_playerShip._playerIsDead) {
        _bgSound.stop();
    }
}

// Game window
int World::Run(sf::RenderWindow &gameScreen)
{
    sf::Event event;
	// Game loop
    while(true)
	{
	    // Checks for input
		while(gameScreen.pollEvent(event))
		{
            if(event.type == sf::Event::Closed) return -1;
            if(event.type == sf::Event::KeyPressed)
			{
                if(event.key.code == sf::Keyboard::Escape) return 0;

            }
             if(_playerShip._playerIsDead==true) return 2;
        }
        //If start button is pressed go to the pause menu
        if(sf::Joystick::isButtonPressed(0, START)) return 0;

		// Updates display
        gameScreen.clear();
        World::update();
        World::show(gameScreen);
        gameScreen.display();
    }
}
