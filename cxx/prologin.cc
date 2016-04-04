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

long int seed;

void partie_init()
{
  // fonction a completer
  seed = time(NULL);
  srand(seed);
  printf("Seed : %ld\n", seed);

}

#define POP_SIZE 10

///
// Fonction appelée à chaque tour
//

bool compare(const unique_ptr<genome>& a, const unique_ptr<genome>& b)
{
	return a->fit > b->fit;
}


void action_to_Action(unique_ptr<action> a)
{

		//printf("Doing %s\n", a->name().c_str());

		if(action_endTurn* b = dynamic_cast<action_endTurn*>(a.get()))
		{
			return;
		}
		else if(action_build* b = dynamic_cast<action_build*>(a.get()))
		{
			afficher_erreur(construire((b->what) ? BATEAU_GALION : BATEAU_CARAVELLE , b->where));
			return;
		}
		else if(action_move* b = dynamic_cast<action_move*>(a.get()))
		{
			afficher_erreur(deplacer(b->id, b->whereTo));
			return;
		}
		else if(action_colonize* b = dynamic_cast<action_colonize*>(a.get()))
		{
			afficher_erreur(coloniser(b->where));
			return;
		}
}

void jouer_tour()
{
	
	map<pair<int,int>,int> island[3];
	map<pair<int,int>,bool> not_volcano;
	map<int,bateau> boat[2];

	for(int x = 0 ; x < TAILLE_TERRAIN ; x++)
		for(int y = 0 ; y < TAILLE_TERRAIN ; y++)
		{
			auto t = info_terrain(position({x,y}));
			not_volcano[make_pair(x,y)] = true;
			if(t != TERRAIN_ERREUR && t != TERRAIN_MER)
			{
				//printf("OO\n");
				int owner = info_ile_joueur(position{x,y});
				owner = (owner == -1) ? 0 : owner;
				island[owner][pair_from_pos(position{x,y})] = info_ile_or({x,y});
				not_volcano[make_pair(x,y)] = (t == TERRAIN_ILE);
			}

			for(auto b : liste_bateaux_position(position{x,y}))
				boat[b.joueur-1][b.id] = b;

		}


	bool player = mon_joueur()-1;

	//printf("\nTurn of %d\n", player);

	state curr_state(player,island,boat,not_volcano,id_dernier_bateau_construit());
	vector<unique_ptr<genome>> pop;
	for(int iGuy = 0 ; iGuy < POP_SIZE ; iGuy++)
	{
		genome *g = new genome();
		g->randomGuy(curr_state);
		pop.push_back(unique_ptr<genome>(g));
	}

	sort(pop.begin(), pop.end(), compare);

	/*for(unique_ptr<genome>& a : pop)
		printf("%f\n", a->fit);*/

	printf("\nI should do : \n");
	for(unique_ptr<action>& a : pop[0]->genes)
		printf("%s\n", a->name().c_str());
	printf("\n");

	//Last give up owner ship
	for(int iAction = 0 ; iAction < pop[0]->genes.size() ; iAction++)
		action_to_Action(move(pop[0]->genes[iAction]));
}

///
// Fonction appelée à la fin de la partie
//
void partie_fin()
{
  // fonction a completer
}

