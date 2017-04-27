//Kyle Tam
//Healthbar.cpp
// 4-22-17
//Last Rev 4-22-17

#include "healthbar.h"
#include "constants.h"
#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Color;

HealthBar::HealthBar( float l,
                      float w ) : _maxHealthBar(HPBarShape(Vector2f(l,w))),
                                  _currentHealthBar(HPBarShape(Vector2f(l, w)))
{
    _maxHealthBar.setFillColor(Color{0,0,0,0});
    _maxHealthBar.setOutlineColor({255,255,255,200});
    _maxHealthBar.setOutlineThickness(3);

    _currentHealthBar.setFillColor(Color{255, 66, 66,230});

    _currentHealthBar.setPosition(WIDTH - l - HP_BAR_SCREEN_BUFFER, HEIGHT - w -HP_BAR_SCREEN_BUFFER);
    _maxHealthBar.setPosition(WIDTH - l - HP_BAR_SCREEN_BUFFER, HEIGHT - w - HP_BAR_SCREEN_BUFFER);

}
HealthBar::~HealthBar(){}

void HealthBar::setCurrentHealthBar(float hpPercentage) {
    //Scales the current hp bar to the percentage given
    _currentHealthBar.setScale(hpPercentage, 1.0);
}

void HealthBar::setCurrVertical(float hpPercentage) {
    _currentHealthBar.setScale(1.0, hpPercentage);
}
