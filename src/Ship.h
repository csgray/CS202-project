// Ship.h
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170412
// Updated: 20170426
//
// Header for ship struct

#ifndef SHIP_H
#define SHIP_H

#include "constants.h"
#include "Photon.h"
#include "Bullet.h"
#include "healthbar.h"

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class World;

struct Ship : public ShipShape
{
	// Constructor
	Ship();

	// Data members
	sf::Vector2f _vel;
	sf::Vector2f _accel;
	sf::Texture _shipTexture;
	sf::SoundBuffer _photonBuffer;
    sf::SoundBuffer _laserBuffer;
    sf::SoundBuffer _explosionBuffer;
    sf::Sound _photonSound;
    sf::Sound _laserSound;
    sf::Sound _explosionSound;
	sf::Clock _clock;
	sf::Time _time;

    // Weapons
    Photon* photonCannon(); //Creates a photon at the ships position
    Bullet* laser(); //Creates a bullet at the ships position

    // Weapon reload bars
	ReloadBar _photonReloadBar;
	ReloadBar _laserReloadBar;

    // Reload values
	// !!!NTF: Put these in some sort of struct along with the weapon functions
    int _laserReloadTime;
    int _laserReloadSpeed;
    int _laserReloadCounter;
    int _photonReloadTime;
    int _photonReloadSpeed;
    int _photonReloadCounter;
    int _playerScore;
    bool _hasDoubleLaser;

    //Health
    float _health;
    float _maxHP;
    bool _playerIsDead;
    bool _isTouchingEnemy;
    bool _inInvincibleFrame;
    HealthBar _hpBar;

    // Checks if an enemy collides with the ship
    bool checkIntersect(const sf::Shape &e);
    bool checkIntersect(const sf::Sprite &e);

    int _bleed;

	// To id the bullets
    int _sourceID;

    // Respond to keyboard inputs, controls movement and weapons
    // Modifies the world
    void update(World & world);
};

#endif // SHIP_H
