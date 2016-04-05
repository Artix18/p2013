#include "genome.h"
#include "state.h"

#include <algorithm>
using namespace std;

void genome::randomGuy(state s)
{
	bool player = s.player;
	fit = s.getScore();
	//printf("%d %d\n", s.boat[s.player].size(), s.island[s.player+1].size());
	while(player == s.player)
	{
		vector<unique_ptr<action>> possible;
		s.possibleActions(possible);
		//printf("\t%d\n", possible.size());
		//for(unique_ptr<action>& a: possible)
			//printf("\t%s\n", a->name().c_str());
		if(possible.size() == 0)
		{
			genes.push_back(unique_ptr<action>(new action_endTurn()));
			break;
		}
		int idAction = rand()%possible.size();
		possible[idAction]->applyTo(&s);
		fit = s.getScore(player != s.player);
		genes.push_back(move(possible[idAction]));
	}
	//printf("\n");
}

void genome::cross_over(genome* a, genome* b, state s)
{
	/*printf("\nCross Over Kikou %d %d\n\n", a->genes.size(), b->genes.size());
	for(auto& ac : a->genes)
		printf("\t%s\n", ac->name().c_str());
	printf("\n");
	for(auto& ac : b->genes)
		printf("\t%s\n", ac->name().c_str());
	printf("\n");*/
	vector<unique_ptr<action>> possible;
	for (int i = 0; i < max(a->genes.size(), b->genes.size()); i++) 
	{
		s.possibleActions(possible);
		
		if(possible.size() == 0)
		{
			genes.push_back(unique_ptr<action>(new action_endTurn()));
			break;
		}
		
		if ((rand()%2) && hasAction(a, possible, i))
			genes.push_back(a->genes[i]->clone());
		else if (hasAction(b, possible, i))
			genes.push_back(b->genes[i]->clone());
		else if (hasAction(a, possible, i))
			genes.push_back(a->genes[i]->clone());
		else 
		{
			if (hasAction(a, possible, i+1))
				genes.push_back(a->genes[i+1]->clone());
			else if (hasAction(b, possible, i+1))
				genes.push_back(b->genes[i]->clone());
			else
				genes.push_back(move(possible[rand()%possible.size()]));
		}
		
		//printf("\t%s\n", genes[i]->name().c_str());
		genes[i]->applyTo(&s);
		if(genes[i]->name() == "End Of Turn")
			break;
	}
	//printf("\n");
}

bool genome::hasAction(genome* a, const vector<unique_ptr<action>>& v, int index)
{
	if (a->genes.size() <= index)
		return false;

	bool hasIt = false;
	for(auto& ac: v)
		if(a->genes[index]->name() == ac->name())
		{
			hasIt = true;
			break;
		}

	return hasIt;
}