//asteroids.cpp
//CS 202 Final Proj
//Auth: William Horn
//Rough draft for game framework

#include "constants.h"
#include "World.h"
#include "Photon.h"

#include <SFML/Graphics.hpp>
using sf::Event;
using sf::Color;

int main(){

    while(true){
        World world;
        //Runs for as long as the window is open
        while(world.isOpen()){
            //Gets all the events since the last iteration
            Event event;
            ///////Event loop/////////
            while(world.pollEvent(event)){
                //Act appropriately for different events

                    if(event.type == sf::Event::Closed){
                        world.close();
                    }

                    //if(event.type == sf::Event::KeyPressed){}

            }////END EVENT LOOP////
            /* clear/draw/display cycle */
            //Clear needs to be called before stuff can be drawn
            world.clear(Color::Black);
            world.update();
            world.show();
            world.display();
        }//END WINDOW LOOP//
        break;
    }
    return 0;
}
