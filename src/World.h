/*
World.h
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 9/4/2017
Last Updated: 26/4/2017
World manages the game screen, and is a derivative of Screens. Class contains all objects and functions used to manage
the game screen.
*/

#ifndef WORLD_H
#define WORLD_H

#include "constants.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Photon.h"
#include "Ship.h"
#include "Screens.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class World : public Screens {
    public:
        /* Player Ship Object */
        Ship _playerShip;

        /* Main Containers for game entities*/
        std::vector<Bullet*> _bullets;
        std::vector<Photon*> _photons;
        std::vector<StarShape*> _stars;
        std::vector<Enemy*> _enemies;

        void updatePhotons();
        void addPhoton(Photon p);

        /* Utility Functions for bullets */

        void updateBullets();
        void addBullet(Bullet b);

        // Utility functions for stars
        void makeStar(int startingHeight);
        void populateInitialStars();
        void updateStars();

        /* Utility Functions for player ship */

        void updateShip();
        std::vector<bounds> onBound(const Ship & playerShip);

        void makeInitEnemies();
        void updateEnemies();

        World();
        ~World();

        void show(sf::RenderWindow &gameScreen);
        void update();

        int Run(sf::RenderWindow &gameScreen) override;

        // Random Real Generators
        static std::random_device ranDev;
        static std::mt19937 rng;

        static std::uniform_real_distribution<float> starDist;
        static std::uniform_real_distribution<float> enemyStartingVel;
        static std::uniform_real_distribution<float> optimalPlayerDist;

        static std::uniform_int_distribution<int> starBrightness;
        static std::uniform_int_distribution<int> randomInt;

};

#endif // WORLD_H
