//Bullet.cpp
//Auth: William Horn
//4.12.2017
//Definitions for Bullet class

#include "../Bullet.h"

#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Color;

Bullet::Bullet(int source,
               float bulletX,
               float bulletY,
               Vector2f dir,
               Color col) : BulletShape(Vector2f(BULLET_WIDTH,BULLET_SPEED)),
                            source(source),
                            damage(BULLET_DAMAGE),
                            direction(dir)
{
    setPosition(bulletX, bulletY);
    setFillColor(col);

}
