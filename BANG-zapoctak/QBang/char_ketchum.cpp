#include "game.h"

void Ketchum::discard_phase()
{
    //nema smysl vyhazovat karty
    if (max_health == health || cards_hand.size() < 2)
	{
		Player::discard_phase();
	}
	else
	{
        //vyhazujeme karty od nejmene podstatnych, coz rozhoduji zivoty hrace
        while (cards_hand.size() > 1 && max_health > health)
		{
            for(int i = 0; i < 2; i++)
            {
                if (health > max_health / 2)
                {
                    bool result = (discard_card("neu") ? true : false);
                    result = (result ? true : discard_card("def"));
                    result = (result ? true : discard_blue());
                    result = (result ? true : discard_card("agr"));
                }
                else
                {
                    bool result = (discard_card("neu") ? true : false);
                    result = (result ? true : discard_blue());
                    result = (result ? true : discard_card("agr"));
                    result = (result ? true : discard_card("def"));
                }
            }
            health++;
		}

        if(cards_hand.size() > health)
        {
            Player::discard_phase();
        }
    }
}

void Ketchum::draw_phase()
{
    discarded = 0;
    Player::draw_phase();
}

void Ketchum::ability()
{
    if(discarded >= 2 && health < max_health && !g->game_order[g->active_player]->isai && g->mode == "")
    {
        discarded -= 2;
        health++;
    }
}
