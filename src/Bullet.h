// Bullet.h
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170412
// Updated: 20170425
//
// Header for Bullet class

#ifndef BULLET_H
#define BULLET_H

#include "constants.h"

#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Color;



struct Bullet : public BulletShape{
    Bullet(int source,
           float x,
           float y,
           Vector2f &dir,
           Color & col = defaultBulletColor);

    static Color defaultBulletColor;
    int _source;
    int _damage;
    Vector2f _direction;
};

#endif // BULLET_H
