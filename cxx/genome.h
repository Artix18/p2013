#ifndef DEF_GEN_H
#define DEF_GEN_H

#include <memory>
#include <vector>

#include "actions.h"

using namespace std;

class state;

class genome
{
	public:
		genome() {}

		void randomGuy(state s);
		void cross_over(genome* a, genome* b, state s);
		void mutate(state s);
		bool hasAction(genome* a, const vector<unique_ptr<action>>& v, int index);

		vector<unique_ptr<action>> genes;



		float fit;
};

#endif
