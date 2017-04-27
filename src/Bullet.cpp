// Bullet.cpp
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170412
// Updated: 20170425
//
// Definitions for Bullet class

#include "Bullet.h"

#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Color;

Color Bullet::defaultBulletColor{255, 0, 0};

Bullet::Bullet(int source,
               float bulletX,
               float bulletY,
               Vector2f & dir,
               Color & col) : BulletShape(Vector2f(BULLET_WIDTH,BULLET_SPEED)),
                            _source(source),
                            _damage(BULLET_DAMAGE),
                            _direction(dir)
{
    setPosition(bulletX, bulletY);
    setFillColor(col);
}
