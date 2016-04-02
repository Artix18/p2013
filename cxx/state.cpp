#include "state.h"

state::state(bool p_player, map<position,int> *p_island, map<int, bateau> *p_boat, int p_max_idBoat)
{
	max_idBoat = p_max_idBoat;
	player = p_player;

	for(int i = 0 ; i < 3 ; i++)
		island[i] = p_island[i];
	for(int i = 0 ; i < 2 ; i++)
		boat[i] = p_boat[i];
}

void state::possibleActions(vector<shared_ptr<action>>& p)
{
	for(auto isl : island[player+1])
	{
		//Build actions
		if(boat[player].size() < LIMITE_BATEAUX)
		{
			if(CARAVELLE_COUT <= isl.second)
				p.push_back(shared_ptr<action>(new action_build(isl.first,0)));
			
			
			if(GALION_COUT <= isl.second)
				p.push_back(shared_ptr<action>(new action_build(isl.first,1)));
		}
		
		//Colonize action
		for(auto b : boat[player])
			if(b.second.btype == BATEAU_CARAVELLE)
				if(island[0].find(b.second.pos) != island[0].end())
					p.push_back(shared_ptr<action>(new action_colonize(b.second.pos)));

		//Move actions
		int dir[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
		for(auto b: boat[player])
			if(b.second.deplacable)
			{
				vector<position> accessible;
				queue<pair<position,int>> aVisiter;
				aVisiter.push(make_pair(b.second.pos,0));
				map<position,bool> vu;

				while(!aVisiter.empty())
				{
					position curr = aVisiter.front().first;
					int dist = aVisiter.front().second;

					if(dist != 0)
						accessible.push_back(curr);

					aVisiter.pop();

					for(int iDir = 0 ; iDir < 4 ; iDir++)
					{
						position voisin{curr.x+dir[iDir][0],curr.y+dir[iDir][1]};
						if(valide(voisin))
							if(vu.find(voisin) == vu.end() && distance(voisin,b.second.pos) <= ((bool)b.second.btype) ? GALION_DEPLACEMENT : CARAVELLE_DEPLACEMENT)
								aVisiter.push(make_pair(voisin,dist+1));
					}
				}

				for(auto a : accessible)
					p.push_back(shared_ptr<action>(new action_move(b.second.id,a)));
			}
		

		//Transfer action island
		for(auto b: boat[player])
			if(b.second.btype == BATEAU_CARAVELLE && island[player+1].find(b.second.pos) != island[player+1].end())
			{
				for(int m = 1 ; m <= island[player+1][b.second.pos] ; m++)
					p.push_back(shared_ptr<action>(new action_transfer_island(b.first,m)));
				for(int m = 1 ; m <= b.second.nb_or ; m++)
					p.push_back(shared_ptr<action>(new action_transfer_island(b.first,-m)));
			}

		//Transfer boat boat
		for(auto b1: boat[player])
			for(auto b2: boat[player])
				if(b1.first != b2.first)
					for(int m = 1 ; m <= b1.second.nb_or ; m++)
						p.push_back(shared_ptr<action>(new action_transfer_boat(b1.first, b2.first, m)));
		
		//End turn action
		p.push_back(shared_ptr<action>(new action_endTurn()));
	}
}

float state::getScore()
{
	int s=0;
	for(auto t: island[player+1])
		s += t.second;
	for(auto b: boat[player])
		s += ((b.second.btype == BATEAU_CARAVELLE) ? CARAVELLE_COUT : GALION_COUT) + b.second.nb_or;



	return s;
}