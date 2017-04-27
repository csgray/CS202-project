/*
main.cpp
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 9/4/2017
Last Updated: 26/4/2017
main.cpp contains the game RenderWindow object, as well as stores a vector of pointers to Screens objects (pure virtual).
New screens are declared in main and stored the the Screens vector by reference; additional screens can be implemented in
main by adding the appropriate header file include, creating a new screen object (must inherit from Screens.h) and storing
it in the screens vector.
The runtime loop manages screens by setting the screenSelect to a value return by each screen's run function.
*/

#include "constants.h"
#include "Screens.h"
#include "Screen_0.h"
#include "World.h"
#include "Photon.h"

#include <SFML/Graphics.hpp>
using sf::Event;
using sf::Color;
#include <vector>
using std::vector;

int main(){
    //vector of screens, holds all game screens
    std::vector<Screens*> screens;
    //manages which screen to run; -1 indicates close status
    int screenSelect=0;

    //create game window
    sf::RenderWindow App(sf::VideoMode((unsigned)WIDTH, (unsigned)HEIGHT), "SPACE GAME");
    App.setFramerateLimit(FRAMERATE);

    //hide mouse cursor
    App.setMouseCursorVisible(false);

    //Make main menu
    MenuScreen ms;
    screens.push_back(&ms);
    //Make game world
    World gw;
    screens.push_back(&gw);

    //main game loop
    while(screenSelect>=0){
        screenSelect = screens[screenSelect]->Run(App);
    }
    //return value set, should not happen
    return 0;
}
