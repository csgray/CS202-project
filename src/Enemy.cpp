//Enemy.cpp
//Auth: William Horn
//4.10.2017
//Definitions for Enemy class

#include "Enemy.h"
#include "World.h"
#include "Loader.h"

#include <SFML/Graphics.hpp>
using sf::Vector2f;
using sf::Color;
#include <random>

std::random_device Enemy::ranDev;
std::mt19937 Enemy::rng = std::mt19937(ranDev());

std::uniform_real_distribution<float> Enemy::rngTargetWidth(ENEMY_WIDTH, WIDTH - ENEMY_WIDTH);
std::uniform_real_distribution<float> Enemy::rngTargetHeight(ENEMY_WIDTH, (float)HEIGHT * (3./5.));
std::uniform_int_distribution<int> Enemy::randomInt(-2000000, 2000000);

//Ctor
Enemy::Enemy(Vector2f starting_pos,
             Vector2f starting_vel,
             int health,
             int d) :  EnemyShape(),
                       vel(starting_vel),
                       accel(Vector2f(0,0)),
                       hp(health),
                       damage(d),
                       sourceID(ENEMY)
{
    Vector2f rngStartingDir(World::enemyStartingVel(World::rng), World::enemyStartingVel(World::rng));
    vel = rngStartingDir;

    load_texture(enemyTexture ,"resources/sprites/eyeball.png");
    setTexture(enemyTexture);
    setScale(.25,.25);
    setPosition(starting_pos);

    enemyDetectionRadius = ENEMY_HEIGHT + ENEMY_WIDTH / 2;
    //Make bullet detection larger then enemy detection
    bulletDetectionRadius = enemyDetectionRadius;
    desiredPlayerDist = World::optimalPlayerDist(World::rng);

    dodgeChargeTime = DODGE_TIME;
    dodgeCounter = 0;

    target = Vector2f(rngTargetWidth(rng), rngTargetHeight(rng));
    targetSwitchChance = 50;
}

bool Enemy::checkIntersect(const Bullet &b) {
    return (getGlobalBounds().intersects(b.getGlobalBounds()));
}

static float distance(const Vector2f & a, const Vector2f & b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

static void scalarMul(Vector2f & vec, float scalar) {
    vec.x *= scalar;
    vec.y *= scalar;
}

static void scalarDiv(Vector2f & vec, float scalar) {
    vec.x /= scalar;
    vec.y /= scalar;
}

static void makeCenter(Vector2f & pos, float xOff, float yOff) {
    pos.x += xOff;
    pos.y += yOff;
}

static void normalize(Vector2f & vec) {
    scalarDiv(vec, sqrt(vec.y * vec.y + vec.x * vec.x));
}

static void setMag(Vector2f & vec, float newMag) {
    normalize(vec);
    scalarMul(vec, newMag);
}

Vector2f Enemy::separate(const vector<Enemy*> & enemies) {
    Vector2f steer{0,0};

    //Find the center of the current enemy
    Vector2f currEnemyPos = getPosition();
    makeCenter(currEnemyPos, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

    //Look through all the enemies
<<<<<<< HEAD:src/Enemy.cpp
    for(int e = 0; e < enemies.size(); ++e) {
        Vector2f pos = enemies[e]->getPosition();
=======
    for(int e = 0; e < (int)enemies.size(); ++e) {
        Vector2f pos = enemies[e].getPosition();
>>>>>>> enemyAI:src/Enemy.cpp
        //Find the center of the enemy
        makeCenter(pos, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

        float dist = distance(currEnemyPos, pos);
        //check if the enemy can see the other enemy
        if(dist < enemyDetectionRadius) {
            Vector2f desired = currEnemyPos - pos;
            steer += desired - vel;
            //Make the force a repulsion force
        }
    }

    return steer;
}

<<<<<<< HEAD:src/Enemy.cpp
Vector2f Enemy::dodge(const vector<Bullet*> & bullets) {
=======
Vector2f Enemy::dodge(const vector<Bullet> & bullets, bool & hasForce) {
>>>>>>> enemyAI:src/Enemy.cpp
    Vector2f boostForce{0,0};

    //look through all the bullets
    Vector2f enemyPos = getPosition();
    makeCenter(enemyPos, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

    for(int i = 0; i < (int)bullets.size(); ++i) {
        //if the bullet is a player bullet
        if(bullets[i]->source == PLAYER) {
            //Find the center of that bullet
            Vector2f bulletPos = bullets[i]->getPosition();
            makeCenter(bulletPos, BULLET_WIDTH, BULLET_SPEED);
            //find the distance between the bullet and the enemy
            float dist = distance(bulletPos, enemyPos);
            //Sees the bullet
            if(dist < bulletDetectionRadius) {
                //Boost left or right based on where the bullet is respectively
                boostForce = (enemyPos.x > bulletPos.x) ? Vector2f(20, 0) : Vector2f(-20, 0);
                hasForce = true;
                break;
            }
        }
    }

    return boostForce;
}

Vector2f Enemy::seek(const ShipShape & playerShip) {
    //Find the center of the enemy
    Vector2f enemyCenter = getPosition();
    //Find the center
    makeCenter(enemyCenter, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

    //Find the center of the player
    Vector2f playerCenter = playerShip.getPosition();
    makeCenter(playerCenter, SHIP_RADIUS / 2, SHIP_RADIUS / 2);

    Vector2f desired =  playerCenter - enemyCenter;

    Vector2f seek = desired - vel;

    return seek;
}

Vector2f Enemy::seekTarget() {
    //Find the center of the enemy
    Vector2f enemyCenter = getPosition();
    //Find the center
    makeCenter(enemyCenter, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

    Vector2f desired =  target - enemyCenter;
    Vector2f seek = desired - vel;

    return seek;

}

Vector2f Enemy::flee() {
    return Vector2f(0,0);
}

void Enemy::update(World & world){
    //Gets the position of the enemyif(hasDodgeForce)
        Vector2f pos = getPosition();

        //Have enemies periodically shoot
        if(World::randomInt(World::rng) % 300 == 0){
            //Make a bullet shooting down
<<<<<<< HEAD:src/Enemy.cpp
            world.bullets.push_back(new Bullet(ENEMY, pos.x, pos.y, Vector2f(0, ENEMY_BULLET_SPEED), Color{247, 168, 255}));
=======
            world.bullets.push_back(Bullet(ENEMY,
                                           pos.x, pos.y,
                                           Vector2f(0, ENEMY_BULLET_SPEED),
                                           Color{247, 168, 255}));
>>>>>>> enemyAI:src/Enemy.cpp
        }
        //Randomly assign new target
        if(!(randomInt(rng) % targetSwitchChance)) {
            //world.playerShip.getPosition().x
            target = Vector2f(rngTargetWidth(rng), rngTargetHeight(rng));
        }

        //  !!!NTF: Bullet dodge doesnt work right

        //
        if(dodgeCounter <= 0) {
            bool hasForce = false;
            Vector2f bulletDodge = dodge(world.bullets, hasForce);

            if(hasForce) {
                accel += bulletDodge;
                dodgeCounter = dodgeChargeTime;
            }
        }
        else{
            dodgeCounter--;
        }


        Vector2f enemySeparation = separate(world.enemies);
        Vector2f targetSteer = seekTarget();

        //Add weights to the separation force for balance

        setMag(enemySeparation, .13);
        setMag(targetSteer, .15);

        //Add the separation force to the total acceleration

        accel += enemySeparation;
        accel += targetSteer;

        //Add the total acceleration to the velocity
        vel += accel;

        //Dampening of the velocity
        scalarMul(vel, .99);

        float velocityMag = sqrt(vel.y * vel.y + vel.x * vel.x);
        //Going faster then the max speed
        if(velocityMag >= ENEMY_MAX_SPEED) {
            //Normalize
            normalize(vel);

            //Scale to the max speed
            scalarMul(vel, ENEMY_MAX_SPEED);
        }

        //Zero out the acceleration
        scalarMul(accel, 0);

        //Move the enemy
        move(vel);

        //Look through all the bullets
        for (int b = world.bullets.size() - 1; b >= 0; --b) {
            //If an enemy and a bullet intersect
            if (checkIntersect(*world.bullets[b]) && world.bullets[b]->source == PLAYER) {
                //Do damage to the enemy
                hp -= world.bullets[b]->damage;


                //Remove the bullet
                delete world.bullets[b];
                world.bullets.erase(world.bullets.begin()+b);
                //Check if the enemy is dead
                if(hp <= 0) return;
            }

        }
        // !!!NTF: THIS IS BAAAADDD...
        for (int p = world.photons.size() - 1; p >= 0; --p) {
            //If an enemy and a bullet intersect
            if (getGlobalBounds().intersects(world.photons[p]->hitBox.getGlobalBounds()) ) {
                //Do damage to the enemy
                hp -= world.photons[p]->damage;




                //Remove the bullet
                delete world.photons[p];
                world.photons.erase(world.photons.begin()+p);
                //Check if enemy is dead
                if(hp <= 0) return;
            }
        }
}
