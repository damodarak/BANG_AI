#ifndef EL_CRINGO_H
#define EL_CRINGO_H

#include "player.h"

//kdykoli dostane ranu vezme si kartu z ruky utocnika

class Cringo : public Player {
public:
    Cringo(Game* game) : Player(15, 3, game) {}
    virtual bool dec_hp(int lifes) override;
};

#endif
