// loader.h
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 201704??
// Updated: 20170426
//
// Header for loader functions

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

bool load_texture(sf::Texture& tex,const std::string& filename);

bool load_buffer(sf::SoundBuffer& soundBuffer, const std::string& filename);

#endif
