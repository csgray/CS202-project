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
typedef sf::RectangleShape EnemyShape;
typedef sf::CircleShape ShipShape;

const float PI = 3.14159265358979323846F;  /* pi */

/* Game Constants */
const float WIDTH = 500.0F;
const int HEIGHT = 750;
const int FRAMERATE = 60;
const float BACKGROUND_SPEED = 7.0F;

const float PLAYER_X_ACCEL = 0.7F;
const float PLAYER_Y_ACCEL = 0.5F;
const float SHIP_RADIUS = 20.0F;
const float SHIP_DRAG = 0.93F;
const float MAX_SPEED = 6.0F;

const int BULLET_DAMAGE = 1;
const int BULLET_LEN = 15;
const int BULLET_WIDTH = 3;
const int BULLET_SPEED = 15;
const int DEFAULT_BULLET_FIRERATE = 20;   //Smaller number is faster fire rate

const float PHOTON_RADIUS = 30;
const int PHOTON_EDGES = 20;
const int PHOTON_SPEED = 3;
const int DEFAULT_PHOTON_FIRERATE = 75;
const float PHOTON_FREQUENCY = PI / 30;
const int PHOTON_DAMAGE = 1000000;

const int STAR_SPAWN_RATE = 1; //Larger means less stars
const float STAR_HEIGHT = 2.0F;
const float STAR_WIDTH = 2.0F;

const int ENEMY_HEIGHT = 30;
const int ENEMY_WIDTH = 20;
const float ENEMY_BULLET_SPEED = 5.0F;
const float ENEMY_MAX_SPEED = 2.7F;
const int DODGE_TIME = 100;


enum ID {PLAYER, ENEMY};
//For bound checking
enum bounds {RIGHT, LEFT, UPPER, LOWER,EMPTY};

#endif // GAME_CONSTANTS_H_INCLUDED
