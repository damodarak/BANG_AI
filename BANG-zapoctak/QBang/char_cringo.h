#ifndef EL_CRINGO_H
#define EL_CRINGO_H

#include "player.h"

//v resolve action

class Cringo : public Player {
public:
    Cringo(Game* game) : Player(16, 3, "gringo", game) {}

};

#endif
