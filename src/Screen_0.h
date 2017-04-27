/*
Screens_0.h
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 17/4/2017
Last Updated: 26/4/2017
Creates a new derivative class to manage the menu screen, inheriting directly from the Screens class.
*/

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
