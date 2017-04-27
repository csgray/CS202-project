// Ship.cpp
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170412
// Updated: 20170426
//
// Definitions for Ship struct

#include "Ship.h"
#include "World.h"
#include "healthbar.h"
#include "gameover.h"
#include "Loader.h"

#include <SFML/Graphics.hpp>
using sf::Color;
using sf::Keyboard;
using sf::Vector2;
using sf::Vector2f;
#include<SFML/Window/Joystick.hpp>

#include <vector>
using std::vector;
#include <iostream>

// Constructor
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
    // Loads texture
	load_texture(_shipTexture,"resources/sprites/MiG-51S.png");
    setTexture(_shipTexture);
	setScale(.75,.75);
    setTextureRect(sf::IntRect(0,5,57,98));

    //Load ship sound files
    load_buffer(_laserBuffer, "resources/sound/laserSound.wav");
    _laserSound.setBuffer(_laserBuffer);
    load_buffer(_photonBuffer,"resources/sound/photonSound.wav");
    _photonSound.setBuffer(_photonBuffer);

	// Starting position
    setPosition(WIDTH / 2, HEIGHT - 2.5*SHIP_RADIUS);

	// Starting health and invincibility
	_playerIsDead = false;
    _isTouchingEnemy = false;
    _inInvincibleFrame = false;
    _bleed = 0;
    _clock.restart();

	// Health and reload bars
	float reloadBarBuffer = 5.0F;
	_hpBar.setCurrentHealthBar(1.0);
    _photonReloadBar.setCurrentHealthBar(1.0);
    _laserReloadBar.setCurrentHealthBar(1.0);

    _hasDoubleLaser = false;

	_photonReloadBar._currentHealthBar.rotate(180);
    _laserReloadBar._currentHealthBar.rotate(180);

    // Changes the position of the laser reload bars
    _laserReloadBar._currentHealthBar.setPosition(Vector2f(reloadBarBuffer + RELOAD_BAR_HEIGHT, HEIGHT - reloadBarBuffer));
    _laserReloadBar._maxHealthBar.setPosition(Vector2f(reloadBarBuffer, HEIGHT - reloadBarBuffer - RELOAD_BAR_WIDTH));
	_laserReloadBar._currentHealthBar.setFillColor(Color(244, 163, 65));
	_laserReloadBar._maxHealthBar.setOutlineThickness(2);

	// Changes the position of the photon reload bar
	_photonReloadBar._currentHealthBar.setPosition(Vector2f(2.5F*reloadBarBuffer + 2*RELOAD_BAR_HEIGHT, HEIGHT - reloadBarBuffer));
    _photonReloadBar._maxHealthBar.setPosition(Vector2f(2.5F*reloadBarBuffer + RELOAD_BAR_HEIGHT, HEIGHT - reloadBarBuffer - RELOAD_BAR_WIDTH));
	_photonReloadBar._currentHealthBar.setFillColor(Color(66, 164, 244));
    _photonReloadBar._maxHealthBar.setOutlineThickness(2);
}

// Collision detection with shapes
bool Ship::checkIntersect(const sf::Shape &e) {
    return (getGlobalBounds().intersects(e.getGlobalBounds()));
}

// Collision detection with sprites
bool Ship::checkIntersect(const sf::Sprite &e) {
    return (getGlobalBounds().intersects(e.getGlobalBounds()));
}

// Laser weapon
Bullet* Ship::laser()
{
    // Gets the ship's x/y position
    float bulletX = this->getPosition().x + SHIP_RADIUS;
    float bulletY = this->getPosition().y;
    Vector2<float> dir(0, -BULLET_SPEED);

	// Creates a bullet
    Bullet* newBullet = new Bullet(PLAYER, bulletX, bulletY, dir);

	// Resets reload timer
    _laserReloadCounter = 0;

    return newBullet;
}

// Photon weapon
Photon* Ship::photonCannon()
{
	// Gets the ship's x/y coordinates
	float X = getPosition().x + 0.7F*SHIP_RADIUS;
	float Y = getPosition().y - 0.7F*SHIP_RADIUS;

	// Creates a photon
	Photon* newPhoton = new Photon{PHOTON_RADIUS};
	newPhoton->setPhotonPosition(X, Y);

	// Resets reload timer
    _photonReloadCounter = 0;

    return newPhoton;
}

// Moves the ship with input from the keyboard and checks if a bullet has been fired
void Ship::update(World & world)
{
    // Starting movement values
    int joystickDetectionThreshold = 10;
    float xAccel;
    float yAccel;

    // Left movement
    if(Keyboard::isKeyPressed(Keyboard::Left) || // Arrow keys
	   Keyboard::isKeyPressed(Keyboard::A) || // WASD
	   sf::Joystick::getAxisPosition(0,sf::Joystick::X) < -joystickDetectionThreshold) // Game controller
	{
        if(sf::Joystick::isConnected(0))
           xAccel = (float)-PLAYER_X_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::X) / -100);

		else
            xAccel = -PLAYER_X_ACCEL;

        _accel += Vector2f(xAccel, 0);
    }

    // Right movement
    if(Keyboard::isKeyPressed(Keyboard::Right) || // Arrow keys
	   Keyboard::isKeyPressed(Keyboard::D) || // WASD
	   sf::Joystick::getAxisPosition(0,sf::Joystick::X) > joystickDetectionThreshold) // Game controller
	{
        if(sf::Joystick::isConnected(0))
            xAccel = (float)PLAYER_X_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::X) / 100);

		else
            xAccel = PLAYER_X_ACCEL;

		_accel += Vector2f(xAccel, 0);
    }

    // Up movement
    if(Keyboard::isKeyPressed(Keyboard::Up) || // Arrow keys
	   Keyboard::isKeyPressed(Keyboard::W) || // WASD
	   sf::Joystick::getAxisPosition(0,sf::Joystick::Y) < -joystickDetectionThreshold) // Game controller
	{
        if(sf::Joystick::isConnected(0))
            yAccel = (float)-PLAYER_Y_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::Y) / -100);

		else
            yAccel = -PLAYER_Y_ACCEL;

		_accel += Vector2f(0, yAccel);
    }

    // Down movement
    if(Keyboard::isKeyPressed(Keyboard::Down) || // Arrow Keys
	   Keyboard::isKeyPressed(Keyboard::S) || // WASD
	   sf::Joystick::getAxisPosition(0,sf::Joystick::Y) > joystickDetectionThreshold) // Game controller
	{
            if(sf::Joystick::isConnected(0))
                yAccel = (float)PLAYER_Y_ACCEL*(sf::Joystick::getAxisPosition(0,sf::Joystick::Y) / 100);

			else
                yAccel = PLAYER_Y_ACCEL;

			_accel += Vector2f(0, yAccel);
    }

	// Add the acceleration to the _velocity
    _vel += _accel;
	_accel *= (float)0;

    //Slight drag to make things easier to control
    _vel *= (float)SHIP_DRAG;

    float velocityMag = sqrt(_vel.y * _vel.y + _vel.x * _vel.x);

	// Checks if going faster then the max speed
    if(velocityMag >= MAX_SPEED)
	{
        // Normalize
        _vel /= (float)velocityMag;

		// Scale to the max speed
        _vel *= (float)MAX_SPEED;
    }

	// Stops the ship from going off the right edge
    if(world.onBound(*this)[0] == RIGHT)
	{
        // If the ship has a rightward(?) velocity
        if(_vel.x > 0)
		{
            _vel.x = 0;
        }
    }

	// Stops the ship from going off the left edge
    if(world.onBound(*this)[1] == LEFT)
	{
        // If the ship has a leftward(?) velocity
        if(_vel.x < 0)
		{
            _vel.x = 0;
        }
    }

	// Stops the ship from going off the upper edge
    if(world.onBound(*this)[2] == UPPER)
	{
        // If the ship has a upward velocity
        if(_vel.y < 0) {
            _vel.y = 0;
        }
    }

	// Stops the ship from going off the bottom edge
    if(world.onBound(*this)[3] == LOWER)
	{
        // If the ship has a downward velocity
        if(_vel.y > 0)
		{
            _vel.y = 0;
        }
    }


    /////////////////weapons and enemies////////////////

    //if the reload counter is full and the button is pressed
    if(((_laserReloadCounter >= _laserReloadTime) && (Keyboard::isKeyPressed(Keyboard::Space))) ||
       ((_laserReloadCounter >= _laserReloadTime) && (sf::Joystick::getAxisPosition(0,sf::Joystick::Z) < -98)))
    {
        if(!_hasDoubleLaser)
        {
            world._bullets.push_back(laser());
            _laserSound.play();
        }
        else
        { //HAS DOUBLE LASER!!!
            Bullet * bullet1 = laser();
            Bullet * bullet2 = laser();

            //Get the position of the ship
            Vector2f shipPos = getPosition();

            float bulletY = shipPos.y;

            float bullet1X = shipPos.x + 12;
            float bullet2X = shipPos.x + 2*SHIP_RADIUS - 12;

            bullet1->setPosition(bullet1X, bulletY);
            bullet2->setPosition(bullet2X, bulletY);

            bullet1->setScale(0.7F, 1);
            bullet2->setScale(0.7F, 1);

            _laserSound.play();
            world._bullets.push_back(bullet1);
            world._bullets.push_back(bullet2);
        }
    }
    //Add to the reload counter if it's not full
    if(_laserReloadCounter <= _laserReloadTime){
        _laserReloadCounter += _laserReloadSpeed;
    }

    // Photon
    // Fires if the reload counter is full and the button is pressed
    if(_photonReloadCounter >= _photonReloadTime && Keyboard::isKeyPressed(Keyboard::E) ||
       _photonReloadCounter >= _photonReloadTime && sf::Joystick::isButtonPressed(0, X))
    {
        //Shoots a photon
        _photonSound.play();
        world._photons.push_back(photonCannon());
    }
    // Add to the reload counter if it's not full
     if(_photonReloadCounter <= _photonReloadTime)
    {
        _photonReloadCounter += _photonReloadSpeed;
    }

	// Red hue when damaged
    if(_bleed <= 0) setColor(Color{255,255,255});
    else --_bleed;

	// Disables invincibility after 500 milliseconds
    if (_inInvincibleFrame && _clock.getElapsedTime().asMilliseconds() > 500)
	{
        _inInvincibleFrame = false;
    }
    //Check  if collied with an enemy
    // !!!NTF: Maybe add some damage to the enemies as well
    for (int enemy = (int)world._enemies.size() - 1; !_inInvincibleFrame && enemy >= 0; --enemy)
    {
        //If the player and an enemy intersect
        if (checkIntersect(*world._enemies[enemy]))
        {
            //minus a single life per collision
            _health--;
            world._enemies[enemy]->_hp--;

			// Start invincibility frame
			if (!_inInvincibleFrame)
			{
                _inInvincibleFrame = true;
                _clock.restart();
            }

            // Find the center of the enemy
            Vector2f enemyPos = world._enemies[enemy]->getPosition();
            enemyPos.x += ENEMY_WIDTH / 2;
            enemyPos.y += ENEMY_HEIGHT / 2;

            // Find the center of the ship
            Vector2f shipCenter = getPosition();
            shipCenter.x += SHIP_RADIUS;
            shipCenter.y += SHIP_RADIUS;

			// Apply a force to "bounce" the player off the enemy
            Vector2f collisionForce = shipCenter - enemyPos;
			collisionForce *= (float).2;
			_vel += collisionForce;

            // Apply an opposite force to the enemy
            collisionForce *= (float)-1;
            world._enemies[enemy]->_vel += collisionForce;

            // Flash red when damaged
            setColor(Color{244, 66, 66, 200});
            _bleed = 10;

			// Check if the damage killed the player
            if (_health <= 0)
                _playerIsDead=true;
        }
    }

    // Look through all the powerups
    for(int p = (int)world._powerups.size() - 1; p >= 0; --p)
    {
        // If collision with one
        if (checkIntersect(*world._powerups[p]))
        {
            // If life up
            if(world._powerups[p]->getType() == LIFE_UP)
            {
                // Add to health
                ++_health;

                // Incrase maximum HP if new health is larger
                if(_health > _maxHP)
                {
                    _maxHP = _health;
                }
            }

            // If reload up
            if(world._powerups[p]->getType() == RELOAD_UP)
            {
                // Decrease reload time until minimum is reached
                if(_laserReloadTime >= 3)
                {
                    _laserReloadTime -= 1;
                }

                // Also increase the photon fire rate
                if(_photonReloadTime >= 15)
                {
                    _photonReloadTime -= 13;
                }
            }

            // At a certain point activate the double laser
            if(_laserReloadTime < 18)
            {
                _hasDoubleLaser = true;
            }

            delete world._powerups[p];
            world._powerups.erase(world._powerups.begin() + p);
        }
    }

    // Check if an enemy bullet hits the player
    // !!!NTF: Separate out the player bullets and the enemy bullets into separate arrays
    for(int b = world._bullets.size() - 1; !_inInvincibleFrame && b >= 0; --b)
	{
        if(world._bullets[b]->_source == ENEMY)
		{
            // If the bullets hits
            if(checkIntersect(*world._bullets[b]))
			{
				// Remove a life
                _health--;

				// Start invincibility frame
				if (!_inInvincibleFrame)
				{
                    _inInvincibleFrame = true;
                    _clock.restart();
                }

				// Flash red when damaged
                setColor(Color{244, 66, 66, 200});
                _bleed = 5;

				// Remove the bullet
                delete world._bullets[b];
                world._bullets.erase(world._bullets.begin() + b);

                // Check if the damage kills the player
                if (_health <= 0)
				{
                    _playerIsDead=true;
                }
            }
        }
    }

    // Move the players ship, changes texture based on velocity
    move(_vel);

    if(_vel.x < -SWITCH_THRESHHOLD)
    {
		setTextureRect(sf::IntRect(70,0,43,99));
    }
    else if (_vel.x > SWITCH_THRESHHOLD)
    {
		setTextureRect(sf::IntRect(120,0,43,99));
    }
    else
    {
		setTextureRect(sf::IntRect(0,5,57,98));
    }

    // Set the _health bar
    float percentHP = _health / _maxHP;
    _hpBar.setCurrentHealthBar(percentHP);

	// Set the laser reload bar
    float percentLaserReload = (float)_laserReloadCounter / _laserReloadTime;

    if(percentLaserReload >= 1)
        percentLaserReload = 1.0F;

    _laserReloadBar.setCurrVertical(percentLaserReload);

	// Set the photon reload bar
    float percentPhotonReload = (float)_photonReloadCounter / _photonReloadTime;

    if(percentPhotonReload >= 1)
        percentPhotonReload = 1.0F;

    _photonReloadBar.setCurrVertical(percentPhotonReload);
}


