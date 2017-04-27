/*
Bullet.h
CS 202 Final Project: Scrolling Space Shooter
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 12/4/2017
Last Updated: 27/04/2017
Header for Bullet Class
*/

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
