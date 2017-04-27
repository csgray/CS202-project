/*
Screen_0.cpp
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 17/4/2017
Updated: 26/4/2017
Screen_0 manages the menu screen.
*/

#include "Screen_0.h"
#include "Loader.h"
#include "constants.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using std::string;

//GameMenu Constants
bool drawMenu = true; //manages initial menu draw
bool playing = false;
sf::CircleShape playButton;
sf::CircleShape exitButton;

string labelFilePath = "resources/sprites/LABELS.png";
sf::Texture newGameLabelTexture;
sf::Sprite newGameLabel;



//Initial Menu Setup
void initialDraw(sf::RenderWindow &gameMenu, bool drawMenu){
    load_texture(newGameLabelTexture, labelFilePath);
    newGameLabel.setTexture(newGameLabelTexture);
    newGameLabel.setTextureRect(sf::IntRect(0,0, 160, 63));
    newGameLabel.setOrigin(160 / 2, 63 / 2.0);
    newGameLabel.setPosition(250,200);

    playButton.setRadius(50);
    playButton.setOrigin(playButton.getRadius(),playButton.getRadius());
    playButton.setPosition(250, 200);
    playButton.setFillColor(sf::Color::Green);

    exitButton.setRadius(50);
    exitButton.setOrigin(exitButton.getRadius(),exitButton.getRadius());
    exitButton.setPosition(250,500);
    exitButton.setFillColor(sf::Color::Red);

    gameMenu.clear();
    gameMenu.draw(playButton);
    gameMenu.draw(exitButton);
    gameMenu.draw(newGameLabel);
    gameMenu.display();


    drawMenu = false;
}

int MenuScreen::Run(sf::RenderWindow &gameMenu){
    sf::Event event;

    //undefined selection causes menu to have no pre-select when returning to the screen
    int menuSelect=-1;

    //checks if game flag is true; if it is, changes menu option to Continue game
    if(playing){
        playButton.setFillColor(sf::Color::Red);
        gameMenu.draw(playButton);
    }

    //checks runtime flag to see if the screen has already been drawn
    if(drawMenu) initialDraw(gameMenu,drawMenu);

    //draw loop
    while(true){
        //check SFML events
        while(gameMenu.pollEvent(event)){
            //check if window has been closed
            if (event.type == sf::Event::Closed) return -1;
            //check for key presses
            if(event.type == sf::Event::KeyPressed){
                //switch on specific key presses or default to generic key press
                switch (event.key.code){
                    case sf::Keyboard::Up:
                        menuSelect=0;
                        break;
                    case sf::Keyboard::Down:
                        menuSelect=1;
                        drawMenu = true;
                        break;
                    case sf::Keyboard::Return:
                        if (menuSelect==0){
                            playing=true;
                            return 1; //starts game
                        }
                        else if (menuSelect==1) return -1;
                        break;
                    case sf::Keyboard::Escape:
                            return -1;
                    default:
                        menuSelect=-1; //all other keys clear selection
                        break;
                }
            }
            //Use the joystick to check the input
            int detectionThreshold = 98;
            if(sf::Joystick::isConnected(0)){
                //If the joystick is pressed halfway up just select
                if(sf::Joystick::getAxisPosition(0,sf::Joystick::Y) <= -detectionThreshold / 2) {
                    menuSelect = 0;
                    //If the right trigger is pushed

                }
                if (sf::Joystick::isButtonPressed(0, A) && menuSelect == 0){
                            playing=true;
                            return 1; //starts game
                    }
                if(sf::Joystick::getAxisPosition(0,sf::Joystick::Y) >= detectionThreshold / 2) {
                    menuSelect = 1;
                }
                if (sf::Joystick::isButtonPressed(0, A) && menuSelect == 1) {
                        return -1;
                }

            }

            //update menu text colors based on selection, and playing/not playing
            if(menuSelect==0){
                playButton.setFillColor(sf::Color::Blue);
                exitButton.setFillColor(sf::Color::Red);
            }
            else if (menuSelect==1){
                if(playing)
                    playButton.setFillColor(sf::Color::Cyan);
                else
                    playButton.setFillColor(sf::Color::Green);
                    exitButton.setFillColor(sf::Color::Blue);
            }
            //if no menu option selected, display defaults
            else{
                if(playing)
                    playButton.setFillColor(sf::Color::Cyan);
                else
                    playButton.setFillColor(sf::Color::Green);
                    exitButton.setFillColor(sf::Color::Red);
            }

            //clears the screen
            gameMenu.clear();

            //draw text menu - WON'T WORK UNTIL FONT IMPORTS WORK
            /*gameMenu.draw(menu1);
            gameMenu.draw(menu2);*/

            //display the screen
            gameMenu.draw(playButton);
            gameMenu.draw(exitButton);
            gameMenu.draw(newGameLabel);
            gameMenu.display();
        }
    }
};
