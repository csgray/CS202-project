//Enemy.h
//Auth: William Horn
//4.20.2017
//Class header to represent base enemy

#ifndef ENEMY_H
#define ENEMY_H

#include "../constants.h"
#include "Bullet.h"

#include <SFML/Graphics.hpp>
using sf::FloatRect;
using sf::Vector2;

struct Enemy : public EnemyShape{

    Enemy(Vector2<float> starting_pos, Vector2<float> starting_dir, int hp, int damage);
    Vector2<float> direction;

    int hp;
    int damage;
    int sourceID;
    bool checkIntersect(const Bullet &b);

};



#endif // ENEMY_H
