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

		void random(state s);

		vector<shared_ptr<action>> genes;
};

#endif
