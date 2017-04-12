//Enemy.cpp
//Auth: William Horn
//4.10.2017
//Definitions for Enemy class

#include "Enemy.h"

#include <SFML/Graphics.hpp>
using sf::Vector2;
using sf::Color;

//Ctor
Enemy::Enemy(Vector2<float> starting_pos,
             Vector2<float> starting_dir,
             int health,
             int d) :  EnemyShape(Vector2<float>(ENEMY_HEIGHT,ENEMY_WIDTH)),
                       direction(starting_dir),
                       hp(health),
                       damage(d)
{
    this->setPosition(starting_pos);
    this->setFillColor(Color(255, 76, 240));

}

bool Enemy::checkIntersect(const BulletShape &b) {
    return (this->getGlobalBounds().intersects(b.getGlobalBounds()));
}
