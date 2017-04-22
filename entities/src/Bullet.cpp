//Bullet.cpp
//Auth: William Horn
//4.12.2017
//Definitions for Bullet class

#include "../../constants.h"
#include "../Bullet.h"

#include <SFML/Graphics.hpp>
using sf::Vector2;
using sf::Color;

Bullet::Bullet(int source, float bulletX, float bulletY, Vector2<float> dir) : BulletShape(Vector2<float>(BULLET_WIDTH,BULLET_SPEED)),
                                                                               damage(BULLET_DAMAGE),
                                                                               source(source),
                                                                               direction(dir)
{
    Color BULLET_COLOR{255, 0, 0};

    this->setPosition(bulletX, bulletY);
    this->setFillColor(BULLET_COLOR);

}
