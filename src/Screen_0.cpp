/*Screen_0.cpp
Team Members: William Horn, Corey Gray, Michael Bilan, Cameron Titus, Kyle Tam, Andrew Cummins
Created: 17/4/2017
Updated: 17/4/2017
Screen_0 is the game menu screen.*/

#include "Screen_0.h"
#include "Loader.h"
#include "constants.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using std::string;

//GameMenu Constants
bool drawMenu = true;
bool playing = false;
sf::CircleShape shape;
sf::CircleShape shape2;

string labelFilePath = "resources/sprites/LABELS.png";
sf::Texture labelTexture;
sf::Sprite startLabel;
sf::Sprite continueLabel;
sf::Sprite exitLabel;



//Initial Menu Setup
void initialDraw(sf::RenderWindow &gameMenu, bool drawMenu, bool playing){
    load_texture(labelTexture, labelFilePath);

    startLabel.setTexture(labelTexture);
    startLabel.setTextureRect(sf::IntRect(160,0, 114,63));
    startLabel.setOrigin(160 / 2. - 23, 63 / 2.0);
    startLabel.setPosition(250,200);
    //startLabel.setScale(1.5F, 1.5);

    continueLabel.setTexture(labelTexture);
    continueLabel.setTextureRect(sf::IntRect(454, 0, 147, 63));
    continueLabel.setOrigin(147 / 2., 63 / 2.);
    continueLabel.setPosition(250,200);
    //continueLabel.setScale(1.5F, 1.5);

    exitLabel.setTexture(labelTexture);
    exitLabel.setTextureRect(sf::IntRect(370, 0, 74, 63));
    exitLabel.setOrigin(74 / 2.0, 63 / 2.0);
    exitLabel.setPosition(250, 500);
    //exitLabel.setScale(1.5F, 1.5);

    shape.setRadius(70);
    shape.setOrigin(shape.getRadius(),shape.getRadius());
    shape.setPosition(250, 200);
    shape.setFillColor(sf::Color::Green);

    shape2.setRadius(70);
    shape2.setOrigin(shape2.getRadius(),shape2.getRadius());
    shape2.setPosition(250,500);
    shape2.setFillColor(sf::Color::Red);

    gameMenu.clear();
    (playing) ? gameMenu.draw(continueLabel) : gameMenu.draw(startLabel);
    gameMenu.draw(exitLabel);
    gameMenu.display();
    drawMenu = false;
}

int MenuScreen::Run(sf::RenderWindow &gameMenu){
    sf::Event event;
    int menuSelect=0;

    //checks if game flag is true; if it is, changes menu option to Continue game
    if(playing){
        shape.setFillColor(sf::Color::Red);
        gameMenu.draw(shape);
    }

    if(drawMenu) initialDraw(gameMenu,drawMenu, playing);

    while(true){
        //check SFML events
        while(gameMenu.pollEvent(event)){
            //check if window has been closed
            if (event.type == sf::Event::Closed) return -1;
            //check for key presses
            if(event.type == sf::Event::KeyPressed){
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
            //update menu text colors based on selection
            const int EXIT_SELECTED = 1;
            const int START_SELECTED = 0;

            if(menuSelect==START_SELECTED){
                (playing) ? continueLabel.setColor(sf::Color::Red) : startLabel.setColor(sf::Color::Red);
                exitLabel.setColor(sf::Color::White);
            }
            else if (menuSelect==EXIT_SELECTED){
                exitLabel.setColor(sf::Color::Red);
                (playing) ? continueLabel.setColor(sf::Color::White) : startLabel.setColor(sf::Color::White);

            }
            else{
                (playing) ? continueLabel.setColor(sf::Color::White) : startLabel.setColor(sf::Color::White);
                exitLabel.setColor(sf::Color::White);
            }

            //clears the screen
            gameMenu.clear();

            //draw text menu - WON'T WORK UNTIL FONT IMPORTS WORK
            /*gameMenu.draw(menu1);
            gameMenu.draw(menu2);*/

            //display the screen
            (playing) ? gameMenu.draw(continueLabel) : gameMenu.draw(startLabel);
            gameMenu.draw(exitLabel);
            gameMenu.display();
        }
    }
};
