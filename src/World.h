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
#include "Enemy.h"
#include "Bullet.h"
#include "Photon.h"
#include "Ship.h"
#include "Screens.h"
#include "powerup.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>


class World : public Screens {
    public:
		// Constructor and destructor
		World();
		~World();

        // Player Ship Object
        Ship _playerShip;
        sf::Clock _clock;
        sf::SoundBuffer _bgBuffer;
        sf::Sound _bgSound;
        sf::SoundBuffer _gameOverBuffer;
        sf::Sound _gameOverSound;

        // Main Containers for game entities
        std::vector<Bullet*> _bullets;
        std::vector<Photon*> _photons;
        std::vector<StarShape*> _stars;
        std::vector<Enemy*> _enemies;
        std::vector<Powerup*> _powerups;

		// Utility functions for photons
        void updatePhotons();
        void addPhoton(Photon &p);

        // Utility functions for bullets
        void updateBullets();
        void addBullet(Bullet &b);

        // Utility functions for stars
        void makeStar(int startingHeight);
        void populateInitialStars();
        void updateStars();

        // Utility Functions for player ship
        std::vector<bounds> onBound(const Ship & playerShip);

		// Utility functions for enemies
        void makeInitEnemies();
        void updateEnemies();

		// Display functions
        void show(sf::RenderWindow &gameScreen);
        void update();
        int Run(sf::RenderWindow &gameScreen) override;

        // Random number generators
        static std::random_device ranDev;
        static std::mt19937 rng;

        static std::uniform_real_distribution<float> starDist;
        static std::uniform_real_distribution<float> enemyStartingVel;
        static std::uniform_real_distribution<float> optimalPlayerDist;

        static std::uniform_real_distribution<float> randomWidht;
        static std::uniform_real_distribution<float> randomHeight;

        static std::uniform_int_distribution<int> starBrightness;
        static std::uniform_int_distribution<int> randomInt;
        static std::uniform_int_distribution<int> randomSplitNumber;
};

#endif // WORLD_H
