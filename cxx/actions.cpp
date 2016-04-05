#include "actions.h"
#include "state.h"

void action_endTurn::applyTo(state* s)
{
	/* Handle combat */
	set<pair<int,int>> vu;
	for(auto b: s->boat[s->player])
		if(vu.find(pair_from_pos(b.second.pos)) == vu.end())
		{
			bool enMer = (info_terrain(b.second.pos) == TERRAIN_MER); 
			vu.insert(pair_from_pos(b.second.pos));
			int nbGalMoi = 0, nbGalLui = 0;
			for(auto b2 : s->boat[s->player])
				if(b2.second.btype == BATEAU_GALION && b2.second.pos == b.second.pos)
					nbGalMoi++;
			for(auto b2 : s->boat[1-s->player])
				if(b2.second.btype == BATEAU_GALION && b2.second.pos == b.second.pos)
					nbGalLui++;

			if(nbGalLui == 0)
				continue;

			if(nbGalMoi > nbGalLui || (nbGalMoi == nbGalLui && enMer))
			{
				vector<int> toEraseMoi;
				for(auto b2 : s->boat[s->player])
					if(b2.second.btype == BATEAU_GALION && b2.second.pos == b.second.pos)
					{
						toEraseMoi.push_back(b2.first);
						if(toEraseMoi.size() > nbGalLui)
							break;
					}
				for(int i : toEraseMoi)
					s->boat[s->player].erase(i);

				vector<int> toEraseLui;
				int money = 0;
				for(auto b2 : s->boat[1-s->player])
					if(b2.second.pos == b.second.pos)
					{
						toEraseLui.push_back(b2.first);
						if(b2.second.btype == BATEAU_CARAVELLE)
							money += b2.second.nb_or;
					}
				if(money != 0)
				{
					int minId = b.first;
					for(auto b2 : s->boat[s->player])
						if(b2.second.pos == b.second.pos && b2.second.btype == BATEAU_CARAVELLE)
							minId = min(minId, b2.first);

					s->boat[s->player][minId].nb_or += money;
				}

				for(int i : toEraseLui)
					s->boat[1-s->player].erase(i);

				if(!enMer && s->island[1-s->player+1].find(pair_from_pos(b.second.pos)) != s->island[1-s->player+1].end())
				{
					s->island[s->player+1][pair_from_pos(b.second.pos)] = s->island[1-s->player+1][pair_from_pos(b.second.pos)];
					s->island[1-s->player+1].erase(pair_from_pos(b.second.pos));
				}
			}
			else if(nbGalLui > nbGalMoi || (nbGalMoi == nbGalLui && !enMer))
			{
				vector<int> toEraseLui;
				for(auto b2 : s->boat[1-s->player])
					if(b2.second.btype == BATEAU_GALION && b2.second.pos == b.second.pos)
					{
						toEraseLui.push_back(b2.first);
						if(toEraseLui.size() > nbGalMoi)
							break;
					}
				for(int i : toEraseLui)
					s->boat[1-s->player].erase(i);

				vector<int> toEraseMoi;
				for(auto b2 : s->boat[s->player])
					if(b2.second.pos == b.second.pos)
						toEraseMoi.push_back(b2.first);

				for(int i : toEraseMoi)
					s->boat[s->player].erase(i);

				if(!enMer && s->island[s->player+1].find(pair_from_pos(b.second.pos)) != s->island[s->player+1].end())
				{
					s->island[1-s->player+1][pair_from_pos(b.second.pos)] = s->island[s->player+1][pair_from_pos(b.second.pos)];
					s->island[s->player+1].erase(pair_from_pos(b.second.pos));
				}

			}
		}


	s->player = !s->player;
	
	
}

void action_build::applyTo(state* s)
{
	s->boat[s->player][s->max_idBoat+1] = bateau{s->max_idBoat+1,where,s->player,(bateau_type)what,0,false};
	s->max_idBoat++;
	s->island[s->player+1][pair_from_pos(where)] -= (what) ? GALION_COUT : CARAVELLE_COUT;
}

void action_colonize::applyTo(state* s)
{
	s->island[s->player+1][pair_from_pos(where)] = s->island[0][pair_from_pos(where)];
	s->island[0].erase(pair_from_pos(where));
}

void action_move::applyTo(state* s)
{
	s->boat[s->player][id].pos = whereTo;
	s->boat[s->player][id].deplacable = false;
}

void action_transfer_island::applyTo(state* s)
{
	s->flag_transfer_island[s->player][id] = true;
	s->boat[s->player][id].nb_or += money;
	s->island[s->player+1][pair_from_pos(s->boat[s->player][id].pos)] += -money;
}

void action_transfer_boat::applyTo(state* s)
{
	s->flag_transfer_boat[s->player][make_pair(idFrom,idTo)] = true;
	s->boat[s->player][idFrom].nb_or -= money;
	s->boat[s->player][idTo].nb_or += money;	
}