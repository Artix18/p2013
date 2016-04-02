#include "genome.h"
#include "state.h"

void genome::random(state s)
{
	bool player = s.player;
	
	while(player == s.player)
	{
		vector<shared_ptr<action>> possible;
		s.possibleActions(possible);

		for(auto a: possible)
			printf("\t%s\n", a->name().c_str());
		if(possible.size() == 0)
		{
			genes.push_back(shared_ptr<action>(new action_endTurn()));
			break;
		}
		int idAction = rand()%possible.size();
		possible[idAction]->applyTo(&s);
		printf("Chosen : %s\n", possible[idAction]->name().c_str());
		genes.push_back(possible[idAction]);
	}
}