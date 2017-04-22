//Ship.cpp
//Auth : William Horn
//4.12.2017
//Definitions for ship class

#include "../../constants.h"
#include "../Ship.h"
#include "../../World.h"

#include <SFML/Graphics.hpp>
using sf::Color;
using sf::Keyboard;
using sf::Vector2;

#include <vector>
using std::vector;

Ship::Ship() : ShipShape(SHIP_RADIUS, 3),
               laserReloadTime(DEFAULT_BULLET_FIRERATE),
               laserReloadSpeed(1),
               laserReloadCounter(0),
               photonReloadTime(DEFAULT_PHOTON_FIRERATE),
               photonReloadSpeed(1),
               photonReloadCounter(0),
               sourceID(PLAYER)
{
    int outline = 2;

    this->setRadius(SHIP_RADIUS);
    this->setOutlineThickness(outline);

    Color outlineColor{183, 183, 183};
    Color fillColor{42, 197, 224};

    this->setFillColor(fillColor);
    this->setOutlineColor(outlineColor);

    this->setPosition(WIDTH / 2, HEIGHT - 2.5*SHIP_RADIUS);
    amountOfLives = 5;
    playerIsDead = false;
    isTouchingEnemy = false;
}

bool Ship::checkIntersect(const EnemyShape &e) {
    return (this->getGlobalBounds().intersects(e.getGlobalBounds()));
}

Bullet Ship::laser(){
    //Gets the x/y position
    float bulletX = this->getPosition().x + SHIP_RADIUS;
    float bulletY = this->getPosition().y;
    Vector2<float> dir(0, -(float)BULLET_SPEED);

    Bullet newBullet(PLAYER, bulletX, bulletY, dir);

    laserReloadCounter = 0;
    return newBullet;
}

Photon Ship::photonCannon(){
    Photon newPhoton{PHOTON_RADIUS};

    float X = getPosition().x + .7*SHIP_RADIUS;
    float Y = getPosition().y - .7*SHIP_RADIUS;

    newPhoton.setPhotonPosition(X, Y);

    photonReloadCounter = 0;
    return newPhoton;
}

// Moves the ship with input from the keyboard and checks if a bullet has been fired
//  !!!NTF: Add acceleration to the movement so instead of
//          this being a direct move it would just apply a force
void Ship::update(World & world){
    //LEFT ARROW TO MOVE LEFT
    if(Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed((Keyboard::A)))){
        if(world.onBound(*this)[1] != LEFT)
           move(-PLAYER_X_SPEED, 0);
    }

    //RIGHT ARROW TO MOVE RIGHT
    if(Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed((Keyboard::D)))){
        if(world.onBound(*this)[0] != RIGHT)
            move(PLAYER_X_SPEED, 0);
    }

    //UP ARROW TO MOVE UP
    if(Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed((Keyboard::W)))){
        if(world.onBound(*this)[2] != UPPER)
            move(0, -PLAYER_Y_SPEED);
    }
    //DOWN ARROW TO MOVE DOWN
    if(Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed((Keyboard::S)))){
        if(world.onBound(*this)[3] != LOWER)
            move(0.0, (float)PLAYER_Y_SPEED);
    }
    //if the reload counter is full and the button is pressed
    if(laserReloadCounter >= laserReloadTime &&
       Keyboard::isKeyPressed(Keyboard::Space)){
        //Fires a bullet from the player ship
        world.bullets.push_back(laser());
    }
    //Add to the reload counter if it's not full
    else {
        laserReloadCounter += laserReloadSpeed;
    }
    //if the reload counter is full and the button is pressed
    if(photonReloadCounter >= photonReloadTime &&
       Keyboard::isKeyPressed(Keyboard::LShift)) {
        //Shoots a photon
        world.photons.push_back(photonCannon());

    }
    //Add to the reload counter if it's not full
    else {
        photonReloadCounter += photonReloadSpeed;
    }

    //Check  if collied with an enemy
    // !!!NTF: Maybe add some damage to the enemies as well
    for (int e = world.enemies.size() - 1; e >= 0; --e) {
        //If the player and an enemy intersect
        if (checkIntersect(world.enemies[e])) {
            //minus a single life per collision
            amountOfLives--;
            setPosition(WIDTH / 2, HEIGHT - 2.5*SHIP_RADIUS);
            if (amountOfLives <= 0) {
                playerIsDead=true;
                break;
            }
        }
    }
}
