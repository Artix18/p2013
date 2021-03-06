#ifndef DEF_STATE_H
#define DEF_STATE_H

#include <tuple>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include "prologin.hh"
#include "actions.h"

using namespace std;

#define TRANSFER_RATE 10 //transfer money by packs of

class state
{

	public:
		state() {}
		state(bool p_player, map<pair<int,int>,int> *p_island, map<int, bateau> *p_boat, map<pair<int,int>,bool> p_not_volcano, int p_max_idBoat);

		void possibleActions(vector<unique_ptr<action>>& p);

		map<pair<int,int>, int> island[3];//neutral,p1,p2; pos, type, gold
		map<int,bateau> boat[2];
		map<int,bool> flag_transfer_island[2]; //first: transfer_island, second: transfer_boat
		map<pair<int,int>,bool> flag_transfer_boat[2];
		map<pair<int,int>,bool> not_volcano;

		bool player;//0 p1, 1 p2

		int max_idBoat;

		float getScore(bool reverse = false);//reverse, score of other player, useful for endTurn that changes player but we want last's one score
};

#endif
