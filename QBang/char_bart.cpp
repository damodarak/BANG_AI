#include "game.h"

//lize kartu kdykoli je zasazen

bool Bart::dec_hp(int lifes)
{
    Player::dec_hp(lifes);
    Card c;
    for (int i = 0; i < lifes; i++)
    {
        c = pd.g->draw_from_deck();
        pd.cards_hand.push_back(c);
    }
    return health > 0;
}
