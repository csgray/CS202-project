/*
Enemy.h
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 20/4/2017
Last Updated: 26/4/2017
<brief text description of file; feel free to update these>
*/

#ifndef ENEMY_H
#define ENEMY_H

#include "constants.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Ship.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <random>
#include <string>
#include <functional>

struct Traits
{
    Traits(float playerSeek,
           float targetSeek,
           int switchChance,
           float bulletDodge,
           float speed,
           bool gun,
           float separate = ENEMY_SEPARATE_FORCE) : _seekPlayerForce(playerSeek),
                                                    _seekTargetForce(targetSeek),
                                                    _targetSwitchChance(switchChance),
                                                    _bulletDodgeForce(bulletDodge),
                                                    _maxSpeed(speed),
                                                    _hasGun(gun),
                                                    _separateForce(separate){}

    float _seekPlayerForce;
    float _seekTargetForce;
    int _targetSwitchChance;
    float _bulletDodgeForce;
    float _maxSpeed;
    bool _hasGun;
    float _separateForce;

    // A variable function for setting the target
    // 2 possible types:
    //   -random x, random y
    //   -player x, random y
    std::function<Vector2f(const ShipShape & ship)> setTarget;
};

class World;

struct Enemy : public EnemyShape
{
    Enemy(sf::Vector2f &startingPos,
          sf::Vector2f &startingVel,
          int hp,
		  int damage,
          Traits &enemyTraits,
          std::string textureFilePath = "resources/sprites/eyeball.png");

    Vector2f _vel;
    Vector2f _accel;
    float _maxSpeed;
    sf::Texture _enemyTexture;
	Traits _traits;

    // Used to periodically dodge bullets
    Vector2f dodge(const std::vector<Bullet *> &bullets, bool &hasForce);
    int _dodgeChargeTime;
    int _dodgeCounter;

    // Push away from other enemies
    Vector2f separate(const std::vector<Enemy*> &enemies);

	// Pull in the direction of the player
    Vector2f seekPlayer(const Ship &playerShip);

	// Seek a random target
    Vector2f seekTarget();
    Vector2f _target;

    float _enemyDetectionRadius;
    float _bulletDetectionRadius;
    float _desiredPlayerDist;

    int _hp;
    int _damage;
    int _sourceID;

    int _bleed;

    bool checkIntersect(const Bullet &b);
    void update(World &world);

    static std::random_device ranDev;
    static std::mt19937 rng;

    static std::uniform_real_distribution<float> rngTargetWidth;
    static std::uniform_real_distribution<float> rngTargetHeight;
    static std::uniform_real_distribution<float> rngFollowerHeight;
    static std::uniform_int_distribution<int> randomInt;
};

// Factory functions for making the different types of enemies
Enemy * make_seeker();
Enemy * make_wanderer();
Enemy * make_follower();

#endif // ENEMY_H
