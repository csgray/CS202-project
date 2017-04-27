/*
Screens.h
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 17/4/2017
Last Updated: 26/4/2017
Pure virtual class. All managed screens inherit run from this class and define it's operation.
*/
#include <SFML/Graphics.hpp>

#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

class Screens{
public:
    virtual int Run(sf::RenderWindow &) = 0;
};

#endif // SCREENS_H_INCLUDED
