//healthbar.h
//Kyle Tam
//4-20-17
//Last rev 4-22-17

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


