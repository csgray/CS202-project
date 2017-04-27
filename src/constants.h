// constants.h
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170407
// Updated: 20170425
//
// Constants related to all parts of the game

#ifndef GAME_CONSTANTS_H_INCLUDED
#define GAME_CONSTANTS_H_INCLUDED

#include <SFML/Graphics.hpp>

/* Easily change shapes later if needed */
typedef sf::CircleShape StarShape;
typedef sf::RectangleShape BulletShape;
typedef sf::Sprite EnemyShape;
typedef sf::Sprite ShipShape;
typedef sf::RectangleShape HPBarShape;

// Pi
const float PI = 3.14159265358979323846F;

// Window settings
const float WIDTH = 500.0F;
const float HEIGHT = 750.0F;
const unsigned FRAMERATE = 60; // Set to 0 to disable framerate limit

// Scrolling background speed (used for moving stars)
const float BACKGROUND_SPEED = 7.0F;

// Player movement and location
const float PLAYER_X_ACCEL = 1.0F;
const float PLAYER_Y_ACCEL = 1.0F;
const float SHIP_RADIUS = 20.0F;
const float SHIP_DRAG = 0.92F;
const float MAX_SPEED = 8.0F;
const float STARTING_HP = 25.0F;
const float SWITCH_THRESHHOLD = 6.5F;

// Health bar dimensions
const float HP_BAR_LEN = 160.0F;
const float HP_BAR_WIDTH = 15.0F;
const float HP_BAR_SCREEN_BUFFER = 10.0F;

// Bullets
const int BULLET_DAMAGE = 1;
const int BULLET_LEN = 15;
const float BULLET_WIDTH = 4.0F;
const float BULLET_SPEED = 23.0F;
const int DEFAULT_BULLET_FIRERATE = 20;   //Smaller number is faster fire rate

// Photons
const float PHOTON_RADIUS = 30.0F;
const int PHOTON_EDGES = 20;
const int PHOTON_SPEED = 2;
const int DEFAULT_PHOTON_FIRERATE = 200;
const float PHOTON_FREQUENCY = PI / 35.0F;
const int PHOTON_DAMAGE = 10;

// Reload bar dimensions (for both bullets and photons)
const float RELOAD_BAR_WIDTH = 160.0F;
const float RELOAD_BAR_HEIGHT = 8.0F;

// Background stars
const int STAR_SPAWN_RATE = 1; //Larger means less stars
const float STAR_HEIGHT = 1.5F;
const float STAR_WIDTH = 2.0F;

// Enemy dimensions for collision detection
const float ENEMY_HEIGHT = 30.0F;
const float ENEMY_WIDTH = 20.0F;

// Enemy bullets
const float ENEMY_BULLET_SPEED = 5.0F;
const float ENEMY_MAX_SPEED = 3.0F;

// Prevents enemies from bunching up
const float ENEMY_SEPARATE_FORCE = 0.16F;

// Enemy dodge parameters
const int DODGE_TIME = 75;
const int DODGE_ANIMATION_TIME = 20;


const float SEEKER_PLAYERSEEKER_FORCE = 0.5F;
const float SEEKER_SPEED = 8.0F;

// Wanderers (dodge shots and randomly move)
const float WANDERER_TARGET_FORCE = 0.17F;
const float WANDERER_MAXSPEED = 3.0F;
const int WANDERER_TARGET_SWITCH = 120;

// Followers (lines up with players and shoots)
const float FOLLOWER_BULLET_DODGE = 8.0F;
const float FOLLOWER_MAXSPEED = 4.5F;
const float FOLLOWER_TARGET_FORCE = 0.3F;
const int FOLLOWER_TARGET_SWITCH = 40;

// Bullet _source values
enum ID {PLAYER, ENEMY};

//For bound checking
enum bounds {RIGHT, LEFT, UPPER, LOWER,EMPTY};

enum controllerButtons {A, B, X, Y, LB, RB, SELECT, START, LTHUMB, RTHUMB, RT};

#endif // GAME_CONSTANTS_H_INCLUDED
