//game_constants.h
//CS 202 Proj
//Auth: William Horn
//4.7.2017

/*
    Constants related to all parts of the game
*/

#include <SFML/Graphics.hpp>

#ifndef GAME_CONSTANTS_H_INCLUDED
#define GAME_CONSTANTS_H_INCLUDED


/* Easily change shapes later if needed */
typedef sf::RectangleShape StarShape;
typedef sf::RectangleShape BulletShape;
typedef sf::RectangleShape HPBarShape;
typedef sf::Sprite EnemyShape;
typedef sf::Sprite ShipShape;


const double PI = 3.14159265358979323846;  /* pi */

/* Game Constants */
const int WIDTH = 500;
const int HEIGHT = 750;
const int FRAMERATE = 60;
const int BACKGROUND_SPEED = 7;

const float PLAYER_X_ACCEL = .7;
const float PLAYER_Y_ACCEL = .5;
const int SHIP_RADIUS = 20;
const float SHIP_DRAG = 0.93;
const float MAX_SPEED = 6;

const float BULLET_DAMAGE = .1;
const int BULLET_LEN = 15;
const int BULLET_WIDTH = 3;
const int BULLET_SPEED = 15;
const int DEFAULT_BULLET_FIRERATE = 20;   //Smaller number is faster fire rate

const int PHOTON_RADIUS = 30;
const int PHOTON_EDGES = 20;
const int PHOTON_SPEED = 3;
const int DEFAULT_PHOTON_FIRERATE = 75;
const float PHOTON_FREQUENCY = PI / 30;
const int PHOTON_DAMAGE = 1000000;

const int STAR_SPAWN_RATE = 1; //Larger means less stars
const int STAR_HEIGHT = 2;
const int STAR_WIDTH = 2;

const int ENEMY_HEIGHT = 30;
const int ENEMY_WIDTH = 20;
const float ENEMY_BULLET_SPEED = 5;
const float ENEMY_MAX_SPEED = 2.7;
const int DODGE_TIME = 100;
const float DAMAGE_MODIFIER = 1.0;


enum ID {PLAYER, ENEMY};
//For bound checking
enum bounds {RIGHT, LEFT, UPPER, LOWER,EMPTY};

#endif // GAME_CONSTANTS_H_INCLUDED
