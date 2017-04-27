/*
healthbar.h
CS 202 Final Project
Team Members: Michael Bilan, Andrew Cummins, Corey Gray, William Horn, Kyle Tam, Cameron Titus
Created: 20/4/2017
Last Updated: 26/4/2017
<brief text description of file; feel free to update these>
*/

#include "constants.h"

#ifndef HEALTHBAR_H
#define HEALTHBAR_H

class HealthBar {

public:
    HealthBar(float length = HP_BAR_LEN, float width = HP_BAR_WIDTH);
    virtual  ~HealthBar();

    void setCurrentHealthBar(float hpPercentage);
    void setCurrVertical(float hpPercentage);

    HPBarShape _maxHealthBar;
    HPBarShape _currentHealthBar;

};

typedef HealthBar ReloadBar;

#endif // HEALTHBAR_H


