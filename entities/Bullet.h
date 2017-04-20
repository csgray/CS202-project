//Bullet.h
//Auth: William Horn
//4.12.2017
//Header for Bullet class

#ifndef BULLET_H
#define BULLET_H

#include "../constants.h"

#include <SFML/Graphics.hpp>
using sf::Vector2;

struct Bullet : public BulletShape{
    Bullet(int source, float x, float y, Vector2<float> dir);

    int source;
    int damage;
    Vector2<float> direction;
};
#endif // BULLET_H
