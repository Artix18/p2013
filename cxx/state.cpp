#include "state.h"

state::state(bool p_player, map<pair<int,int>,int> *p_island, map<int, bateau> *p_boat, map<pair<int,int>,bool> p_not_volcano, int p_max_idBoat)
{
	max_idBoat = p_max_idBoat;
	player = p_player;

	for(int i = 0 ; i < 3 ; i++)
		island[i] = p_island[i];
	for(int i = 0 ; i < 2 ; i++)
		boat[i] = p_boat[i];
	not_volcano = p_not_volcano;
}

void state::possibleActions(vector<unique_ptr<action>>& p)
{
	for(auto isl : island[player+1])
	{
		//Build actions
		if(not_volcano[isl.first])
		{
			if(boat[player].size() < LIMITE_BATEAUX)
			{
				if(CARAVELLE_COUT <= isl.second)
					p.push_back(unique_ptr<action>(new action_build(position{isl.first.first,isl.first.second},0)));
			
			
				if(GALION_COUT <= isl.second)
					p.push_back(unique_ptr<action>(new action_build(position{isl.first.first,isl.first.second},1)));
			}
		}
		
		//Colonize action
		for(auto b : boat[player])
		{
			if(b.second.btype == BATEAU_CARAVELLE)
				if(island[0].find(make_pair(b.second.pos.x,b.second.pos.y)) != island[0].end())
					p.push_back(unique_ptr<action>(new action_colonize(b.second.pos)));
		}
				
		//Move actions
		int dir[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
		for(auto b: boat[player])
			if(b.second.deplacable)
			{
				vector<position> accessible;
				queue<pair<position,int>> aVisiter;
				aVisiter.push(make_pair(b.second.pos,0));
				map<pair<int,int>,bool> vu;
				vu[pair_from_pos(b.second.pos)] = true;

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
						if(valide(voisin) && (vu.find(pair_from_pos(voisin)) == vu.end()))
						{
							int dep = 0;
							switch(b.second.btype)
							{
								case BATEAU_CARAVELLE:
									dep = CARAVELLE_DEPLACEMENT;
									break;
								case BATEAU_GALION:
									dep = GALION_DEPLACEMENT;
									break;
							}
							if(dist+1 <=  dep)
							{
								vu[pair_from_pos(voisin)] = true;
								aVisiter.push(make_pair(voisin,dist+1));
							}
						}
					}
				}

				for(auto a : accessible)
					p.push_back(unique_ptr<action>(new action_move(b.second.id,a)));
			}
		
		/*	
		//Transfer action island
		for(auto b: boat[player])
		{
			if(flag_transfer_island[player].find(b.first) == flag_transfer_island[player].end())
			{
				if(b.second.btype == BATEAU_CARAVELLE && island[player+1].find(pair_from_pos(b.second.pos)) != island[player+1].end())
				{
					if(island[player+1][pair_from_pos(b.second.pos)] != 0)
						for(int m = min(TRANSFER_RATE,island[player+1][pair_from_pos(b.second.pos)]) ; m <= island[player+1][pair_from_pos(b.second.pos)] ; m += TRANSFER_RATE)
							p.push_back(unique_ptr<action>(new action_transfer_island(b.first,m)));
					if(b.second.nb_or != 0)
						for(int m = min(TRANSFER_RATE,b.second.nb_or) ; m <= b.second.nb_or ; m += TRANSFER_RATE)
							p.push_back(unique_ptr<action>(new action_transfer_island(b.first,-m)));
				}
			}
		}

		
		//Transfer boat boat
		for(auto b1: boat[player])
		{
			for(auto b2: boat[player])
			{
				if(b1.first != b2.first && b1.second.pos == b2.second.pos)
				{
					if(flag_transfer_boat[player].find(make_pair(b1.first,b2.first)) == flag_transfer_boat[player].end())
					{
						if(b1.second.nb_or != 0)
							for(int m = min(TRANSFER_RATE,b1.second.nb_or) ; m <= b1.second.nb_or ; m += TRANSFER_RATE)
								p.push_back(unique_ptr<action>(new action_transfer_boat(b1.first, b2.first, m)));
					}
				}
			}
		}*/
		
		//End turn action
		p.push_back(unique_ptr<action>(new action_endTurn()));
	}
}

float state::getScore(bool reverse /* = false */)
{
	bool my_player = player;
	if(reverse)
		my_player = 1-player;

	float s=0;
	for(auto t: island[my_player+1])
		s += t.second;
	int nbCar = 0, nbGal = 0;
	for(auto b: boat[my_player])
	{
		if(b.second.btype == BATEAU_CARAVELLE)
			nbCar++;
		if(b.second.btype == BATEAU_GALION)
			nbGal++;
		s += ((b.second.btype == BATEAU_CARAVELLE) ? CARAVELLE_COUT : GALION_COUT) + b.second.nb_or;
	}

	s += 10*island[my_player+1].size() + (float(island[0].size())/float(island[0].size()+island[1].size()+island[2].size()))*nbCar + (float(island[1-my_player].size())/float(island[0].size()+island[1].size()+island[2].size()))*nbGal;
	//Moyenne des distances moyennes des caravelles aux iles vierges
	/*float dmoymoy = 0.0;
	int nbCar = 0;
	for(auto b: boat[player])
	{
		float dmoy = 0.0;
		if(b.second.btype == BATEAU_CARAVELLE)
		{
			nbCar++;
			for(auto isl: island[0])
				dmoy += d::distance(pos_from_pair(isl.first),b.second.pos);
			dmoy /= island[0].size();
		}
		dmoymoy += dmoy;
	}

	if(nbCar != 0)
		dmoymoy /= nbCar;

	s += (nbCar != 0)*float(island[0].size())/float(island[0].size()+island[1].size()+island[2].size())*(2*float(TAILLE_TERRAIN)-dmoymoy);

	dmoymoy = 0.0;
	int nbGal = 0;
	for(auto b: boat[player])
	{
		float dmoy = 0.0;
		if(b.second.btype == BATEAU_GALION)
		{
			nbGal++;
			for(auto isl: island[2-player])
				dmoy += d::distance(pos_from_pair(isl.first),b.second.pos);
			
			dmoy /= island[2-player].size();
		}
		dmoymoy += dmoy;
	}


	if(nbGal != 0)
		dmoymoy /= nbGal;
	s += (nbGal != 0)*float(island[0].size())/float(island[0].size()+island[1].size()+island[2].size())*(2*float(TAILLE_TERRAIN)-dmoymoy);
	*/
	return s;
}