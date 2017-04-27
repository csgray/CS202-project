/*
World.cpp
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 9/4/2017
Last Updated: 26/4/2017
Defines functions and object behavior for game screen objects (including background management).
*/

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


///////////////////////////STAR FUNCTIONS/////////////////////////////////
// Creates a new star
void World::makeStar(int startingHeight)
{

    Color starColor(255,255,255,starBrightness(rng));
	sf::Uint8 alpha = starColor.a;

	// Sets star size, shape, and color
	Vector2<float> starSize;

    StarShape* newStar= new StarShape((float)STAR_HEIGHT * (alpha / 255.0), 6);

    // Makes a new star with a random position along width of screen
    newStar->setFillColor(starColor);
    //Makes a new star with a random position along with of screen

    float starX = starDist(rng);
    newStar->setPosition(starX, (float)startingHeight);
	_stars.push_back(newStar);
}

// Fills screen with stars
void World::populateInitialStars(){
    for(int h = 0; h < HEIGHT; ++h){
        //This is finicky
        if(!(h % (STAR_SPAWN_RATE*3))){
            makeStar(h);

        }
    }
}

// Move the stars down the screen and deletes them if they fall off
// Also makes stars according to the spawn rate
void World::updateStars()
{
    // Move all the stars down
    for(int i = _stars.size() - 1; i >= 0; --i)
	{
        _stars[i]->move(0.0, (float)BACKGROUND_SPEED);
        // Move stars to the top with a random width if they reaches the bottom
        if(_stars[i]->getPosition().y > HEIGHT + STAR_HEIGHT)
		{
            _stars[i]->setPosition(starDist(rng), 0);
        }
    }
}
///////////////////////////END STAR FUNCTIONS///////////////////////////

//////////////////////////BULLET FUNCTIONS//////////////////////////////
// Moves bullets up the screen and deletes them if they fall off
void World::updateBullets()
{
    // Loops through the bullets vector backwards
    for(int i = _bullets.size() - 1; i >= 0; --i)
	{
        // Checks if the bullet is off the screen
        if(_bullets[i]->getPosition().x < 0)
		{
            // Removes the bullet if so
            delete _bullets[i];
            _bullets.erase(_bullets.begin() + i);
        }

        // Moves _bullets up if not off the screen
        else{
            _bullets[i]->move(_bullets[i]->_direction.x, _bullets[i]->_direction.y);
        }
    }
}

void World::updatePhotons(){
    for(int i = _photons.size() - 1; i >= 0; --i){
        if(_photons[i]->getPosition().x < 0){
            delete _photons[i];
            _photons.erase(_photons.begin() + i);
        }
        else{
            _photons[i]->_moveCounter += PHOTON_FREQUENCY;
            float xDiff = 3*sin(_photons[i]->_moveCounter);
            float yDiff = -PHOTON_SPEED;
            _photons[i]->movePhoton(xDiff, yDiff);
        }
    }
}
//////////////////////END BULLET FUNCTIONS//////////////////////////////

//////////////////////SHIP FUNCTIONS///////////////////////////////////

// Returns if ship is bounded by the edges of the game window
vector<bounds> World::onBound(const Ship & playerShip) {
    Vector2<float> pos = playerShip.getPosition();
    vector <bounds> playerScope ={EMPTY,EMPTY,EMPTY,EMPTY};

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
void World::makeInitEnemies(){
    static int numSeekers = 0;
    static int numWanderers = 3;
    static int numFollowers = 3;

    for(int i = 0; i < numSeekers; ++i){
        //Makes a seeker at a random width at the top of the screen
        _enemies.push_back(make_seeker());
    }
    for(int i = 0; i < numWanderers; ++i){
        //Makes a seeker at a random width at the top of the screen
        _enemies.push_back(make_wanderer());
    }
    for(int i = 0; i < numFollowers; ++i){
        //Makes a seeker at a random width at the top of the screen
        _enemies.push_back(make_follower());
    }
    numWanderers++;
    numFollowers++;
}

void World::updateEnemies(){
    //Look through all the enemies
    for(int e = _enemies.size() - 1; e >= 0; --e){
        _enemies[e]->update(*this);
        //If dead
        if (_enemies[e]->_hp <= 0 || _enemies[e]->getPosition().y > HEIGHT) {
            if (_enemies[e]->_hp <= 0) {
                _playerShip._playerScore+=25;
                //Split the wanderers into 2 seekers
                if(_enemies[e]->_traits._bulletDodgeForce == 0 && _enemies[e]->_traits._seekTargetForce) {
                    Vector2f pos = _enemies[e]->getPosition();
                    //Splits the wander
                    for(int i = 0; i < SPLIT_NUMBER; i++) {
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
    if (_enemies.size() <= 0) {
        makeInitEnemies();
    }

}
////////////////////////END ENEMY FUNCTIONS/////////////////////////

// Constructor
World::World() : Screens(),
                 _playerShip(Ship())
{
    populateInitialStars();
}
//Cleans up all the memory
World::~World(){
    for(size_t i = 0; i < _bullets.size(); ++i) {
        delete _bullets[i];
    }
    for(size_t i = 0; i < _photons.size(); ++i) {
        delete _photons[i];
    }
    for(size_t i = 0; i < _stars.size(); ++i) {
        delete _stars[i];
    }
    for(size_t i = 0; i < _enemies.size(); ++i) {
        delete _enemies[i];
    }
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

//Draws all the entities to the sfml window
bool World::show(sf::RenderWindow &gameScreen){
    // !!!NTF: Find a way to just loop through all the entities and draw them
    //         instead of having separate loops
    for(const auto & s : _stars){
        gameScreen.draw(*s);
    }
    for(const auto & b : _bullets){
       gameScreen.draw(*b);
    }
    for(const auto & p : _photons){
        gameScreen.draw(*p);
        //this->draw(p.hitBox);
    }
    for(const auto & e : _enemies){
        gameScreen.draw(*e);
    }
    if (!_playerShip._playerIsDead) {
        gameScreen.draw(_playerShip);
        //Draw health bar
        gameScreen.draw(_playerShip._hpBar._currentHealthBar);
        gameScreen.draw(_playerShip._hpBar._maxHealthBar);
        //Draw reload bars
        gameScreen.draw(_playerShip._photonReloadBar._currentHealthBar);
        gameScreen.draw(_playerShip._photonReloadBar._maxHealthBar);

        gameScreen.draw(_playerShip._laserReloadBar._currentHealthBar);
        gameScreen.draw(_playerShip._laserReloadBar._maxHealthBar);

    }
    else
        return false;
    return true;
}

int World::Run(sf::RenderWindow &gameScreen){
    sf::Event event;

    //draw loop
    while(true){
        while(gameScreen.pollEvent(event)){
            if(event.type == sf::Event::Closed) return -1;
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape) return 0;

            }
        }
        if(sf::Joystick::isButtonPressed(0, START)) return 0;
        gameScreen.clear();
        World::update();
        if(!World::show(gameScreen)) return 2;
        gameScreen.display();
    }
}
