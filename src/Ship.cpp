/*
Ship.cpp
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 12/4/2017
Last Updated: 26/4/2017
Defines characteristics of ship class objects.
*/

#include "Ship.h"
#include "World.h"
#include "healthbar.h"

#include <SFML/Graphics.hpp>
using sf::Color;
using sf::Keyboard;
using sf::Vector2;
using sf::Vector2f;
#include<SFML/Window/Joystick.hpp>

#include <vector>
using std::vector;
#include <iostream>

#include "Loader.h"

Ship::Ship() : ShipShape(),
               _vel(Vector2f(0.0, 0.0)),
               _accel(Vector2f(0.0, 0.0)),
               _laserReloadTime(DEFAULT_BULLET_FIRERATE),
               _laserReloadSpeed(1),
               _laserReloadCounter(0),
               _photonReloadTime(DEFAULT_PHOTON_FIRERATE),
               _photonReloadSpeed(1),
               _photonReloadCounter(_photonReloadTime),
               _photonReloadBar(ReloadBar(RELOAD_BAR_HEIGHT, RELOAD_BAR_WIDTH)),
               _laserReloadBar(ReloadBar(RELOAD_BAR_HEIGHT, RELOAD_BAR_WIDTH)),
               _health(STARTING_HP),
               _maxHP(_health),
               _playerScore(0),
               _hpBar(HealthBar()),
               _sourceID(PLAYER)

{
    load_texture(_shipTexture,"resources/sprites/MiG-51S.png");
    setTexture(_shipTexture);

    setScale(.75,.75);
    setTextureRect(sf::IntRect(0,5,57,98));

    setPosition(WIDTH / 2, HEIGHT - 2.5*SHIP_RADIUS);
    _playerIsDead = false;
    _isTouchingEnemy = false;
    _inInvincibleFrame = false;
    _bleed = 0;
    _clock.restart();

    _hpBar.setCurrentHealthBar(1.0);
    _photonReloadBar.setCurrentHealthBar(1.0);
    _laserReloadBar.setCurrentHealthBar(1.0);

    float reloadBarBuffer = 5.0F;

    _photonReloadBar._currentHealthBar.rotate(180);
    _laserReloadBar._currentHealthBar.rotate(180);

    //Change the positions of the reload bars
    _laserReloadBar._currentHealthBar.setPosition(Vector2f(reloadBarBuffer + RELOAD_BAR_HEIGHT, HEIGHT - reloadBarBuffer));
    _laserReloadBar._maxHealthBar.setPosition(Vector2f(reloadBarBuffer, HEIGHT - reloadBarBuffer - RELOAD_BAR_WIDTH));
    _photonReloadBar._currentHealthBar.setPosition(Vector2f(2.5F*reloadBarBuffer + 2*RELOAD_BAR_HEIGHT, HEIGHT - reloadBarBuffer));
    _photonReloadBar._maxHealthBar.setPosition(Vector2f(2.5F*reloadBarBuffer + RELOAD_BAR_HEIGHT, HEIGHT - reloadBarBuffer - RELOAD_BAR_WIDTH));

    _photonReloadBar._currentHealthBar.setFillColor(Color(66, 164, 244));
    _laserReloadBar._currentHealthBar.setFillColor(Color(244, 163, 65));

    _photonReloadBar._maxHealthBar.setOutlineThickness(2);
    _laserReloadBar._maxHealthBar.setOutlineThickness(2);

}

bool Ship::checkIntersect(const sf::Shape &e) {
    return (getGlobalBounds().intersects(e.getGlobalBounds()));
}

bool Ship::checkIntersect(const sf::Sprite &e) {
    return (getGlobalBounds().intersects(e.getGlobalBounds()));
}

Bullet* Ship::laser(){
    //Gets the x/y position
    float bulletX = this->getPosition().x + SHIP_RADIUS;
    float bulletY = this->getPosition().y;
    Vector2<float> dir(0, -BULLET_SPEED);

    Bullet* newBullet = new Bullet(PLAYER, bulletX, bulletY, dir);

    _laserReloadCounter = 0;
    return newBullet;
}

Photon* Ship::photonCannon(){
    Photon* newPhoton = new Photon{PHOTON_RADIUS};

    float X = getPosition().x + 0.7F*SHIP_RADIUS;
    float Y = getPosition().y - 0.7F*SHIP_RADIUS;

    newPhoton->setPhotonPosition(X, Y);

    _photonReloadCounter = 0;
    return newPhoton;
}

// Moves the ship with input from the keyboard and checks if a bullet has been fired
//  !!!NTF: Add acceleration to the movement so instead of
//          this being a direct move it would just apply a force
void Ship::update(World & world){
    ///////////////////////movement///////////////////////
    int joystickDetectionThreshold = 10;
    float xAccel;
    float yAccel;

    //LEFT ARROW TO MOVE LEFT
    if(Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed((Keyboard::A)))
       || (sf::Joystick::getAxisPosition(0,sf::Joystick::X)) < -joystickDetectionThreshold){
        if(sf::Joystick::isConnected(0))
           xAccel = (float)-PLAYER_X_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::X) / -100);
        else
            xAccel = -PLAYER_X_ACCEL;
        _accel += Vector2f(xAccel, 0);
    }

    //RIGHT ARROW TO MOVE RIGHT
    if(Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed((Keyboard::D)))
       ||(sf::Joystick::getAxisPosition(0,sf::Joystick::X)) > joystickDetectionThreshold){
        if(sf::Joystick::isConnected(0))
            xAccel = (float)PLAYER_X_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::X) / 100);
        else
            xAccel = PLAYER_X_ACCEL;
        _accel += Vector2f(xAccel, 0);
    }

    //UP ARROW TO MOVE UP
    if(Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed((Keyboard::W)))
       || sf::Joystick::getAxisPosition(0,sf::Joystick::Y) < -joystickDetectionThreshold){
        if(sf::Joystick::isConnected(0))
            yAccel = (float)-PLAYER_Y_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::Y) / -100);
        else
            yAccel = -PLAYER_Y_ACCEL;
        _accel += Vector2f(0, yAccel);
    }
    //DOWN ARROW TO MOVE DOWN
    if(Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed((Keyboard::S)))
       || sf::Joystick::getAxisPosition(0,sf::Joystick::Y) > joystickDetectionThreshold){
            if(sf::Joystick::isConnected(0))
                yAccel = (float)PLAYER_Y_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::Y) / 100);
            else
                yAccel = PLAYER_Y_ACCEL;
            _accel += Vector2f(0, yAccel);
    }
    //Add the acceleration to the _velocity
    _vel += _accel;

    _accel *= (float)0;

    //Slight drag to make things easier to control
    _vel *= (float)SHIP_DRAG;

    float velocityMag = sqrt(_vel.y * _vel.y + _vel.x * _vel.x);
    //Going faster then the max speed
    if(velocityMag >= MAX_SPEED) {
        //Normalize
        _vel /= (float)velocityMag;


        //Scale to the max speed
        _vel *= (float)MAX_SPEED;

    }
    //Check if the ship is on a bound
    if(world.onBound(*this)[0] == RIGHT) {
        //If the ship has a rightward(?) velocity
        if(_vel.x > 0) {
            _vel.x = 0;
        }
    }
    if(world.onBound(*this)[1] == LEFT) {
        //If the ship has a leftward(?) velocity
        if(_vel.x < 0) {
            _vel.x = 0;
        }
    }

    if(world.onBound(*this)[2] == UPPER) {
        //If the ship has a upward velocity
        if(_vel.y < 0) {
            _vel.y = 0;
        }
    }
    if(world.onBound(*this)[3] == LOWER) {
        //If the ship has a downward velocity
        if(_vel.y > 0) {
            _vel.y = 0;
        }
    }

    /////////////////weapons and enemies////////////////

    //if the reload counter is full and the button is pressed
    if((_laserReloadCounter >= _laserReloadTime && Keyboard::isKeyPressed(Keyboard::Space)) ||
       (_laserReloadCounter >= _laserReloadTime && sf::Joystick::getAxisPosition(0,sf::Joystick::Z) < -98)) {
        //Fires a bullet from the player ship
        world._bullets.push_back(laser());
    }
    //Add to the reload counter if it's not full
    if(_laserReloadCounter <= _laserReloadTime){
        _laserReloadCounter += _laserReloadSpeed;
    }
    //if the reload counter is full and the button is pressed

    if(_photonReloadCounter >= _photonReloadTime && Keyboard::isKeyPressed(Keyboard::E) ||
       _photonReloadCounter >= _photonReloadTime && sf::Joystick::isButtonPressed(0, X)) {

        //Shoots a photon
        world._photons.push_back(photonCannon());

    }
    //Add to the reload counter if it's not full
     if(_photonReloadCounter <= _photonReloadTime) {
        _photonReloadCounter += _photonReloadSpeed;
    }

    if(_bleed <= 0) setColor(Color{255,255,255});
    else --_bleed;

    if (_inInvincibleFrame && _clock.getElapsedTime().asMilliseconds() > 500) {
        _inInvincibleFrame = false;
    }

    //Check  if collied with an enemy
    // !!!NTF: Maybe add some damage to the enemies as well
    for (int e = world._enemies.size() - 1; !_inInvincibleFrame && e >= 0; --e) {
        //If the player and an enemy intersect
        if (checkIntersect(*world._enemies[e])) {
            //minus a single life per collision
            _health--;
            world._enemies[e]->_hp--;
            if (!_inInvincibleFrame) {
                _inInvincibleFrame = true;
                _clock.restart();
            }

            //Find the center of the enemy
            Vector2f enemyPos = world._enemies[e]->getPosition();
            enemyPos.x += ENEMY_WIDTH / 2;
            enemyPos.y += ENEMY_HEIGHT / 2;

            //Find the center of the ship
            Vector2f shipCenter = getPosition();
            shipCenter.x += SHIP_RADIUS;
            shipCenter.y += SHIP_RADIUS;

            Vector2f collisionForce = shipCenter - enemyPos;

            collisionForce *= (float).2;

            _vel += collisionForce;

            //Apply an opposite force to the enemy
            collisionForce *= (float)-1;
            world._enemies[e]->_vel += collisionForce;

            //Flash red if hit by an enemy
            setColor(Color{244, 66, 66, 200});
            _bleed = 10;

            if (_health <= 0) {
                _playerIsDead=true;
                break;
            }
        }
    }
    //Check if an enemy bullet hits the player
    //  !!!NTF: Separate out the player bullets and
    //          the enemy bullets into separate arrays
    for(int b = world._bullets.size() - 1; !_inInvincibleFrame && b >= 0; --b) {
        if(world._bullets[b]->_source == ENEMY) {
            //If the bullets hits
            if(checkIntersect(*world._bullets[b])) {
                //Remove a life

                _health--;
                if (!_inInvincibleFrame) {
                    _inInvincibleFrame = true;
                    _clock.restart();
                }

                setColor(Color{244, 66, 66, 200});
                _bleed = 5;

                delete world._bullets[b];
                world._bullets.erase(world._bullets.begin() + b);

                //If yr dead...
                if (_health <= 0) {
                    _playerIsDead=true;
                    break;
                }


            }
        }
    }

    //Move the players ship
    move(_vel);

    if(_vel.x < -SWITCH_THRESHHOLD)        setTextureRect(sf::IntRect(70,0,43,99));
    else if (_vel.x > SWITCH_THRESHHOLD)   setTextureRect(sf::IntRect(120,0,43,99));
    else                setTextureRect(sf::IntRect(0,5,57,98));

    //Set the _health bar correctly
    float percentHP = _health / _maxHP;
    _hpBar.setCurrentHealthBar(percentHP);
    //Set the reload bars
    float percentLaserReload = (float)_laserReloadCounter / _laserReloadTime;
    if(percentLaserReload >= 1) percentLaserReload = 1.0F;
    _laserReloadBar.setCurrVertical(percentLaserReload);

    float percentPhotonReload = (float)_photonReloadCounter / _photonReloadTime;
    if(percentPhotonReload >= 1) percentPhotonReload = 1.0F;
    _photonReloadBar.setCurrVertical(percentPhotonReload);



}
