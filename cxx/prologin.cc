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
#include <signal.h>
#include <string.h>
#include <sys/time.h>
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


int bon_code(int c)
{
	if(c > 2)
		return 2;
	return c;
}

void jouer_tour()
{
	static int POP_SIZE = 500;
	static float eliteRate = 1.0/2;
	static float mutRate = 0.1;

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
				int owner = bon_code(info_ile_joueur(position{x,y}));
				owner = (owner == -1) ? 0 : owner;
				island[owner][pair_from_pos(position{x,y})] = info_ile_or({x,y});
				not_volcano[make_pair(x,y)] = (t == TERRAIN_ILE);
			}
			
			for(auto b : liste_bateaux_position(position{x,y}))
			{
				bateau bonBat = b;
				bonBat.joueur = bon_code(b.joueur);
				boat[bonBat.joueur-1][bonBat.id] = bonBat;
			}

		}


	bool player = bon_code(mon_joueur())-1;

	printf("\nTurn of %d\n", player);
	
	state curr_state(player,island,boat,not_volcano,id_dernier_bateau_construit());
	vector<unique_ptr<genome>> pop;
	struct timeval stop, start, result;
	for(int iGuy = 0 ; iGuy < POP_SIZE ; iGuy++)
	{
		gettimeofday(&start, NULL);
		genome *g = new genome();
		g->randomGuy(curr_state);
		pop.push_back(unique_ptr<genome>(g));
		gettimeofday(&stop, NULL);
		timersub(&stop, &start, &result);
		if((long int)result.tv_usec >= (1e6/float(POP_SIZE))/3 && POP_SIZE >= 2)
		{
			printf("Down grade POP_SIZE\n");

			POP_SIZE /= 2;
			break;
		}

	}
	sort(pop.begin(), pop.end(), compare);

	int toKill = pop.size()*eliteRate; 
	for(int i = 0 ; i < toKill ; i++)
		pop.pop_back();


	for(int i = 0 ; i < toKill ; i++)
	{
		int a = rand()%pop.size();
		int b = rand()%pop.size();
		while(a == b)
			b = rand()%pop.size();

		genome *g = new genome();
		g->cross_over(pop[a].get(), pop[b].get(), curr_state);

		double prob = (double)rand() / (double)RAND_MAX;
		if(prob < mutRate)
		{
			/*printf("Mutate\n");
			for(auto& ac : g->genes)
				printf("\t%s\n", ac->name().c_str());*/
			g->mutate(curr_state);
			/*printf("\n");
			for(auto& ac : g->genes)
				printf("\t%s\n", ac->name().c_str());*/
		}
		
		pop.push_back(unique_ptr<genome>(g));
	}


	
	printf("POP_SIZE: %d\n", POP_SIZE);
	printf("I should do : %d\n", pop[0]->genes.size());
	for(const unique_ptr<action>& a : pop[0]->genes)
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

