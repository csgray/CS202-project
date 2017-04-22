/* Screens.h
Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
Created: 17/4/2017
Updated: 17/4/2017
Pure virtual class, used to create games screens.*/

#include <SFML/Graphics.hpp>

#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

class Screens{
public:
    virtual int Run(sf::RenderWindow &) = 0;
};

#endif // SCREENS_H_INCLUDED
