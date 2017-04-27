/*
Photon.cpp
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 12/4/2017
Last Updated: 26/4/2017

*/


#include "constants.h"
#include "Photon.h"

#include <SFML/Graphics.hpp>
using sf::TriangleFan;
using sf::Triangles;
using sf::TriangleStrip;
using sf::VertexArray;
using sf::Vertex;
using sf::Vector2;
using sf::Color;
using sf::CircleShape;
using sf::FloatRect;
#include <cmath>
using std::cos;
using std::sin;

int hitBoxDiff = 25;

//Ctor
Photon::Photon(float radius, unsigned int numVertices) : _vertices(VertexArray(TriangleFan, numVertices)),
                                                         _hitBox(CircleShape(radius - hitBoxDiff, numVertices)),
                                                         _damage(PHOTON_DAMAGE),
                                                         _moveCounter(0)
{
    //Center of the circle
    _vertices[0].position = Vector2<float>(0,0);
    _vertices[0].color = Color(200,200,255);

    for(int v = 1; v < (int)numVertices - 1; ++v){
        _vertices[v].position = Vector2<float>(cos((2 * PI * v) / (numVertices - 1)) * radius,
                                              sin((2 * PI * v)/ (numVertices - 1)) * radius);
        _vertices[v].color =  Color(0, 0, 255, 0);
    }

    _vertices[numVertices - 1].position = _vertices[1].position;
    _vertices[numVertices - 1].color = Color(0, 0, 255, 0);

    this->setOrigin(-radius + hitBoxDiff, -radius + hitBoxDiff);
    _hitBox.setFillColor(Color(255,255,255,255));

};

void Photon::setPhotonPosition(float x, float y){
    this->setPosition(x,y);
    _hitBox.setPosition(x,y);
}

void Photon::movePhoton(float delX, float delY){
    this->move(delX, delY);
    _hitBox.move(delX, delY);
}

FloatRect Photon::getGlobalBounds(){
    return _hitBox.getGlobalBounds();
}

//So the custom object can be drawn on the screen
