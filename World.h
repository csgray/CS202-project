// world.h
// CS 202 Project: Scrolling Space Shooter
// Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
// Created: 20170409
// Updated: 20170412
//
// Header for World class

#ifndef WORLD_H
#define WORLD_H

#include "constants.h"
#include "entities/Enemy.h"
#include "entities/Bullet.h"
#include "entities/Photon.h"
#include "entities/Ship.h"
#include "Screens.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class World : public Screens{
    public:

        /* Player Ship Object */
        Ship playerShip;

        /* Main Containers for game entities*/
        std::vector<Bullet> bullets;
        std::vector<Photon> photons;
        std::vector<StarShape> stars;
        std::vector<Enemy> enemies;

        void updatePhotons();
        void addPhoton(Photon p);

        /* Utility Functions for bullets */

        void updateBullets();
        void addBullet(Bullet b);

        // Utility functions for stars
        void makeStar(float startingHeight);
        void populateInitialStars();
        void updateStars();

        /* Utility Functions for player ship */

        void updateShip();
        std::vector<bounds> onBound(const Ship & playerShip);

        void makeInitEnemies();
        void updateEnemies();

        void show(sf::RenderWindow &);
        void update();
        Ship getPlayerShip();

        // Random Real Generators
        static std::random_device ranDev;
        static std::mt19937 rng;

        static std::uniform_real_distribution<float> starDist;
        static std::uniform_real_distribution<float> enemyStartingVel;
        static std::uniform_real_distribution<float> optimalPlayerDist;

        static std::uniform_int_distribution<int> starBrightness;
        static std::uniform_int_distribution<int> randomInt;

        virtual int Run(sf::RenderWindow &) override;
};

#endif // WORLD_H
