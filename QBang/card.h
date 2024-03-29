#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <climits>

class Game;
class PlayerData;

enum Modes {NONE, BANG, VEDLE, PIVO, SALON, WELLSFARGO, DOSTAVNIK,
             HOKYNARSTVI, PANIKA, BALOU, INDIANI, KULOMET, DUEL, DYNAMIT,
             VEZENI, BAREL, MUSTANG, APPALOOSA, VOLCANIC, SCHOFIELD,
             REMINGTON, CARABINE, WINCHESTER, SLAB_BANG, CARLSON_DRAW};

enum Types {AGR, DEF, NEU};

enum Suits {SRDCE, KARY, PIKY, KRIZE};

static std::string Suit_names[] {"Srdce", "Kary", "Piky", "Krize"};

static std::string Card_names[] {"", "Bang", "Vedle", "Pivo", "Salon", "Wells Fargo", "Dostavnik",
                                "Hokynarstvi", "Panika", "Cat Balou", "Indiani", "Kulomet", "Duel",
                                "Dynamit", "Vezeni", "Barel", "Mustang", "Appaloosa", "Volcanic",
                                "Schofield", "Remington", "Carabine", "Winchester", "Bang", ""};

class Card {
public:
    Card() : id(INT_MAX), name(""), edge('.'), suit(-1), rank(INT_MAX), card_type(-1) {}

    Card(int id, const std::string& name, char edge, int suit,
         int rank, int type, int range, int mode) : id(id),
        name(name), edge(edge), suit(suit), rank(rank), card_type(type),
        dyn_active(false), range(range), mode(mode) {}

    std::string file_loc();
    std::string suit_loc();
    std::string rnk();
    bool play(int position, PlayerData& pd);

	int id;
	std::string name;
	char edge;//Modry, Hnedy okraj
    int suit;
	int rank;
    int card_type;//Agro, Def, Neut
    bool dyn_active;//dynamit doutna, takze nemuze bouchnout
	int range;//jedine v pripade, ze se jedna o zbran
    int mode;
};

#endif
