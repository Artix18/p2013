#include "genome.h"
#include "state.h"

void genome::randomGuy(state s)
{
	bool player = s.player;
	fit = s.getScore();
	while(player == s.player)
	{
		vector<unique_ptr<action>> possible;
		s.possibleActions(possible);

		//for(unique_ptr<action>& a: possible)
			//printf("\t%s\n", a->name().c_str());
		if(possible.size() == 0)
		{
			genes.push_back(unique_ptr<action>(new action_endTurn()));
			break;
		}
		int idAction = rand()%possible.size();
		possible[idAction]->applyTo(&s);
		//printf("Chosen : %s\n", possible[idAction]->name().c_str());
		if(player == s.player)
			fit = s.getScore();
		genes.push_back(move(possible[idAction]));
	}
}