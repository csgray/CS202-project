//Enemy.h
//Auth: William Horn
//4.20.2017
//Class header to represent base enemy

#ifndef ENEMY_H
#define ENEMY_H

#include "../constants.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Ship.h"

#include <SFML/Graphics.hpp>
using sf::FloatRect;
using sf::Vector2f;
#include <vector>
using std::vector;

class World;

struct Enemy : public EnemyShape{

    Enemy(Vector2f starting_pos, Vector2f starting_vel, int hp, int damage);

    Vector2f vel;
    Vector2f accel;
    float maxSpeed;
    float enemyDetectionRadius;
    float desiredPlayerDist;
    //Push away from other enemies
    Vector2f separate(const vector<Enemy> & enemies);
    //Pull in the direction of the player
    Vector2f seek(const Ship & playerShip);
    //Run away from bullets
    Vector2f flee();

    int hp;
    int damage;
    int sourceID;

    bool checkIntersect(const Bullet &b);
    void update(World & world);
};



#endif // ENEMY_H
