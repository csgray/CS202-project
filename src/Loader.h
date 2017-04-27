/*
Loader.h
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 20/4/2017
Last Updated: 26/4/2017
Functions for loading text and font files.
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SFML/Graphics.hpp>

bool load_texture(sf::Texture& tex,const std::string& filename);

bool load_font(sf::Font& tex,const std::string& filename);

#endif
