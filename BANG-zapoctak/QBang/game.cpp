#include <algorithm>
#include <fstream>
#include <string>

#include "game.h"

using namespace std;

void Game::load_characters()
{
	Hrac pl = make_unique<Bart>(this);
	characters.push_back(move(pl));
	pl = make_unique<Blackj>(this);
	characters.push_back(move(pl));
	pl = make_unique<Calamity>(this);
	characters.push_back(move(pl));
	pl = make_unique<Carlson>(this);
	characters.push_back(move(pl));
	pl = make_unique<Cringo>(this);
	characters.push_back(move(pl));
	pl = make_unique<Jesse>(this);
	characters.push_back(move(pl));
    pl = make_unique<Jourd>(this);
    characters.push_back(move(pl));
    pl = make_unique<Ketchum>(this);
    characters.push_back(move(pl));
    pl = make_unique<Lucky>(this);
    characters.push_back(move(pl));
	pl = make_unique<Paul>(this);
	characters.push_back(move(pl));
	pl = make_unique<Pedro>(this);
	characters.push_back(move(pl));
	pl = make_unique<Rose>(this);
	characters.push_back(move(pl));
	pl = make_unique<Slab>(this);
	characters.push_back(move(pl));
	pl = make_unique<Suzy>(this);
	characters.push_back(move(pl));
	pl = make_unique<Vulture>(this);
	characters.push_back(move(pl));
	pl = make_unique<Willy>(this);
	characters.push_back(move(pl));

	auto rd = random_device{};
	auto rng = default_random_engine{ rd() };
	for (size_t i = 0; i < 20; i++)
	{
		shuffle(begin(characters), end(characters), rng);//pro nahodny vyber postav
	}
}
void Game::load_card(vector<string>& v)
{
    Card card(stoi(v[4]), v[0], v[3][0], v[1], stoi(v[2]), v[5]);

    deck.push_back(card);

	auto rd = random_device{};
	auto rng = default_random_engine{ rd() };
    shuffle(begin(deck), end(deck), rng);//pro nahodny vyber karet
}
Card Game::draw_from_deck()
{
	Card c;
	c = deck.front();
	deck.pop_front();
	return c;
}
void Game::create(int players, string roles)
{
    player_alive = player_count = players;
    create_players(players - 1);

    int num = (characters[players]->ranking > characters[players + 1]->ranking ? players + 1 : players);

    Hrac p = move(characters[num]);
    characters.erase(characters.begin() + num);
    p->isai = false;
    game_order.push_back(move(p));

    auto rd = random_device{};
    auto rng = default_random_engine{ rd() };
    for (int i = 0; i < 20; i++)
    {
        shuffle(begin(roles), end(roles), rng);//pro nahodny vyber roli
    }

    for (int i = 0; i < players; i++)
    {
        game_order[i]->set_role(roles[i]);
    }

}
void Game::create_players(int count)
{
    for (size_t i = 0; i < (size_t)count; i++)
	{
		if (characters[i]->ranking < characters[i + 1]->ranking)
		{
			Hrac p = move(characters[i]);
			characters.erase(characters.begin() + i);
			game_order.push_back(move(p));
		}
		else
		{
			Hrac p = move(characters[i + 1]);
			characters.erase(characters.begin() + i + 1);
			game_order.push_back(move(p));
		}
	}
}
void Game::rotate_serif()
{
	while (game_order[0]->say_role() != 'S')
	{
		rotate(game_order.begin(), game_order.begin() + 1, game_order.end());
	}

    active_player = 0;
}
void Game::draw_cards_start()
{
	Card c;
	for (size_t i = 0; i < game_order.size(); i++)
	{
        for (size_t j = 0; j < (size_t)game_order[i]->max_health; j++)
		{
			c = draw_from_deck();
			game_order[i]->take_card(c);
		}	
	}
}
bool Game::finished()
{
	if (player_alive == 1)
	{
		return true;
	}
    else if(game_order[0]->say_role() != 'S')
    {
        return true;
    }
    else if(game_order[0]->health == 0)
    {
        return true;
    }
    else
    {
        for(size_t i = 0; i < game_order.size(); i++)
        {
            if(game_order[i]->role != 'S' && game_order[i]->role != 'V')
            {
                return false;
            }
        }
        return true;
    }
}
void Game::set_initial_enemies()
{
	int sheriff_id = 0;
	vector<int> ids;

	for (size_t i = 0; i < game_order.size(); i++)
	{
		if (game_order[i]->say_role() == 'S')
		{
			sheriff_id = game_order[i]->id;
		}
		else
		{
			ids.push_back(game_order[i]->id);
		}
	}

	for (size_t i = 0; i < game_order.size(); i++)
	{
		game_order[i]->set_enemy(sheriff_id, ids);
	}
}
void Game::set_distances()
{
    //ZAKLADNI GRAF
	for (size_t i = 0; i < game_order.size(); i++)
	{
		map<int, int> m;
		for (size_t j = 0; j < game_order.size() / 2; j++)
		{		
            m[game_order[(i + j + 1) % player_alive]->id] = static_cast<int>(j) + 1;//forward
            m[game_order[(i - j + player_alive - 1) % player_alive]->id] = static_cast<int>(j) + 1;//backwards
		}

		distances[game_order[i]->id] = m;
	}

    //SCHOPNOSTI POSTAV
	for (size_t i = 0; i < game_order.size(); i++)
	{
        if (game_order[i]->name == "rose")
		{
			change_distance(game_order[i]->id, -1);
		}
        else if (game_order[i]->name == "paul")
		{
			int paul_id = game_order[i]->id;

			for (auto&& pl : distances)
			{
				change_distance(pl.first, 1, paul_id);
			}
		}
	}

    //ZBRANE
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->has_gun() != -1)
        {
            change_distance(game_order[i]->id, -(game_order[i]->has_gun() - 1));
        }
    }

    //KONE
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->index(game_order[i]->cards_desk, "Appaloosa") != -1)
        {
            change_distance(game_order[i]->id, -1);
        }
    }
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->index(game_order[i]->cards_desk, "Mustang") != -1)
        {
            int id = game_order[i]->id;

            for (auto&& pl : distances)
            {
                change_distance(pl.first, 1, id);
            }
        }
    }
}
void Game::change_distance(int id1, int change, int id2)//zmena hrany v orientovanem ohodnocenem grafu
{
	if (id1 == id2)
	{
		return;
	}

	if (id2 == -1)
	{
		for (auto&& pl : distances.find(id1)->second)
		{
			pl.second += change;
		}
	}
	else 
	{
		distances.find(id1)->second[id2] += change;
	}
}
void Game::add_labels(QVector<QList<QLabel*>>& layout)
{
    size_t count = game_order.size();
    for(; notai < count; notai++)
    {
        if(!game_order[notai]->isai)
        {
            break;
        }
    }
    game_order[notai]->char_l = layout[6][0];
    game_order[notai]->hp_l = layout[6][1];
    game_order[notai]->role_l = layout[6][2];
    for(int i = 3; i < 9; i++)
    {
        game_order[notai]->m_l.append(layout[6][i]);
    }
    for(int i = 9; i < 19; i++)
    {
        game_order[notai]->cards_l.append(layout[6][i]);
    }

    for(size_t i = 1; i <count; i++)
    {
        add_label_ai((notai - i + count) % count, layout[i - 1]);
    }
}
void Game::add_label_ai(int ai, QList<QLabel*>& labels)
{
    game_order[ai]->char_l = labels[0];
    game_order[ai]->hp_l = labels[1];
    game_order[ai]->card_l = labels[2];
    game_order[ai]->count_l = labels[3];
    game_order[ai]->role_l = labels[4];
    for(int i = 5; i < 11; i++)
    {
        game_order[ai]->m_l.append(labels[i]);
    }
}
void Game::saloon()
{
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->max_health > game_order[i]->health)
        {
            game_order[i]->health++;
        }
    }
}
int Game::game_loop()
{
    if(finished())
    {
        return 404;
    }


    if(mode == "")
    {
        if(!game_order[active_player]->drawed)
        {
            game_order[active_player]->drawed = true;
            if(game_order[active_player]->resolve_dyn())
            {
                if(!game_order[active_player]->dec_hp(3))//neprezil to
                {
                    vector<Card> reward = game_order[active_player]->give_all_cards();
                    vulture_sam(reward);
                    int del = active_player;
                    rm_enemy(game_order[active_player]->id);
                    active_player = (active_player + 1) % player_alive;
                    game_order.erase(game_order.begin() + del);
                    player_alive--;
                    set_distances();
                    return 3;
                }
            }
            if(!game_order[active_player]->resolve_jail())
            {
                active_player = (active_player + 1) % player_alive;
                return 3;
            }
            game_order[active_player]->draw_phase();
            return 3;
        }
        else
        {
            int game = game_order[active_player]->game_phase();
            if(game == 0)
            {
                game_order[active_player]->discard_phase();
                active_player = (active_player + 1) % player_alive;
            }
            else if(game == 1)
            {
                mode = deck.back().name;
            }
            else
            {
                return 3;
            }
        }
    }
    else
    {
        resolve_played_card();
    }

    return 3;
}
int Game::name_to_id(const QString& name)
{
    string n = name.toStdString();
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(n == game_order[i]->name)
        {
            return game_order[i]->id;
        }
    }
    return -1;
}
QString Game::id_to_name(int id)
{
    if(id == -1)
    {
        return "";
    }
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->id == id)
        {
            return QString::fromStdString(game_order[i]->name);
        }
    }
    return "";
}
void Game::vulture_sam(std::vector<Card>& reward)
{
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->name == "vulture")
        {
            for(size_t j = 0; j < reward.size(); j++)
            {
                game_order[i]->cards_hand.push_back(reward[j]);
            }
        }
        return;
    }

    for(size_t j = 0; j < reward.size(); j++)
    {
        deck.push_back(reward[j]);
    }
}
void Game::resolve_played_card()
{
    if(mode == "Kulomet")
    {
        if(neu_turn == -1)
        {
            neu_turn = (active_player + 1) % player_alive;
        }
        bool vedle = game_order[neu_turn]->play_vedle();
        if(!vedle)
        {
            bool hp = game_order[neu_turn]->dec_hp(1);
            if(!hp)
            {
                vector<Card> reward = game_order[neu_turn]->give_all_cards();
                vulture_sam(reward);
                rm_enemy(game_order[neu_turn]->id);
                game_order.erase(game_order.begin() + neu_turn);
                player_alive--;
                set_distances();
            }
        }

        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }
    }
    else if(mode == "Indiani")
    {
        if(neu_turn == -1)
        {
            neu_turn = (active_player + 1) % player_alive;
        }
        bool bang = game_order[neu_turn]->play_bang();
        if(!bang)
        {
            bool hp = game_order[neu_turn]->dec_hp(1);
            if(!hp)
            {
                vector<Card> reward = game_order[neu_turn]->give_all_cards();
                vulture_sam(reward);
                rm_enemy(game_order[neu_turn]->id);
                game_order.erase(game_order.begin() + neu_turn);
                player_alive--;
                set_distances();
            }
        }

        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }
    }
    else if(mode == "Hokynarstvi")
    {
        if(neu_turn == -1)
        {
            neu_turn = active_player;
            load_emporio();
        }
        int choice = game_order[neu_turn]->choose(emporio);
        game_order[neu_turn]->cards_hand.push_back(emporio[choice]);
        emporio.erase(emporio.begin() + choice);

        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }
    }
    else if(mode == "Duel")
    {
        bool result;
        if(duel_active_turn)
        {
            result = game_order[active_player]->play_bang();

            if(!result)
            {
                bool hp = game_order[active_player]->dec_hp(1);
                if(!hp)
                {
                    active_player = (active_player + 1) % player_alive;
                    game_order[active_player]->drawed = false;
                    game_order[active_player]->played_bang = false;

                    vector<Card> reward = game_order[neu_turn]->give_all_cards();
                    vulture_sam(reward);
                    rm_enemy(game_order[active_player]->id);
                    game_order.erase(game_order.begin() + active_player);
                    player_alive--;
                    set_distances();
                }
                mode = "";
            }
        }
        else
        {
            int enemy_id = game_order[active_player]->target_id;
            int pos = id_to_pos(enemy_id);
            result = game_order[pos]->play_bang();

            if(!result)
            {
                bool hp = game_order[pos]->dec_hp(1);
                if(!hp)
                {
                    vector<Card> reward = game_order[pos]->give_all_cards();
                    vulture_sam(reward);
                    rm_enemy(game_order[pos]->id);
                    game_order.erase(game_order.begin() + pos);
                    player_alive--;
                    set_distances();
                }
                mode = "";
            }
        }
        duel_active_turn = !duel_active_turn;
    }
    else if(mode == "Bang")
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = id_to_pos(enemy_id);
        bool vedle = game_order[pos]->play_vedle();
        if(!vedle)
        {
            bool hp = game_order[pos]->dec_hp(1);
            if(!hp)
            {
                vector<Card> reward = game_order[pos]->give_all_cards();
                vulture_sam(reward);
                rm_enemy(enemy_id);
                game_order.erase(game_order.begin() + pos);
                player_alive--;
                set_distances();
            }
        }
        mode = "";
    }
    else if(mode == "CatBalou")
    {
        int pos = id_to_pos(game_order[active_player]->target_id);
        Card c = game_order[pos]->give_random_card();
        deck.push_back(c);
        mode = "";
    }
    else if(mode == "Panika")
    {
        int enemy_id = game_order[active_player]->target_id;

        if(game_order[active_player]->can_play_panika(enemy_id))
        {
            int pos = id_to_pos(enemy_id);
            Card c = game_order[pos]->give_random_card();
            game_order[active_player]->cards_hand.push_back(c);
            mode = "";
        }

        mode = "";
    }

    error = "resolve_played_card";
}
int Game::id_to_pos(int id)
{
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->id == id)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}
void Game::after_kill(int player_id, int dead_id)
{

}

void Game::load_emporio()
{
    for(size_t i = 0; i < game_order.size(); i++)
    {
        emporio.push_back(draw_from_deck());
    }
}
void Game::rm_enemy(int id)
{
    for(size_t i = 0; i <game_order.size(); i++)
    {
        if(game_order[i]->enemies_id.find(id) != game_order[i]->enemies_id.end())
        {
            game_order[i]->enemies_id.erase(id);
        }
    }
}
