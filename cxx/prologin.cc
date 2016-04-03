///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"
#include "state.h"
#include "genome.h"
#include <unistd.h>
#include <algorithm>
///
// Fonction appelée au début de la partie
//
void partie_init()
{
  // fonction a completer


}

#define POP_SIZE 500

///
// Fonction appelée à chaque tour
//

bool compare(const unique_ptr<genome>& a, const unique_ptr<genome>& b)
{
	return a->fit > b->fit;
}

void jouer_tour()
{
	srand(time(NULL));

	map<position,int> island[3];
	map<int,bateau> boat[2];

	int max_idBoat = 0;
	for(int x = 0 ; x < TAILLE_TERRAIN ; x++)
		for(int y = 0 ; y < TAILLE_TERRAIN ; y++)
		{
			auto t = info_terrain(position({x,y}));
			if(t != TERRAIN_ERREUR)
			{
				int owner = info_ile_joueur(position{x,y});
				owner = (owner == -1) ? 0 : owner;
				island[owner][position{x,y}] = info_ile_or({x,y});
			}
			for(auto b: liste_bateaux_position(position{x,y}))
			{
				max_idBoat = max(max_idBoat,b.id);
				boat[b.joueur-1][b.id] = b;		
			}

		}

	bool player = 2-adversaire();

	state curr_state(player,island,boat,id_dernier_bateau_construit());
	
	vector<unique_ptr<genome>> pop;
	for(int iGuy = 0 ; iGuy < POP_SIZE ; iGuy++)
	{
		genome *g = new genome();
		g->randomGuy(curr_state);
		pop.push_back(unique_ptr<genome>(g));
	}

	sort(pop.begin(), pop.end(), compare);
	printf("I should do : \n");
	for(unique_ptr<action>& a : pop[0]->genes)
		printf("%s\n", a->name().c_str());
}

///
// Fonction appelée à la fin de la partie
//
void partie_fin()
{
  // fonction a completer
}

