/*Screen_0.h
Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
Created: 17/4/2017
Updated: 17/4/2017
Screen_0 inherits from the Screens.h virtual class; it is an object that controls the menu screen.*/

#ifndef SCREEN0_H_INCLUDED
#define SCREEN0_H_INCLUDED

#include "Screens.h"
#include <SFML/Graphics.hpp>

class MenuScreen : public Screens{
public:
    virtual int Run(sf::RenderWindow &) override;
};

void initialDraw();

#endif // SCREEN0_H_INCLUDED
