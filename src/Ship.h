//Ship.h
//Auth : William Horn
//4.12.2017
//Header for ship class

#ifndef SHIP_H
#define SHIP_H

#include "constants.h"
#include "Photon.h"
#include "Bullet.h"
#include "healthbar.h"

#include <vector>

#include <SFML/Graphics.hpp>

class World;

struct Ship : public ShipShape{
    //constructor
    Ship();

    sf::Vector2f _vel;
    sf::Vector2f _accel;
    sf::Texture _shipTexture;
    sf::Texture _explosionTexture;
    sf::Clock _clock;
    sf::Time _time;


    /* weapons */
    //Creates a photon at the ships position
    Photon* photonCannon();
    //Creates a bullet at the ships position
    Bullet* laser();

    /* reload values */ // !!!NTF: Put these in some sort of struct
                        //         Along with the weapon functions
    int _laserReloadTime;
    int _laserReloadSpeed;
    int _laserReloadCounter;
    int _photonReloadTime;
    int _photonReloadSpeed;
    int _photonReloadCounter;
    int _playerScore;

    ReloadBar _photonReloadBar;
    ReloadBar _laserReloadBar;

    /* health */
    float _health;
    float _maxHP;
    bool _playerIsDead;
    bool _isTouchingEnemy;
    bool _inInvincibleFrame;
    HealthBar _hpBar;

    //Checks if an enemy collides with the ship
    bool checkIntersect(const sf::Shape &e);
    bool checkIntersect(const sf::Sprite &e);

    int _bleed;
    //To id the bullets
    int _sourceID;

    //Respond to keyboard inputs, controls movement and weapons
    //Modifies the world
    void update(World & world);

};

#endif // SHIP_H
