///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#include "prologin.hh"
#include "state.h"
#include "genome.h"
#include <unistd.h>
///
// Fonction appelée au début de la partie
//
void partie_init()
{
  // fonction a completer


}

///
// Fonction appelée à chaque tour
//
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
	//printf("G %d\n", get<1>(island[1][position({1,1})]));
	state curr_state(player,island,boat,max_idBoat);
	genome g;
	//printf("F %d\n", get<1>(curr_state.island[1][position{1,1}]));
	g.random(curr_state);

	/*float dmoymoy = 1000*1000*1000;
	bool a = false;
	for(auto b: boat[0])
	{
		if(!a)
		{
			a = true;
			dmoymoy = 0;
		}
		float dmoy = 0;
		for(auto i : island[0])
			dmoy += (float)distance(b.second.pos,i.first);
		dmoy /= island[0].size();
		dmoymoy += dmoy;
	}

	dmoymoy /= boat[0].size();

	printf("%f\n", boat[0].size()*CARAVELLE_COUT+(1/dmoymoy) * (island[0].size()+island[2].size())/island[1].size());

	exit(1);*/
	
}

///
// Fonction appelée à la fin de la partie
//
void partie_fin()
{
  // fonction a completer
}

