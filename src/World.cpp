// world.cpp
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170409
// Updated: 20170412
//
// Definitions and constructor for World class

//Ctor for  world class

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


// Setup for random real number generator for stars
std::random_device World::ranDev;
std::mt19937 World::rng = std::mt19937(ranDev());
std::uniform_real_distribution<float> World::starDist(0.0,(float)WIDTH);
std::uniform_real_distribution<float> World::enemyStartingVel(-10.,10.);
std::uniform_real_distribution<float> World::optimalPlayerDist(HEIGHT / 2, HEIGHT - 100);
std::uniform_int_distribution<int> World::randomInt(-1000, 1000);
std::uniform_int_distribution<int> World::starBrightness(100, 255);


///////////////////////////STAR FUNCTIONS/////////////////////////////////
// Creates a new star
void World::makeStar(float startingHeight)
{
	// Sets star size, shape, and color
	Vector2<float> starSize((float)STAR_HEIGHT, (float)STAR_WIDTH);
    StarShape* newStar= new StarShape(starSize);

    // Makes a new star with a random position along width of screen
    newStar->setFillColor(Color(255,255,255,starBrightness(rng)));
    //Makes a new star with a random position along with of screen

    float starX = starDist(rng);
    newStar->setPosition(starX, startingHeight);
	stars.push_back(newStar);
}

// Fills screen with stars
void World::populateInitialStars(){
    for(int h = 0; h < HEIGHT; ++h){
        //This is finicky
        if(!(h % (STAR_SPAWN_RATE*7))){
            makeStar(h);

        }
    }
}

// Move the stars down the screen and deletes them if they fall off
// Also makes stars according to the spawn rate
void World::updateStars()
{
    // Move all the stars down
    for(int i = stars.size() - 1; i >= 0; --i)
	{
        stars[i]->move(0.0, (float)BACKGROUND_SPEED);
        // Move stars to the top with a random width if they reaches the bottom
        if(stars[i]->getPosition().y > HEIGHT + STAR_HEIGHT)
		{
            stars[i]->setPosition(starDist(rng), 0);
        }
    }
}
///////////////////////////END STAR FUNCTIONS///////////////////////////

//////////////////////////BULLET FUNCTIONS//////////////////////////////
// Moves bullets up the screen and deletes them if they fall off
void World::updateBullets()
{
    // Loops through the bullets vector backwards
    for(int i = bullets.size() - 1; i >= 0; --i)
	{
        // Checks if the bullet is off the screen
        if(bullets[i]->getPosition().x < 0)
		{
            // Removes the bullet if so
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
        }

        // Moves bullets up if not off the screen
        else{
            bullets[i]->move(bullets[i]->direction.x, bullets[i]->direction.y);
        }
    }
}

void World::updatePhotons(){
    for(int i = photons.size() - 1; i >= 0; --i){
        if(photons[i]->getPosition().x < 0){
            delete photons[i];
            photons.erase(photons.begin() + i);
        }
        else{
            photons[i]->moveCounter += PHOTON_FREQUENCY;
            float xDiff = 3*sin(photons[i]->moveCounter);
            float yDiff = -PHOTON_SPEED;
            photons[i]->movePhoton(xDiff, yDiff);
        }
    }
}
//////////////////////END BULLET FUNCTIONS//////////////////////////////

//////////////////////SHIP FUNCTIONS///////////////////////////////////
Ship World::getPlayerShip() {
    return playerShip;

}

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
// Creates first wave of enemies
void World::makeInitEnemies(){
    for(int h = 5; h < HEIGHT / 2; h += ENEMY_HEIGHT + 5){
        Vector2<float> starting_pos(starDist(rng), h);

        Vector2<float> starting_dir = (randomInt(rng) % 2) ? Vector2<float>(-1,0) : Vector2<float>(1,0);

        enemies.push_back(new Enemy(starting_pos, starting_dir, 5, 5));
    }
}

void World::updateEnemies(){
    //Look through all the enemies
    for(int e = enemies.size() - 1; e >= 0; --e){
        enemies[e]->update(*this);
        if (enemies[e]->hp <= 0) {
            delete enemies[e];
            enemies.erase(enemies.begin()+e);
        }
    }

}
////////////////////////END ENEMY FUNCTIONS/////////////////////////

// Constructor
World::World() : RenderWindow(VideoMode(WIDTH, HEIGHT), "ASTEROIDS"),
                 playerShip(Ship())
{
    populateInitialStars();
    makeInitEnemies();
    this->setFramerateLimit(FRAMERATE);
}

// Updates all the entities in the game world
void World::update()
{
    playerShip.update(*this);
    updateStars();
    updateBullets();
    updatePhotons();
    updateEnemies();
}

//Draws all the entities to the sfml window
void World::show(){
    // !!!NTF: Find a way to just loop through all the entities and draw them
    //         instead of having separate loops
    for(const auto & s : stars){
        draw(*s);
    }
    for(const auto & b : bullets){
        draw(*b);
    }
    for(const auto & p : photons){
        draw(*p);
        //this->draw(p.hitBox);
    }
    for(const auto & e : enemies){
        draw(*e);
    }
    if (!playerShip.playerIsDead) {
        draw(playerShip);
    }

}
