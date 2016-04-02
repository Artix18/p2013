#include "actions.h"
#include "state.h"

void action_endTurn::applyTo(state* s)
{
	s->player = !s->player;
}

void action_build::applyTo(state* s)
{
	s->boat[s->player][s->max_idBoat+1] = bateau{s->max_idBoat+1,where,s->player,(bateau_type)what,0,false};
	s->max_idBoat++;
	s->island[s->player+1][where] -= (what) ? GALION_COUT : CARAVELLE_COUT;
}

void action_colonize::applyTo(state* s)
{
	s->island[s->player+1][where] = s->island[0][where];
	s->island[0].erase(where);
}

void action_move::applyTo(state* s)
{
	s->boat[s->player][id].pos = whereTo;
	s->boat[s->player][id].deplacable = false;
}

void action_transfer_island::applyTo(state* s)
{
	s->boat[s->player][id].nb_or += money;
	s->island[s->player+1][s->boat[s->player][id].pos] += -money;
}

void action_transfer_boat::applyTo(state* s)
{
	s->boat[s->player][idFrom].nb_or -= money;
	s->boat[s->player][idTo].nb_or += money;	
}