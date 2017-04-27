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
#include <vector>
using std::vector;
#include <string>
using std::string;

//For random enemy stuff
std::random_device Enemy::ranDev;
std::mt19937 Enemy::rng = std::mt19937(ranDev());

//Used for randomly assigning a starting width
std::uniform_real_distribution<float> Enemy::rngTargetWidth(ENEMY_WIDTH, WIDTH - ENEMY_WIDTH);
//Range which the random value is picked for the followers
std::uniform_real_distribution<float> Enemy::rngFollowerHeight(HEIGHT / 5, HEIGHT * (2 / 5.0));
std::uniform_int_distribution<int> Enemy::randomInt(-2000000, 2000000);

//Ctor
Enemy::Enemy(Vector2f &startingPos,
             Vector2f &startingVel,
             int health,
             int dam,
             Traits &enemyTraits,
             string textureFilePath) :  EnemyShape(),
                                        _vel(startingVel),
                                        _accel(Vector2f(0.0F,0.0F)),
                                        _traits(enemyTraits),
                                        _hp(health),
                                        _damage(dam),
                                        _sourceID(ENEMY)
{
    //Applies a random starting velocity to the enemies
    Vector2f rngStartingDir(World::enemyStartingVel(World::rng), World::enemyStartingVel(World::rng));
    _vel = rngStartingDir;

    //Sets all the texture stuff
    load_texture(_enemyTexture, textureFilePath);
    setTexture(_enemyTexture);
    setScale(0.2F,0.2F);
    setPosition(startingPos);

    //Sets the radius that other enemies are detected at
    _enemyDetectionRadius = ENEMY_HEIGHT + ENEMY_WIDTH / 2;

    // Make bullet detection larger then enemy detection
    _bulletDetectionRadius = _enemyDetectionRadius * 1.5F;
    _desiredPlayerDist = World::optimalPlayerDist(World::rng);

    _dodgeChargeTime = DODGE_TIME;
    _dodgeCounter = 0;
    //Sets the init target that the enemy will seek, for followers this is a diff function
    _target = Vector2f(rngTargetWidth(rng), rngFollowerHeight(rng));

    _bleed = 0;
}

bool Enemy::checkIntersect(const Bullet &b) {
    return (getGlobalBounds().intersects(b.getGlobalBounds()));
}
/*                     VECTOR UTILITY FUNCTIONS              */
//Gets the distance between two position vectors
static float distance(const Vector2f & a, const Vector2f & b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
//Change the x and y components of a vector by different scalar amounts
static void makeCenter(Vector2f & pos, float xOff, float yOff) {
    pos.x += xOff;
    pos.y += yOff;
}
//Normalize a vector
static void normalize(Vector2f & vec) {
    vec /= (float)sqrt(vec.y * vec.y + vec.x * vec.x);
}
//Set the magnitude of a vector while retaining its direction
static void setMag(Vector2f & vec, float newMag) {
    normalize(vec);
    vec *= (float)newMag;
}

//Apply a medium force to keep the enemies from grouping up
Vector2f Enemy::separate(const vector<Enemy*> & enemies)
{
    Vector2f steer{0,0};

    //Find the center of the current enemy
    Vector2f currEnemyPos = getPosition();
    makeCenter(currEnemyPos, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

    //Look through all the enemies

    for(int e = 0; e < (int)enemies.size(); ++e) {
        Vector2f pos = enemies[e]->getPosition();
        //Find the center of the enemy
        makeCenter(pos, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

        float dist = distance(currEnemyPos, pos);
        //check if the enemy can see the other enemy
        if(dist < _enemyDetectionRadius) {
            Vector2f desired = currEnemyPos - pos;
            steer += desired - _vel;
            //Make the force a repulsion force
        }
    }

    return steer;
}

//Add a force moving to move the enemy away from any bullet within a certain dist
Vector2f Enemy::dodge(const vector<Bullet *> & _bullets, bool & hasForce) {
    Vector2f boostForce{0,0};

    //look through all the bullets
    Vector2f enemyPos = getPosition();
    makeCenter(enemyPos, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);

    for(int i = 0; i < (int)_bullets.size(); ++i) {
        //if the bullet is a player bullet
        if(_bullets[i]->_source == PLAYER) {
            //Find the center of that bullet
            Vector2f bulletPos = _bullets[i]->getPosition();
            makeCenter(bulletPos, BULLET_WIDTH, BULLET_SPEED);
            //find the distance between the bullet and the enemy
            float dist = distance(bulletPos, enemyPos);
            //Sees the bullet
            if(dist < _bulletDetectionRadius) {
                //Boost left or right based on where the bullet is respectively
                boostForce = (enemyPos.x > bulletPos.x) ? Vector2f(20, 0) : Vector2f(-20, 0);
                hasForce = true;
                break;
            }
        }
    }

    return boostForce;
}

Vector2f Enemy::seekPlayer(const Ship & playerShip) {
    //Find the center of the enemy
    Vector2f enemyCenter = getPosition();
    //Find the center
    makeCenter(enemyCenter, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);
    //Find the center of the player
    Vector2f playerCenter = playerShip.getPosition();
    makeCenter(playerCenter, SHIP_RADIUS / 2, SHIP_RADIUS / 2);
    //A vector pointing from the enemy to the player
    Vector2f desired =  playerCenter - enemyCenter;
    //Calculate the steering force taking into account the current vel
    Vector2f seek = desired - _vel;

    return seek;
}
//Calculate the seeking force
Vector2f Enemy::seekTarget() {
    //Find the center of the enemy
    Vector2f enemyCenter = getPosition();
    //Find the center
    makeCenter(enemyCenter, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2);
    //A vector pointing from the enemy to the target
    Vector2f desired =  _target - enemyCenter;
    //Calculate the seek force based on that vector
    Vector2f seek = desired - _vel;

    return seek;

}

/* Computes all the logic of the enemies. update is responsible for the
    -movement, pathing decisions
    -Hit detection
    -Animation
*/
void Enemy::update(World & world){
    //Gets the position of the enemyif(hasDodgeForce)
        Vector2f pos = getPosition();

        //Have enemies periodically shoot
        if(_traits._hasGun && World::randomInt(World::rng) % 300 == 0){
            //Make a bullet shooting down
            Color EnemyBulletColor{3, 198, 0};
            //set the bullets direction to go down
            Vector2f dir{0, ENEMY_BULLET_SPEED};
            //Add the new bullet to the world
            world._bullets.push_back(new Bullet(ENEMY,
                                               pos.x, pos.y,
                                               dir,
                                               EnemyBulletColor));
        }
        //Randomly assign new target
        if(!(randomInt(rng) % _traits._targetSwitchChance)) {
            _target = _traits.setTarget(world._playerShip);
        }

        //Only if the enemy has dodge
        if(_traits._bulletDodgeForce){
            //Reset back to the default texture
            if(_dodgeCounter == (_dodgeChargeTime -DODGE_ANIMATION_TIME))
                setTextureRect(sf::IntRect(4, 3, 50, 50));
            if(_dodgeCounter <= 0) {
                //Default texture rect
                bool hasForce = false;
                Vector2f bulletDodge = dodge(world._bullets, hasForce);

                if(hasForce) {
                    _accel += bulletDodge;
                    _dodgeCounter = _dodgeChargeTime;

                    if(_accel.x > 0) //moving to the right
                        setTextureRect(sf::IntRect(70,4,49,39));
                    if(_accel.x < 0) //moving to the left
                        setTextureRect(sf::IntRect(129,5,50,35));
                    }
            }
            else{
                _dodgeCounter--;
            }
        }

        //Calculate the separation force on all the enemies
        Vector2f enemySeparation = separate(world._enemies);
        //Add weights to the separation force for balance
        setMag(enemySeparation, _traits._separateForce);

        //Only add target steer if that enemy has it
        if(_traits._seekTargetForce) {
            Vector2f targetSteer = seekTarget();
            setMag(targetSteer, _traits._seekTargetForce);
            _accel += targetSteer;
        }
        //Only seek the player if that enemy has it
        if(_traits._seekPlayerForce) {
            //If the seeker passed the player
            if(getPosition().y <= world._playerShip.getPosition().y) {
                Vector2f playerSteer = seekPlayer(world._playerShip);
                setMag(playerSteer, _traits._seekPlayerForce);
                _accel += playerSteer;
            }
            //
            else
                _vel.y += 0.5F;

        }
        //Add the separation force to the total acceleration
        _accel += enemySeparation;

		 //Add the total acceleration to the velocity
        _vel += _accel;

        //Dampening of the velocity
        _vel *= 0.99F;

        float velocityMag = sqrt(_vel.y * _vel.y + _vel.x * _vel.x);
        //Going faster then the max speed
        if(velocityMag >= _traits._maxSpeed) {
            //Normalize
            normalize(_vel);

            //Scale to the max speed
            _vel *= (float)_traits._maxSpeed;
        }

        //Zero out the acceleration
        _accel *= 0.0F;

        //Move the enemy
        move(_vel);

        //Look through all the bullets
        for (int b = world._bullets.size() - 1; b >= 0; --b) {
            //If an enemy and a bullet intersect
            if (checkIntersect(*world._bullets[b]) && world._bullets[b]->_source == PLAYER) {
                //Do damage to the enemy
                _hp -= world._bullets[b]->_damage;

                //Have the enemy flash red if hit
                setColor(Color{244, 66, 66, 200});
                _bleed = 10;

                //Remove the bullet
                delete world._bullets[b];
                world._bullets.erase(world._bullets.begin()+b);
                //Check if the enemy is dead
                if(_hp <= 0) return;
                //erases the enemy outside the function
            }

        }
        // ntf: this needs a refactor
        for (int p = world._photons.size() - 1; p >= 0; --p) {
            //If an enemy and a bullet intersect
            if (getGlobalBounds().intersects(world._photons[p]->_hitBox.getGlobalBounds()) ) {
                //Do damage to the enemy
                _hp -= world._photons[p]->_damage;

                //Remove the bullet
                delete world._photons[p];
                world._photons.erase(world._photons.begin()+p);
                //Check if enemy is dead
                if(_hp <= 0) return;
                //erases the enemy outside the function
            }
        }
        //If the enemy is done bleeding set it back to normal
        if(_bleed <= 0) {
            setColor(Color::White);
        } //Keep bleeding
        else
            --_bleed;
}
//Creates a new seeker enemy
Enemy * make_seeker() {
    //Traits of the seeker
    Traits seekerTraits(SEEKER_PLAYERSEEKER_FORCE,   //Player seek force
                        0,                           //Target seek force
                        4,                           //Target switch chance
                        0,                           //Bullet Dodge force
                        SEEKER_SPEED,                //Enemy Speed
                        false);                      //Has a gun



    //Dummy function
    std::function<Vector2f(const ShipShape &)> targetSetter = [](const ShipShape &)-> Vector2f{return Vector2f(0,0);};
    //Add the dummy function
    seekerTraits.setTarget = targetSetter;

    Vector2f initPos{Enemy::rngTargetWidth(Enemy::rng), 0};
    Vector2f initVel{0,0};
    //Make the seeker with the traits
    Enemy * seeker = new Enemy(initPos, initVel, 1, 0, seekerTraits, "resources/sprites/homingEnemySheet.png");
    //Texture settings for the sprite
    seeker->setScale(0.48, 0.48);
    seeker->setTextureRect(sf::IntRect(0,0,81,87));

    return seeker;
}
//Creates  a new wanderer enemy, wanderers break up into a set number of seekers when they die
//wanderers have guns and periodically get their position reassigned
Enemy * make_wanderer() {
    //Applies all the settings of that particular enemy
    Traits wandererTraits(0,                        //Player seek force
                          WANDERER_TARGET_FORCE,    //Target seek force
                          WANDERER_TARGET_SWITCH,   //Target switch chance
                          0,                        //Bullet Dodge force
                          WANDERER_MAXSPEED,        //Enemy Speed
                          true);                    //Has a gun

    //Function for assigning target, ship is a variable for compatibility
    std::function<Vector2f (const ShipShape &)> targetSetter = [](const ShipShape & ship)->Vector2f{return Vector2f(Enemy::rngTargetWidth(Enemy::rng),
                                                                                                                    Enemy::rngFollowerHeight(Enemy::rng));};
    //Add the function to the traits
    wandererTraits.setTarget = targetSetter;
    //Set the init pos of the enemy at the top of the screen
    Vector2f initPos{Enemy::rngTargetWidth(Enemy::rng), 0};
    Vector2f initVel{0,0};
    //The hp of the enemy
    int hp = 6;

    //Make the new enemies with the traits created
    Enemy * wanderer = new Enemy(initPos, initVel, hp, 1, wandererTraits);


    wanderer->setScale(0.41, 0.41);

    return wanderer;
}
//Create a new follower, followers have guns and get reassigned a point but instead of getting a
//random x position, they get the players x position and a random y
Enemy * make_follower() {
    Traits followerTraits(0,                        //Player seek force
                          FOLLOWER_TARGET_FORCE,    //Target seek force
                          FOLLOWER_TARGET_SWITCH,   //Target switch chance
                          FOLLOWER_BULLET_DODGE,    //Bullet Dodge force
                          FOLLOWER_MAXSPEED,        //Enemy Speed
                          true);                    //Has a gun

    //Returns a vector2f of a new target
    std::function<Vector2f (const ShipShape &)> targetSetter = [](const ShipShape & ship)->Vector2f{return Vector2f(ship.getPosition().x,
                                                                                                                     Enemy::rngFollowerHeight(Enemy::rng));};
    //Add the function to the traits
    followerTraits.setTarget = targetSetter;

    Vector2f initPos{Enemy::rngTargetWidth(Enemy::rng), 0};
    Vector2f initVel{0,0};
    int hp = 5;
    //Make the enemy with the traits
    Enemy * follower = new Enemy(initPos, initVel, hp, 1, followerTraits, "resources/sprites/dodgeEnemySheet.png");

    follower->setScale(1, 1);
    follower->setTextureRect(sf::IntRect(4, 3, 50, 50));

    return follower;
}


