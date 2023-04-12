#include "game.h"
#include "ai.h"

void Pedro::draw_phase()
{
    //pokud zbyva posledni hrac mimo nas tak ho pridame na seznam nepratel
    if(g->game_order.size() == 2)
    {
        enemies_id.insert(g->game_order[(g->active_player + 1) % 2]->id);
    }
    //Kdyby nahodou si tam pomocnik serifa dal serifa
    if(role == 'V' && enemies_id.find(g->game_order[0]->id) != enemies_id.end())
    {
        enemies_id.erase(enemies_id.find(g->game_order[0]->id));
    }

    ability_used = true;
    drawed = true;

    //zjistujeme jestli nas zajima dana karta nebo jestli nam nepomuze ze situace
    if(g->deck.back().name == "Dostavnik" || g->deck.back().name == "WellsFargo")
    {
        cards_hand.push_back(g->draw_from_deck());
        cards_hand.push_back(g->deck.back());
        g->deck.pop_back();
        return;
    }
    else if((health > max_health / 2 && g->deck.back().card_type == AGR) ||
            (health <= max_health / 2 && g->deck.back().card_type == DEF))
    {
        cards_hand.push_back(g->draw_from_deck());
        cards_hand.push_back(g->deck.back());
        g->deck.pop_back();
        return;
    }
    else
    {
        Player::draw_phase();
    }
}

void Pedro::ability()
{
    if(Ai::no_pedro_abil(g, id))
    {
        return;
    }
    cards_hand.push_back(g->draw_from_deck());
    cards_hand.push_back(g->deck.back());
    g->deck.pop_back();
    ability_used = true;
    drawed = true;
}
