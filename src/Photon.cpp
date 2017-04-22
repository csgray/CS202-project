//Asteroid.cpp
//Auth: William Horn
//4.12.2017
//Definitions for Asteroid class

#include "../../constants.h"
#include "../Photon.h"

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
Photon::Photon(float radius, unsigned int numVertices) : vertices(VertexArray(TriangleFan, numVertices)),
                                                         hitBox(CircleShape(radius - hitBoxDiff, numVertices)),
                                                         damage(PHOTON_DAMAGE),
                                                         moveCounter(0)
{
    //Center of the circle
    vertices[0].position = Vector2<float>(0,0);
    vertices[0].color = Color(200,200,255);

    for(int v = 1; v < (int)numVertices - 1; ++v){
        vertices[v].position = Vector2<float>(cos((2 * PI * v) / (numVertices - 1)) * radius,
                                              sin((2 * PI * v)/ (numVertices - 1)) * radius);
        vertices[v].color =  Color(0, 0, 255, 0);
    }

    vertices[numVertices - 1].position = vertices[1].position;
    vertices[numVertices - 1].color = Color(0, 0, 255, 0);

    this->setOrigin(-radius + hitBoxDiff, -radius + hitBoxDiff);
    hitBox.setFillColor(Color(255,255,255,255));

};

void Photon::setPhotonPosition(float x, float y){
    this->setPosition(x,y);
    hitBox.setPosition(x,y);
}

void Photon::movePhoton(float delX, float delY){
    this->move(delX, delY);
    hitBox.move(delX, delY);
}

FloatRect Photon::getGlobalBounds(){
    return hitBox.getGlobalBounds();
}

//So the custom object can be drawn on the screen
