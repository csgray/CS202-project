//Bullet.h
//Auth: William Horn
//4.12.2017
//Header for Bullet class

#ifndef BULLET_H
#define BULLET_H

#include "../constants.h"

#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Color;

struct Bullet : public BulletShape{

    Bullet(int source,
           float x,
           float y,
           Vector2f dir,
           Color col = Color{255, 0, 0});

    int source;
    int damage;
    Vector2f direction;
};
#endif // BULLET_H
